#include "MapPointsFilterProxyModel.h"

namespace dm {

    MapPointsFilterProxyModel::MapPointsFilterProxyModel(QObject *parent)
                              : QSortFilterProxyModel(parent), m_filterTypeID(-1)
    {

    }

    int MapPointsFilterProxyModel::getFilterTypeID() const
    {
        return m_filterTypeID;
    }

    void MapPointsFilterProxyModel::setFilterTypeID(int typeID)
    {
        if (m_filterTypeID != typeID) {
            m_filterTypeID = typeID;
            invalidateFilter();
            Q_EMIT filterTypeIDChanged();
        }
    }

    bool MapPointsFilterProxyModel::addMapPoint(const QVariant &data)
    {
        int row = rowCount(); // 新条目的位置（末尾）
        beginInsertRows(QModelIndex(), row, row);
        bool success = sourceModel()->insertRow(row, mapToSource(index(row, 0)));
        if (success) {
            QModelIndex newIndex = sourceModel()->index(row, 0);
            sourceModel()->setData(newIndex, data);
        }
        endInsertRows();
        return success;
    }

    bool MapPointsFilterProxyModel::updateMapPoint(int proxyRow, const QVariant &data)
    {
        QModelIndex proxyIndex = index(proxyRow, 0);
        QModelIndex sourceIndex = mapToSource(proxyIndex);
        return sourceModel()->setData(sourceIndex, data);
    }

    bool MapPointsFilterProxyModel::removeMapPoint(int proxyRow)
    {
        if (proxyRow < 0 || proxyRow >= rowCount()) {
            return false;  // 校验行号有效性
        }
        QModelIndex proxyIndex = index(proxyRow, 0);
        QModelIndex sourceIndex = mapToSource(proxyIndex);
        beginRemoveRows(QModelIndex(), proxyRow, proxyRow);
        bool success = sourceModel()->removeRow(sourceIndex.row(), sourceIndex.parent());  // 确保提供正确的父索引
        endRemoveRows();
        invalidate();  // 或 invalidateFilter();

        return success;
    }

    bool MapPointsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
    {
        if (m_filterTypeID == -1) {
            return true;
        }
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        int typeID = sourceModel()->data(index, Qt::UserRole+2).toInt();
        return typeID == m_filterTypeID;
    }

} // namespace dm
