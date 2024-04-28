#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "MapModel/MapModel.h"

#include "MapModel/MapPoints/MapPointListModel.h"
#include "MapModel/MapPoints/MapPointsFilterProxyModel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);


    // dm::MapModel mapModel;
    dm::MapPointListModel mapPointListModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("MapPoinstsModel", &mapPointListModel);

    // engine.rootContext()->setContextProperty("MapModel", &mapModel);
    // engine.rootContext()->setContextProperty("MapProxyModel", &pointProxyModel);
    qmlRegisterType<dm::MapPointsFilterProxyModel>("MapPointsFilterProxyModel", 1, 0, "MapPointsFilterProxyModel");

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
