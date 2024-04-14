#include "hxMapUdpServer.h"

#include <QDebug>
#include <QtCore>

#include <chrono>
#include <fstream>
#include <mutex>

#include "hxUdpProto/header.pb.h"
#include "hv/UdpServer.h"
#include "hv/TcpServer.h"
#include "hv/hendian.h"
using namespace hv;

const int PacketMagicNumber = 95527;
const int port = 8899;
const std::string Map_Res = "D:/github/hxMapEditor/build/Release/.config/map/";

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

class hxMapUdpServerDt
{
    public:
        packet::Message             msgReceive;
        std::vector<sockaddr>       clientIPs;  //保存ip用于算法检测结果发送，用完就删除

        TcpServer                   tcpServer;
        UdpServer                   udpServer;
};

hxMapUdpServer::hxMapUdpServer(QObject *parent):QObject(parent)
{
    m_Dt = std::make_unique<hxMapUdpServerDt>();

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
        int bindfd = m_Dt->tcpServer.createsocket(9999, "127.0.0.1");
        if (bindfd < 0) {
            qDebug() << "tcpServer create error!" << Qt::endl;
            return;
        }
        qDebug() << "tcpServer create sucessed!" << Qt::endl;

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

            //获取包头长度
            printf("============tcpServer.onMessage.size = %d\n",(int)buf->size());
            // std::string s1((char*)buf->data(), (int)buf->size());
            // qDebug() << "============= requestMapNameList.size = " << s1.size() << Qt::endl;
            // for (unsigned char c : s1) {
            // std::cout << std::hex << (int)c << " ";
            // }
            // std::cout << std::endl;

            packet::Message msgReceive;
            msgReceive.ParseFromString(absl::string_view((char*)buf->data()+UnPack::head_body_length, buf->size()-UnPack::head_body_length));
            switch (msgReceive.type())
            {
                case packet::MessageType::MAPLIST:
                    channel->write(onReceiveMapRequest());
                    break;
                case packet::MessageType::FILE:
                    {
                        //获取地图名称 和 需要下载对应的文件名
                        const auto & file_list = msgReceive.maplist();
                        std::string mapName = file_list.items(0);
                        std::vector<std::string> requestFileList;
                        //mapName
                        for(int i = 1; i < file_list.items_size(); i++)
                            requestFileList.emplace_back(file_list.items(i).c_str());

                        std::vector<std::string> send_file_vector;
                        onReceiveDownloadMapRequest(mapName, requestFileList, send_file_vector );
                        for(const auto& msg : send_file_vector)  channel->write(msg);

                        // channel->write
                        // {
                            
                        //     // QString::fromUtf8(mapList.items(i).c_str())
                        //     qDebug() << "need send to client file -> " << QString::fromUtf8(file_list.items(i).c_str()) << Qt::endl;
                        // }
                        
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

hxMapUdpServer::~hxMapUdpServer()
{
    // 清理Protobuf库
    google::protobuf::ShutdownProtobufLibrary();
}

void hxMapUdpServer::start()
{
    m_Dt->udpServer.start();

    m_Dt->tcpServer.setThreadNum(10);
    m_Dt->tcpServer.setLoadBalance(LB_LeastConnections);
    m_Dt->tcpServer.start();
}

void hxMapUdpServer::stop()
{
    m_Dt->udpServer.stop();
    m_Dt->tcpServer.stop();
}

void hxMapUdpServer::updateAndSendLocation(double x, double y, double yaw)
{
}

void hxMapUdpServer::onAlgorithmDetectionResult(const QString &result)
{

}

std::string hxMapUdpServer::onReceiveMapRequest()
{
    qDebug() << "onReceiveMapRequest" << Qt::endl;
    std::string mapRequest;
    packet::Maplist map_list;
    map_list.add_items("aaa");
    map_list.add_items("Vb");
    map_list.add_items(u8"布鲁塞尔");
    map_list.add_items(u8"反对党发1");
    packet::Message msgRequestMapList;
    msgRequestMapList.set_type(packet::MessageType::MAPLIST);
    *msgRequestMapList.mutable_maplist() = map_list;
    msgRequestMapList.SerializeToString(&mapRequest);
    return PACK_NET_STREAM(mapRequest);
}

void hxMapUdpServer::onReceiveDownloadMapRequest(std::string mapName, std::vector<std::string> fileList, std::vector<std::string> &sendVector)
{
    //初始化清空 sendVector

    //test log
    {
        std::cout << "mapName = " << mapName << std::endl;


    }

    //打开每个文件并序列化
    std::string filepath = "";
    for(const auto& fileName : fileList)
    {
        filepath = Map_Res + mapName + "/" + fileName;
        std::cout << "filePath = " << filepath << std::endl;

        // 打开文件用于读取
        std::ifstream infile(filepath, std::ios::binary | std::ios::ate);
        if (!infile.is_open()) {
            qDebug() << "Failed to open file for reading:" << QString::fromStdString(filepath) << Qt::endl;
            continue;
        }

        std::streamsize size = infile.tellg(); // 获取文件大小
        infile.seekg(0, std::ios::beg); // 回到文件开头

        // 读取整个文件内容到字符串
        std::vector<char> content(size);
        if (!infile.read(content.data(), size)) {
            qDebug() << "Failed to read the file:" << QString::fromStdString(filepath) << Qt::endl;
            continue;
        }

        // 拆分为1200字节大小的包
        const size_t packetSize = 1200;
        std::vector<std::vector<char>> packets;

        // 预分配以避免多次重新分配
        packets.reserve((size + packetSize - 1) / packetSize);

        for (size_t offset = 0; offset < size; offset += packetSize) {
            size_t currentSize = (packetSize < ( size - offset)) ? packetSize : ( size - offset);
            packets.emplace_back(content.begin() + offset, content.begin() + offset + currentSize);
        }

        qDebug() << QString::fromStdString(fileName) <<  ".total = " << size << " packets.size = " << packets.size() << Qt::endl;

        packet::File filePacket;
        packet::Message filePacketMessage;
        filePacketMessage.set_type(packet::MessageType::FILE);
        filePacket.set_file_name(fileName);
        for (size_t i = 0; i < packets.size(); ++i) {
            //01 组包
            const auto& packet = packets[i];
            filePacket.set_file_content(packet.data(), packet.size());
            filePacket.set_file_packet_num(i);
            filePacket.set_file_packet_total(packets.size());
            *filePacketMessage.mutable_file() = filePacket;

            //02 发送打包后的 filePacketMessage
            std::string out;
            filePacketMessage.SerializeToString(&out);
            sendVector.emplace_back(PACK_NET_STREAM(out));
        }
        //文件为空的特殊情况
        if (packets.empty()) {
            filePacket.clear_file_content(); 
            filePacket.set_file_packet_num(1);
            filePacket.set_file_packet_total(1); 
            *filePacketMessage.mutable_file() = filePacket;
            std::string out;
            filePacketMessage.SerializeToString(&out);
            sendVector.emplace_back(PACK_NET_STREAM(out));
        }

        qDebug() << "File split into" << packets.size() << "packets." << Qt::endl;
    }
}

void hxMapUdpServer::onReceiveFile()
{
    //接收到请求
    qDebug() << "onReceiveFile" << Qt::endl;
    const auto& fileList = m_Dt->msgReceive.maplist();
    std::string mapName = fileList.items(0);

    for(int i = 1; i < fileList.items_size(); i++)
    {
        qDebug() << u8"准备发送 - " << QString::fromStdString(mapName) << " - " << QString::fromStdString(fileList.items(i)) << Qt::endl;
    }


    // 文件路径应基于实际文件位置进行调整
    std::string dirPath = Map_Res + mapName;

    // // 打开文件用于读取
    // std::ifstream infile(filepath, std::ios::binary | std::ios::ate);
    // if (!infile.is_open()) {
    //     qDebug() << "Failed to open file for reading:" << QString::fromStdString(filepath) << Qt::endl;
    //     return;
    // }

    // std::streamsize size = infile.tellg(); // 获取文件大小
    // infile.seekg(0, std::ios::beg); // 回到文件开头

    // // 读取整个文件内容到字符串
    // std::vector<char> content(size);
    // if (!infile.read(content.data(), size)) {
    //     qDebug() << "Failed to read the file:" << QString::fromStdString(filepath) << Qt::endl;
    //     return;
    // }

    // // 拆分为1200字节大小的包
    // const size_t packetSize = 1200;
    // std::vector<std::vector<char>> packets;

    // // 预分配以避免多次重新分配
    // packets.reserve((size + packetSize - 1) / packetSize);

    // for (size_t offset = 0; offset < size; offset += packetSize) {
    //     size_t currentSize = (packetSize < ( size - offset)) ? packetSize : ( size - offset);
    //     packets.emplace_back(content.begin() + offset, content.begin() + offset + currentSize);
    // }

    // qDebug() << QString::fromStdString(filename) <<  ".total = " << size << " packets.size = " << packets.size() << Qt::endl;

    // packet::File filePacket;
    // packet::Message filePacketMessage;
    // filePacketMessage.set_type(packet::MessageType::FILE);
    // filePacket.set_file_name(filename);
    // for (size_t i = 0; i < packets.size(); ++i) {
    //     //01 组包
    //     const auto& packet = packets[i];
    //     filePacket.set_file_content(packet.data(), packet.size());
    //     filePacket.set_file_packet_num(i);
    //     filePacket.set_file_packet_total(packets.size());
    //     *filePacketMessage.mutable_file() = filePacket;

    //     //02 发送打包后的 filePacketMessage
    //     std::string out;
    //     filePacketMessage.SerializeToString(&out);
    //     int re = m_Dt->udpServer.sendto(out);
    //     if(re < 0)  qDebug() << "send error" << Qt::endl;
    // }

    // qDebug() << "File split into" << packets.size() << "packets." << Qt::endl;
}

int hxMapUdpServer::cacuSendFile(std::string fileName, std::string fielPath, int packetSize)
{
    return 0;
}

void hxMapUdpServer::sendFile()
{
}

//void send_file(std::string fileName, std::string fielPath, int packetSize = 1200)