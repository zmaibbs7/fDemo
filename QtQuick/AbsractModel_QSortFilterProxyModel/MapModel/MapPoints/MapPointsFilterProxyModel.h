#ifndef MAPPOINTSFILTERPROXYMODEL_H
#define MAPPOINTSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDebug>

class MapPointsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int filterTypeID READ getFilterTypeID WRITE setFilterTypeID NOTIFY filterTypeIDChanged FINAL)

public:
    MapPointsFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent), filterTypeID(-1) {}

    int getFilterTypeID() const {
        return filterTypeID;
    }

    void setFilterTypeID(int typeID) {
        if (filterTypeID != typeID) {
            filterTypeID = typeID;
            invalidateFilter();
            Q_EMIT filterTypeIDChanged();
        }
    }

    // 添加一个条目
    Q_INVOKABLE bool addMapPoint(const QVariant& data) {
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

    // 更新条目
    Q_INVOKABLE bool updateMapPoint(int proxyRow, const QVariant& data) {
        QModelIndex proxyIndex = index(proxyRow, 0);
        QModelIndex sourceIndex = mapToSource(proxyIndex);
        return sourceModel()->setData(sourceIndex, data);
    }

    // 删除条目
    Q_INVOKABLE bool removeMapPoint(int proxyRow) {
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

signals:
    void filterTypeIDChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        if (filterTypeID == -1) {
            return true;
        }
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        int typeID = sourceModel()->data(index, Qt::UserRole+2).toInt();
        return typeID == filterTypeID;
    }

private:
    int filterTypeID = -1;
};


#endif // MAPPOINTSFILTERPROXYMODEL_H
