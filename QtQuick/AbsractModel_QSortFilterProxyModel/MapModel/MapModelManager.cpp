#include "MapModelManager.h"
#include <QQmlEngine>
#include <QDir>
#include <QFileInfoList>

namespace dm {
    class MapModelManagerDt{
    public:
        QVector<QSharedPointer<MapModel>>   mapModel;
        int                 currentFloorIndex = -1;
    };
} // namespace dm

namespace dm
{
    MapModelManager::MapModelManager(QObject *parent):QObject(parent)
    {
        qRegisterMetaType<MapModel*>("MapModel*");

        m_Dt = QSharedPointer<MapModelManagerDt>::create();

    }

    MapModelManager::~MapModelManager()
    {

    }

    QSharedPointer<MapModel> MapModelManager::mapModel(int floorIndex, bool isCurrentFloor)
    {
    // var
        QSharedPointer<MapModel> r_model;
    // 返回当前楼层
        if(isCurrentFloor) r_model = m_Dt->mapModel[m_Dt->currentFloorIndex];
    // 返回指定楼层
        else r_model = m_Dt->mapModel[floorIndex];
    // 强制QML不接管
        QQmlEngine::setObjectOwnership(r_model.get(), QQmlEngine::CppOwnership);
    // return
        return r_model;
    }

    bool MapModelManager::switchMap(int floor)
    {
        return false;
    }

    bool MapModelManager::loadMap(const QString &buildingName)
    {
    //清空历史数据
        m_Dt->mapModel.clear();
        
    //查找文件夹前缀buildingName_的大厦地图
        const QString map_res = "C:/Users/zmaib/AppData/Roaming/MapEditor/.config/map/";
        QDir dir(map_res);
        QString prefix = buildingName + "_";

        // Set filter to search only for directories and apply the prefix filter
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        dir.setNameFilters({prefix + "*"});

        // Iterate through directories that match the filter
        QFileInfoList list = dir.entryInfoList();
        for (const QFileInfo &fileInfo : list) {
            if (fileInfo.isDir()) {
                qDebug() << u8"大厦找到楼层地图 : " << fileInfo.filePath() << Qt::endl;
                const QString& mapPath = fileInfo.filePath() + "/";

                QSharedPointer<MapModel> newModel = QSharedPointer<MapModel>::create();
                m_Dt->mapModel.append(newModel);

                if(!newModel->loadMap(mapPath)) return false;
            }
        }
        return true;
    }

} // namespace dm
