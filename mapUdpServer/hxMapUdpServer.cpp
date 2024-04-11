#include "hxMapUdpServer.h"

#include <QDebug>
#include <QtCore>

#include <chrono>
#include <vector>

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
class hxMapUdpServerDt
{
    public:
        packet::MessagePacketHeader msgReceive;
        std::vector<sockaddr>       clientIPs; //保存ip用于算法检测结果发送，用完就删除
        UdpServer                   server;
};

hxMapUdpServer::hxMapUdpServer(QObject *parent):QObject(parent)
{
    m_Dt = std::make_unique<hxMapUdpServerDt>();

    // 初始化Protobuf库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    //创建udp server
    {
        int bindfd = m_Dt->server.createsocket(port);
        if (bindfd < 0) {
            qDebug() << "udp server create error!" << Qt::endl;
            return;
        }
        qDebug() << "udp server create sucessed!" << Qt::endl;
    }

    //onMessage处理
    m_Dt->server.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        m_Dt->msgReceive.ParseFromString(std::string((char*)buf->data(), (int)buf->size()));
        switch (m_Dt->msgReceive.packet_type())
        {
            case packet::MessageType::HEARTBEAT:
                m_Dt->server.sendto(buf);
                break;
            case packet::MessageType::CONTROL:
                onControlMessage();
                break;
            case packet::MessageType::FILE:
                onFileDataMessage();
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

void hxMapUdpServer::onAlgorithmDetectionResult(const QString &result)
{

}

void hxMapUdpServer::onControlMessage()
{
}

void hxMapUdpServer::onFileDataMessage()
{
}
