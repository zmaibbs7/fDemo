#ifndef MAPMODEL_MANAGER_H_
#define MAPMODEL_MANAGER_H_

#include <QObject>
#include "../CppCommon/singleton.h"
#include "MapModel.h"


#include "MapPoints/MapPointListModel.h"
// Q_DECLARE_METATYPE(dm::MapPointListModel)

namespace dm
{
    class MapModelManager : public QObject,  public CppCommon::Singleton<MapModelManager>
    {
        Q_OBJECT
        friend CppCommon::Singleton<MapModelManager>;
    private:
        MapModelManager(QObject *parent = nullptr);
        virtual ~MapModelManager();
    public:
        //获取地图 通过
        Q_INVOKABLE QObject* mapModel(int floorIndex, bool isCurrentFloor = false);
        //楼层地图切换
        bool switchMap(int floor);
        //加载新地图[参数是列表]
        bool loadMap(const std::vector<std::string>& pathList);

    private:
        QVector<MapModel> m_mapModel;

        MapPointListModel listModel;
    };
} // namespace dm

#endif //MAPMODEL_MANAGER_H_

