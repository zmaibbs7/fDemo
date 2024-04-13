#include "hxMapUdpServer.h"

#include <QDebug>
#include <QtCore>

#include <chrono>
#include <vector>
#include <fstream>
#include <mutex>

#include "hxUdpProto/header.pb.h"
#include "hv/UdpServer.h"

// 平台相关头文件和库
#ifdef _WIN32
 #include <winsock2.h>
 #pragma comment(lib, "ws2_32.lib") // 链接到 Winsock 库
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

using namespace hv;

const int PacketMagicNumber = 95527;
const int port = 8899;
const std::string Map_Res = "C:/Users/zmaib/Project/Release/MapEditor/.config/map/";
class hxMapUdpServerDt
{
    public:
        packet::Message             msgReceive;
        std::vector<sockaddr>       clientIPs;  //保存ip用于算法检测结果发送，用完就删除
        UdpServer                   server;
        std::mutex                  mutex;
};

hxMapUdpServer::hxMapUdpServer(QObject *parent):QObject(parent)
{
    m_Dt = std::make_unique<hxMapUdpServerDt>();

    // 初始化Protobuf库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    //创建udp server
    {
        int bindfd = m_Dt->server.createsocket(port, "127.0.0.1");
        if (bindfd < 0) {
            qDebug() << "udp server create error!" << Qt::endl;
            return;
        }
        qDebug() << "udp server create sucessed!" << Qt::endl;
    }

    //onMessage处理
    m_Dt->server.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        m_Dt->msgReceive.ParseFromString(std::string((char*)buf->data(), (int)buf->size()));
        switch (m_Dt->msgReceive.type())
        {
            case packet::MessageType::HEARTBEAT:
                qDebug() << "send heart ->" << QString::fromStdString(channel->peeraddr())  << Qt::endl;
                m_Dt->server.sendto(buf);
                break;
            case packet::MessageType::MAPLIST:
                onReceiveMapRequest();
                break;
            case packet::MessageType::FILE:
                onReceiveFile();
                break;
            default:
                break;
        }
    };

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
    m_Dt->server.start();
}

void hxMapUdpServer::stop()
{
    m_Dt->server.stop();
}

void hxMapUdpServer::updateAndSendLocation(double x, double y, double yaw)
{
}

void hxMapUdpServer::onAlgorithmDetectionResult(const QString &result)
{

}

void hxMapUdpServer::onReceiveMapRequest()
{
    qDebug() << "onReceiveMapRequest" << Qt::endl;
    std::string mapRequest;
    packet::Maplist map_list;
    map_list.add_items("aaa");
    map_list.add_items("Vb");
    map_list.add_items(u8"布鲁塞尔");
    map_list.add_items(u8"反对党发");
    packet::Message msgRequestMapList;
    msgRequestMapList.set_type(packet::MessageType::MAPLIST);
    *msgRequestMapList.mutable_maplist() = map_list;
    msgRequestMapList.SerializeToString(&mapRequest);
    m_Dt->server.sendto(mapRequest);
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
    //     int re = m_Dt->server.sendto(out);
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