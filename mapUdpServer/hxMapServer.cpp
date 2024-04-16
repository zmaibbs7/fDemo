#include "hxMapServer.h"

#include <QDebug>
#include <QtCore>

#include <chrono>
#include <fstream>
#include <mutex>
#include <codecvt>
#include <locale>

#include "protoMessage/header.pb.h"
#include "hv/UdpServer.h"
#include "hv/TcpServer.h"
#include "hv/hendian.h"
using namespace hv;

const int           port                = 8899;  // 端口
const size_t        MaxPacketSize       = 1200;  // 最大数据包大小
const std::string   Map_Res             = "C:/Users/zmaib/Project/dreame/build-hxMapEditor-Desktop_Qt_5_15_2_MSVC2019_64bit-Release/.config/map/";

#define PACK_NET_STREAM(str) UnPack::toBigEndianByteStream(str)

struct UnPack {
    static constexpr int head_body_length = 8; //拆包的包头长度
    static constexpr uint32_t magic_number = 0x12345678; //[*] 用不到的话后面删除，魔术用于判断
    static uint32_t data_length;
    
    // 转换为大端字节序的字节流
    static std::string toBigEndianByteStream(const std::string& data){
        std::string stream;
        UnPack::data_length = (uint32_t)data.size(); 
        stream.resize(sizeof(magic_number) + sizeof(UnPack::data_length) + data.size());
        uint8_t* buffer = reinterpret_cast<uint8_t*>(&stream[0]);  
        buffer = serialize<uint32_t>(buffer, magic_number);
        buffer = serialize<uint32_t>(buffer, UnPack::data_length);
        memcpy(buffer, data.data() , data.size());
        return stream;
    }
};
uint32_t UnPack::data_length = 0;

class hxFile
{
public:
    static std::string mapResPath;           // 资源地图的根路径
    std::string map_name = "";               // 地图的名称
    std::string file_name = "";              // 文件的名称
    int file_packet_total = 0;               // 预期的数据包总数
    std::map<int, std::vector<char>> file_packets; // 存储文件数据包的容器
    std::string file_whole_md5 = "";         // 整个文件的MD5校验和

    // 如果尚未设置，则设置初始文件信息
    void setFileInfo(const std::string& mapName, const std::string& fileName, int packetTotal, const std::string& fileMd5)
    {
        if (!file_name.empty())  return;

        qDebug() << u8"初始化文件信息，总数据包数:" << packetTotal;
        map_name = mapName;
        file_name = fileName;
        file_packet_total = packetTotal;
        file_whole_md5 = fileMd5;
    }

    // 处理接收的数据包并处理文件组装
    bool receive(int packetNum, const std::vector<char>& content)
    {
        file_packets[packetNum] = content;

        // 调试输出，用于跟踪数据包
        // qDebug() << "总计 = " << file_packet_total << ", 已接收 = " << file_packets.size();

        // 检查是否接收到所有数据包
        if (file_packets.size() < file_packet_total) {
            return false; // 尚未接收到所有数据包
        }

        // 处理过多的数据包
        if (file_packets.size() > file_packet_total) {
            qDebug() << "接收到过多的数据包！";
        }

        // 确保目录存在
        ensureDirectoryExists();

        // 组装并保存文件
        return assembleAndSaveFile();
    }

    // 文件拆包给发送做准备
    void splitToVector( std::vector<std::string> &sendVector)
    {
        std::string filePath = mapResPath + map_name + "/" + file_name;
        // 创建用于UTF-8到UTF-16的转换器
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring widePath = converter.from_bytes(filePath);
        std::cout << "文件路径 = " << filePath << std::endl;

        // 尝试以二进制读取模式打开文件
        std::ifstream infile(widePath, std::ios::binary | std::ios::ate);
        if (!infile.is_open()) {
            qDebug() << u8"打开文件失败:" << QString::fromStdString(filePath);
            return;
        }

        // 计算文件大小并重置流位置到文件开始
        std::streamsize size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        // 将整个文件读入一个字符向量中
        std::vector<char> content(static_cast<size_t>(size));
        if (!infile.read(content.data(), size)) {
            qDebug() << u8"读取文件失败:" << QString::fromStdString(filePath);
            return;
        }

        // 将内容分割成预定义最大大小的数据包
        std::vector<std::vector<char>> packets;
        packets.reserve((size + MaxPacketSize - 1) / MaxPacketSize);

        for (size_t offset = 0; offset < static_cast<size_t>(size); offset += MaxPacketSize) {
            size_t currentSize = (std::min)(MaxPacketSize, static_cast<size_t>(size) - offset);
            // size_t currentSize = (MaxPacketSize < ( size - offset)) ? MaxPacketSize : ( size - offset);
            packets.emplace_back(content.begin() + offset, content.begin() + offset + currentSize);
        }

        // 调试日志，记录文件和数据包信息
        qDebug() << QString::fromStdString(filePath) << u8".文件总大小 = " << size << u8" 数据包数量 = " << packets.size();

        // 创建并序列化每个数据包到发送向量中
        packet::Message filePacketMessage;
        filePacketMessage.set_type(packet::MessageType::FILE);
        packet::File* filePacket = filePacketMessage.mutable_file();
        filePacket->set_map_name(map_name);
        filePacket->set_file_name(file_name);

        for (size_t i = 0; i < packets.size(); ++i) {
            filePacket->set_file_content(packets[i].data(), packets[i].size());
            filePacket->set_file_packet_num(i);
            filePacket->set_file_packet_total(packets.size());

            std::string out;
            filePacketMessage.SerializeToString(&out);
            sendVector.emplace_back(PACK_NET_STREAM(out));
        }

        // 处理空文件情况，发送一个空数据包
        if (packets.empty()) {
            filePacket->clear_file_content(); 
            filePacket->set_file_packet_num(1);
            filePacket->set_file_packet_total(1); 
            std::string out;
            filePacketMessage.SerializeToString(&out);
            sendVector.emplace_back(PACK_NET_STREAM(out));
        }

        qDebug() << u8"文件被分割成" << packets.size() << u8"个数据包。";
    }
private:
    // 确保目标目录存在，如果不存在则创建
    void ensureDirectoryExists()
    {
        QString dirPath = QString::fromStdString(hxFile::mapResPath + map_name);
        QDir dir(dirPath);
        if (!dir.exists()) {
            if (dir.mkpath(".")) {
                qDebug() << "目录创建成功：" << dirPath;
            } else {
                qDebug() << "目录创建失败：" << dirPath;
            }
        }
    }

    // 从数据包组装文件并保存到磁盘
    bool assembleAndSaveFile()
    {
        std::string fullPath = hxFile::mapResPath + map_name + "/" + file_name;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring widePath = converter.from_bytes(fullPath);
        std::ofstream outfile(widePath, std::ios::binary | std::ios::out);
        if (!outfile.is_open()) {
            qDebug() << "打开文件写入失败：" << QString::fromStdString(fullPath);
            return false;
        }

        // 按顺序将数据包写入文件
        for (int i = 0; i < file_packet_total; ++i) {
            outfile.write(file_packets[i].data(), file_packets[i].size());
        }
        outfile.close();
        qDebug() << QString::fromStdString(file_name) << "文件组装完成，并已写入磁盘。";

        // 组装后清理数据包
        file_packets.clear();

        return true;
    }
};
std::string hxFile::mapResPath = Map_Res;

//仅当前cpp使用函数
namespace internal
{
    //修改接收缓存大小
    bool setSocketRecvBufferSize(int sockfd, int size) {
        int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
        if (ret < 0) {
            perror("setsockopt SO_RCVBUF failed");
            return false;
        }
        return true;
    }
}

class hxMapServerDt
{
    public:
        packet::Message               msgReceive;       // udp接收的数据
        std::vector<sockaddr>         clientIPs;        // 保存ip用于算法检测结果发送，用完就删除
        std::map<std::string, hxFile> filesReceiver;    // 文件接收器

        TcpServer                     tcpServer;        // tcp服务器
        UdpServer                     udpServer;        // udp服务器
};

hxMapServer::hxMapServer(QObject *parent):QObject(parent)
{
    m_Dt = std::make_unique<hxMapServerDt>();

    // 初始化Protobuf库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    //创建udpServer
    {
        int bindfd = m_Dt->udpServer.createsocket(port, "127.0.0.1");
        if (bindfd < 0) {
            qDebug() << "udpServer create error!" << Qt::endl;
            return;
        }
        qDebug() << "udpServer create sucessed!" << Qt::endl;

        //udpServer onMessage处理
        m_Dt->udpServer.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
            m_Dt->msgReceive.ParseFromString(std::string((char*)buf->data(), (int)buf->size()));
            switch (m_Dt->msgReceive.type())
            {
                case packet::MessageType::HEARTBEAT:
                    m_Dt->udpServer.sendto(buf);
                    break;
                default:
                    break;
            }
        };
    }

    //创建tcpServer
    {
        int bindfd = m_Dt->tcpServer.createsocket(port, "127.0.0.1");
        if (bindfd < 0) {
            qDebug() << "tcpServer create error!" << Qt::endl;
            return;
        }
        qDebug() << "tcpServer create sucessed!" << Qt::endl;
        
        //设置缓冲区
        {
            int recvBufferSize = 2 * 1024 * 1024; // 10M
            if (!internal::setSocketRecvBufferSize(bindfd, recvBufferSize)) {
                qDebug() << "Failed to set receive buffer size" << Qt::endl;
                return;
            }
        }

        //设置拆包
        {
            unpack_setting_t unpack_setting;
            memset(&unpack_setting, 0, sizeof(unpack_setting_t));
            unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
            unpack_setting.mode = UNPACK_BY_LENGTH_FIELD;
            unpack_setting.body_offset = UnPack::head_body_length;
            unpack_setting.length_field_offset = 4;
            unpack_setting.length_field_bytes = 4;
            unpack_setting.length_adjustment = 0;
            unpack_setting.length_field_coding = ENCODE_BY_BIG_ENDIAN;
            m_Dt->tcpServer.setUnpack(&unpack_setting);
        }

        m_Dt->tcpServer.onConnection = [](const SocketChannelPtr& channel) {
            std::string peeraddr = channel->peeraddr();
            if (channel->isConnected()) {
                printf("m_Dt->tcpServer %s connected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
            } else {
                printf("m_Dt->tcpServer %s disconnected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
            }
        };

        m_Dt->tcpServer.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
            packet::Message msgReceive;
            msgReceive.ParseFromString(absl::string_view((char*)buf->data()+UnPack::head_body_length, buf->size()-UnPack::head_body_length));
            switch (msgReceive.type())
            {
                //接收请求 获取地图列表
                case packet::MessageType::MAPLIST:
                    channel->write(onReceiveMapRequest());
                    break;
                case packet::MessageType::FILE:
                    {
                        //接收请求 下载服务器地图
                        if(msgReceive.has_itemlist())
                        {
                            //获取地图名称 和 需要下载对应的文件名
                            const auto & file_list = msgReceive.itemlist();
                            std::string mapName = file_list.items(0);
                            std::vector<std::string> requestFileList;
                            //mapName
                            for(int i = 1; i < file_list.items_size(); i++)
                                requestFileList.emplace_back(file_list.items(i).c_str());

                            std::vector<std::string> send_file_vector;
                            onReceiveDownloadMapRequest(mapName, requestFileList, send_file_vector );
                            for(const auto& msg : send_file_vector)  channel->write(msg);
                        }
                        //接收请求 上传地图到服务器
                        else if(msgReceive.has_file())
                        {
                            const auto& file                = msgReceive.file();
                            const auto& mapName             = file.map_name();
                            const auto& fileName            = file.file_name();
                            const auto& file_packet_total   = file.file_packet_total();
                            const auto& file_md5            = file.whole_file_md5();
                            const auto& packetNum           = file.file_packet_num();
                            qDebug() << "server receive file - " << QString::fromStdString(fileName) << " mapNmae = " <<  QString::fromStdString(mapName) << Qt::endl;
                            std::vector<char> content(file.file_content().begin(), file.file_content().end());
                            //记录文件名
                            m_Dt->filesReceiver[fileName].setFileInfo(mapName, fileName, file_packet_total, file_md5);
                            //接收文件data
                            bool successed = m_Dt->filesReceiver[fileName].receive(packetNum, content);
                            if(successed) m_Dt->filesReceiver.erase(fileName);
                        }
                        break;
                    }                    
                default:
                    break;
            }
        };
    }

    //服务器默认开启
    start();
}

hxMapServer::~hxMapServer()
{
    // 清理Protobuf库
    google::protobuf::ShutdownProtobufLibrary();
}

void hxMapServer::start()
{
    m_Dt->udpServer.start();

    m_Dt->tcpServer.setThreadNum(10);
    m_Dt->tcpServer.setLoadBalance(LB_LeastConnections);
    m_Dt->tcpServer.start();
}

void hxMapServer::stop()
{
    m_Dt->udpServer.stop();
    m_Dt->tcpServer.stop();
}

void hxMapServer::updateAndSendLocation(double x, double y, double yaw)
{
}

void hxMapServer::onAlgorithmDetectionResult(const QString &result)
{

}

std::string hxMapServer::onReceiveMapRequest()
{
    qDebug() << "onReceiveMapRequest" << Qt::endl;
    std::string mapRequest;
    packet::Itemlist map_list;
    map_list.add_items("aaa");
    map_list.add_items("ccc");
    map_list.add_items(u8"布鲁塞尔");
    map_list.add_items(u8"工作区");
    packet::Message msgRequestMapList;
    msgRequestMapList.set_type(packet::MessageType::MAPLIST);
    *msgRequestMapList.mutable_itemlist() = map_list;
    msgRequestMapList.SerializeToString(&mapRequest);
    return PACK_NET_STREAM(mapRequest);
}

void hxMapServer::onReceiveDownloadMapRequest(std::string mapName, std::vector<std::string> fileList, std::vector<std::string> &sendVector)
{
    // 清空发送向量以确保它准备好接收新数据
    sendVector.clear();

    // 为调试目的记录地图名称
    std::cout << "地图名称 = " << mapName << std::endl;

    // 遍历文件列表处理每个文件
    for (const auto& fileName : fileList) {
        hxFile splitFile;
        splitFile.setFileInfo(mapName, fileName, 0, "");
        splitFile.splitToVector(sendVector);
    }
}

//仅当前cpp使用函数
bool setSocketRecvBufferSize(int sockfd, int size) {
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
    if (ret < 0) {
        perror("setsockopt SO_RCVBUF failed");
        return false;
    }
    return true;
}