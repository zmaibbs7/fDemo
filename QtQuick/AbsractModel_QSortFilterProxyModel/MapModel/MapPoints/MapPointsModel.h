#ifndef MAPPOINTSMODEL_H_
#define MAPPOINTSMODEL_H_


#include <QList>
#include <QSharedPointer>
#include "MapPoint.h"
/*线程安全*/
namespace dm {
    class MapPointsModelDt;
    class MapPointsModel
    {
    public:
        MapPointsModel();
        ~MapPointsModel();

        //loadFromJson

        //saveToJson

    public:

        void append(const MapPoint& point);

        int count();

        const MapPoint& at(int i);

        MapPoint & operator[](int i);

        void insert(int i, const MapPoint& point);

        void removeAt(int i);


    private:
        QSharedPointer<MapPointsModelDt> m_Dt;
    };

} // namespace dm

#endif // MAPPOINTSMODEL_H_
