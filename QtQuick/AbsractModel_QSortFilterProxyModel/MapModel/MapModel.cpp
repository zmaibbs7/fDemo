#include "MapModel.h"
#include <QDebug>
#include <QFileInfoList>

#include "MapPoints/MapPointsModel.h"
// #include "MapPoints/MapPointListModel.h"
#include "MapPoints/MapPointsFilterProxyModel.h"

#include <fstream>


//MapModelDt
namespace dm
{
    class MapModelDt
    {
    public:
        MapPointsModel            pointsModel;
        // MapPointListModel         pointsListModel;
        // MapPointsFilterProxyModel pointsFilterModel;
    };
} // namespace dm

namespace dm
{
    MapModel::MapModel(QObject *parent):QObject(parent)
    {
        qDebug() << "MapModel created \n";
        m_Dt = QSharedPointer<MapModelDt>::create();


        // m_Dt->pointsListModel.bind(&m_Dt->pointsModel);
        // m_Dt->pointsFilterModel.setSourceModel(&m_Dt->pointsListModel);

        for (int i = 0; i < 50; ++i) {
            MapPoint p;
            p.name = "name " + QString::number(i);
            p.index = i % 4;
            m_Dt->pointsModel.append(p);
        }
    }

    MapModel::~MapModel()
    {
        qDebug() << "MapModel being destroyed\n";
    }

    bool MapModel::loadMap(const QString &path)
    {
        QString localMapJsonPath = path + "localMap.json";
        QFileInfo fileInfo(localMapJsonPath);
        if (!fileInfo.exists()) {
            qDebug() << "file not exist : " << localMapJsonPath << Qt::endl;
        }
        std::ifstream f(localMapJsonPath.toLocal8Bit().toStdString());
        



        return true;
    }

    MapPointsModel *MapModel::pointsModel()
    {
        return &m_Dt->pointsModel;
    }

    // QObject *MapModel::pointsListModel()
    // {
    //     return static_cast<QObject*>(&m_Dt->pointsListModel);
    // }

    // QObject *MapModel::pointsFilterModel()
    // {
    //     m_Dt->pointsFilterModel.setFilterTypeID(2);
    //     return static_cast<QObject*>(&m_Dt->pointsFilterModel);
    // }

    // void MapModel::setPointsFilterModel(QObject* model)
    // {
    //     m_Dt->pointsFilterModel.setFilterTypeID(2);
    // }

    // void MapModel::pointsSetFilterModel(int typeID)
    // {
        // m_Dt->pointsFilterModel.setFilterTypeID(typeID);
    // }
} // namespace dm
