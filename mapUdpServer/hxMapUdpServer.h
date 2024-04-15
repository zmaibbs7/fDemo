#ifndef HXMAPUDPSERVER_H_
#define HXMAPUDPSERVER_H_

#include <memory>
#include <QObject>
#include <vector>

class hxMapUdpServerDt;
class hxMapUdpServer : public QObject
{
    Q_OBJECT
public:
    hxMapUdpServer(QObject *parent = nullptr);
    virtual ~hxMapUdpServer();

    //开始连接
    void start();
    //停止连接
    void stop();
    //机器人实时位置获取并发送
    void updateAndSendLocation(double x, double y, double yaw);
public slots:
    //算法检测返回json
    void onAlgorithmDetectionResult(const QString& result);
signals:
    // 获取算法检测返回值信号
    void algorithmDetectionResultReceived(const QString& result);
private:
    // 接收 - 请求获取地图列表
    std::string onReceiveMapRequest();
    // 接收 - 请求下载地图
    void onReceiveDownloadMapRequest(std::string mapName, std::vector<std::string> fileList,std::vector<std::string> & sendVector);
private:
    std::unique_ptr<hxMapUdpServerDt> m_Dt;
};
#endif //HXMAPUDPSERVER_H_