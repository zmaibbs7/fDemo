#ifndef MAPPOINTSFILTERPROXYMODEL_H
#define MAPPOINTSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace dm {

/**
 * @brief MapPointsFilterProxyModel 类提供了一个专门用于管理地图点的过滤代理模型。
 *
 * 它支持通过其方法添加、更新和删除地图点。此外，它还允许根据通过属性设置的类型标识符进行过滤。
 */
class MapPointsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int filterTypeID READ getFilterTypeID WRITE setFilterTypeID NOTIFY filterTypeIDChanged FINAL)

public:
    /**
     * @brief MapPointsFilterProxyModel 的构造函数。
     * @param parent 该模型的父对象，通常如果不需要则为 nullptr。
     */
    explicit MapPointsFilterProxyModel(QObject *parent = nullptr);

    /**
     * @brief 获取当前的过滤类型 ID。
     * @return 用于过滤地图点的过滤类型 ID。
     */
    int getFilterTypeID() const;

    /**
     * @brief 设置用于决定显示哪些地图点的过滤类型 ID。
     * @param typeID 过滤类型 ID。
     */
    void setFilterTypeID(int typeID);

    /**
     * @brief 向模型中添加一个地图点。
     * @param data 要添加的地图点的数据。
     * @return 如果地图点成功添加，则返回 true；否则返回 false。
     */
    Q_INVOKABLE bool addMapPoint(const QVariant& data);

    /**
     * @brief 在模型中更新现有的地图点。
     * @param proxyRow 代理模型中地图点存在的行索引。
     * @param data 地图点的新数据。
     * @return 如果地图点成功更新，则返回 true；否则返回 false。
     */
    Q_INVOKABLE bool updateMapPoint(int proxyRow, const QVariant& data);

    /**
     * @brief 从模型中删除一个地图点。
     * @param proxyRow 代理模型中要删除的地图点的行索引。
     * @return 如果地图点成功删除，则返回 true；否则返回 false。
     */
    Q_INVOKABLE bool removeMapPoint(int proxyRow);

signals:
    /**
     * @brief 当过滤类型 ID 发生变化时发出的信号。
     */
    void filterTypeIDChanged();

protected:
    /**
     * @brief 决定是否将行包含在模型中。
     * @param sourceRow 源模型中的行索引。
     * @param sourceParent 源模型中行的父索引。
     * @return 如果行应该包含在内，则返回 true；否则返回 false。
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    int m_filterTypeID = -1;  ///< 存储当前的过滤类型 ID。
};

} // namespace dm

#endif // MAPPOINTSFILTERPROXYMODEL_H
