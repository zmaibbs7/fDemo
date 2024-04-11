#ifndef HXMAPUDPSERVER_H_
#define HXMAPUDPSERVER_H_

#include <memory>
#include <QObject>

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

public slots:
    //算法检测返回json
    void onAlgorithmDetectionResult(const QString& result);

signals:
    // 连接状态改变信号
    void connectionStatusChanged(bool isConnected);
    // 获取算法检测返回值信号
    void algorithmDetectionResultReceived(const QString& result);

private:
    void onControlMessage();   //控制消息处理
    void onFileDataMessage();  //文件消息处理
private:
    std::unique_ptr<hxMapUdpServerDt> m_Dt;
};
#endif //HXMAPUDPSERVER_H_