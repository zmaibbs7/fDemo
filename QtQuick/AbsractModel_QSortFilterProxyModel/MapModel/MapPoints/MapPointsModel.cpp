#include "MapPointsModel.h"

#include<QDebug>

//MapPointsModelDt
namespace dm {
    class MapPointsModelDt
    {
    public:
        QList<MapPoint> data;
    };
} // namespace dm


namespace dm {

    MapPointsModel::MapPointsModel()
    {
        m_Dt = QSharedPointer<MapPointsModelDt>::create();
        qDebug() << "MapPointsModel crated" << Qt::endl;

        for (int i = 0; i < 16; ++i) {
            MapPoint p;
            p.name = "name " + QString::number(i);
            p.index = i % 4;
            append(p);
        }
    }

    MapPointsModel::~MapPointsModel()
    {
        qDebug() << "MapPointsModel deleted" << Qt::endl;
    }

    void MapPointsModel::append(const MapPoint &point)
    {
        m_Dt->data.append(point);
    }

    int MapPointsModel::count()
    {
        return m_Dt->data.count();
    }

    const MapPoint &MapPointsModel::at(int i)
    {
        return m_Dt->data.at(i);
    }

    MapPoint &MapPointsModel::operator[](int i)
    {
        return m_Dt->data[i];
    }

    void MapPointsModel::insert(int i, const MapPoint &point)
    {
        m_Dt->data.insert(i, point);
    }

    void MapPointsModel::removeAt(int i)
    {
        m_Dt->data.removeAt(i);
    }
} // namespace dm
