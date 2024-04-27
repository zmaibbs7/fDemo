#ifndef MAPPOINTSFILTERPROXYMODEL_H
#define MAPPOINTSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class MapPointsFilterProxyModel : public QSortFilterProxyModel
{
public:
    MapPointsFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent) {}


    // 设置过滤类型ID
    Q_INVOKABLE void setFilterTypeID(int typeID) {
        if (filterTypeID != typeID) {
            filterTypeID = typeID;
            // 重置过滤条件，触发重新过滤
            invalidateFilter();
        }
    }

    // 获取当前的过滤类型ID
    int getFilterTypeID() const {
        return filterTypeID;
    }

protected:
    // 实现自定义过滤逻辑
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        if (filterTypeID == -1) {
            // 如果filterTypeID为-1，不进行过滤
            return true;
        }

        // 获取当前行的索引
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

        // 假设typeID存储在第0列（根据你的模型调整列号）
        int typeID = sourceModel()->data(index, Qt::UserRole+2).toInt();

        // 只接受typeID与filterTypeID相等的行
        return typeID == filterTypeID;
    }

private:
    int filterTypeID = -1;
};

#endif // MAPPOINTSFILTERPROXYMODEL_H
