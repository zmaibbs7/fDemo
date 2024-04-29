#ifndef MAPMODEL_MANAGER_H_
#define MAPMODEL_MANAGER_H_

#include <QObject>
#include "../CppCommon/singleton.h"
#include "MapModel.h"
namespace dm
{
    class MapModelManager : public QObject,  public CppCommon::Singleton<MapModelManager>
    {
        friend CppCommon::Singleton<MapModelManager>;
        Q_OBJECT
        Q_PROPERTY(MapModel* map READ getMap)

    private:
        MapModelManager(QObject *parent = nullptr);
        virtual ~MapModelManager();
    public:
        //获取地图 floorIndex
        MapModel* getMap(int floorIndex, bool isCurrentFloor = false);

        //楼层地图切换
        bool switchMap(int floor);

        //加载新地图[参数是列表]
        bool loadMap(const std::vector<std::string>& pathList);

    private:
        QVector<MapModel> mapModel;
    };
} // namespace dm

#endif //MAPMODEL_MANAGER_H_