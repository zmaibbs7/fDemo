﻿#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>

// #include "MapPoints.h"
#include "MapPoints/MapPointListModel.h"
namespace dm
{

    class MapModelDt;
    class MapModel : public QObject
    {
        Q_OBJECT
    public:
        explicit MapModel(QObject *parent = nullptr);
        virtual ~MapModel();

        // Q_PROPERTY(QObject* pointsListModel READ pointsListModel NOTIFY pointsListModelChanged)
        // Q_PROPERTY(QObject* pointsFilterModel READ pointsFilterModel NOTIFY pointsFilterModelChanged)

        // 加载LocalMap.json
        bool loadLocalMap(const std::string& path);

        //
        Q_INVOKABLE MapPointsModel* pointsModel();

        // 返回所有位置点QAbstractListModel
        // QObject* pointsListModel();

        // 返回绑定的特殊点(typeID = 4 )

        // QObject* pointsFilterModel();

    // signals:
    //     void pointsListModelChanged();
        // void pointsFilterModelChanged();
    private:
        QSharedPointer<MapModelDt> m_Dt;
    };
} // namespace dm
#endif // MAPMODEL_H
