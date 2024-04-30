#ifndef MAPMODEL_MANAGER_H_
#define MAPMODEL_MANAGER_H_

#include <QObject>
#include <QSharedPointer>
#include "../CppCommon/singleton.h"
#include "MapModel.h"


// #include "MapPoints/MapPointListModel.h"
namespace dm
{
    class MapModelManagerDt;
    class MapModelManager : public QObject,  public CppCommon::Singleton<MapModelManager>
    {
        Q_OBJECT
        friend CppCommon::Singleton<MapModelManager>;
    private:
        MapModelManager(QObject *parent = nullptr);
        virtual ~MapModelManager();
    public:
        //获取地图model
        Q_INVOKABLE QSharedPointer<MapModel> mapModel(int floorIndex, bool isCurrentFloor = false);
        //楼层地图切换
        bool switchMap(int floor);
        //加载新地图[参数是列表]
        Q_INVOKABLE bool loadMap(const QString& buildingName);

    private:
        // QVector<MapModel> m_mapModel;

        // MapPointListModel listModel;
        QSharedPointer<MapModelManagerDt> m_Dt;
    };
} // namespace dm

#endif //MAPMODEL_MANAGER_H_

