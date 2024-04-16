#ifndef HXMAPSERVER_H_
#define HXMAPSERVER_H_

#include <memory>
#include <QObject>
#include <vector>

class hxMapServerDt;
class hxMapServer : public QObject
{
    Q_OBJECT
public:
    hxMapServer(QObject *parent = nullptr);
    virtual ~hxMapServer();

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
    std::unique_ptr<hxMapServerDt> m_Dt;
};
#endif //HXMAPSERVER_H_