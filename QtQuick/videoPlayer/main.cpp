#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaService>

#include <QMediaRecorder>
#include <QAudioRecorder>
#include <QDebug>
#include <QStringList>

void printSupportedMediaTypes() {
    QAudioRecorder recorder;

    // 获取支持的音频编解码器
    QStringList audioCodecs = recorder.supportedAudioCodecs();
    qDebug() << "Supported audio codecs:";
    for (const QString &codecName : audioCodecs) {
        QString description = recorder.audioCodecDescription(codecName);
        qDebug() << codecName << ":" << description;
    }

    // 获取支持的容器格式
    QStringList containers = recorder.supportedContainers();
    qDebug() << "\nSupported containers:";
    for (const QString &containerName : containers) {
        QString description = recorder.containerDescription(containerName);
        qDebug() << containerName << ":" << description;
    }
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    printSupportedMediaTypes();


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
