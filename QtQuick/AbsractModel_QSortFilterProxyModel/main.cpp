#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "MapModel/MapModelManager.h"
#include "MapModel/MapPoints/MapPointsFilterProxyModel.h" //point ProxyModel

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("MapModelManager", &dm::MapModelManager::GetInstance());

    //放到MapModel里面
    qmlRegisterType<dm::MapPointsFilterProxyModel>("MapPointsFilterProxyModel", 1, 0, "MapPointsFilterProxyModel"); //point ProxyModel
    //path ProxyModel
    //region ProxyModel

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
