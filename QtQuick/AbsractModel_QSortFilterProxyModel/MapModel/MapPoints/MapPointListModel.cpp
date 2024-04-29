#include "MapPointListModel.h"
#include "MapPointsModel.h"

#include<QDebug>

enum LIST_ITEM_ROLE
{
    nameRole = Qt::UserRole+1,
    indexRole,
    yawRole,
    posRole,
    isErrorRole
};

//MapPointsModelDt
namespace dm {
    class MapPointListModelDt
    {
    public:
        MapPointsModel*             model;
        QHash<int,QByteArray>       roleName;
    };
} // namespace dm

//QAbstractListModel
namespace dm {

MapPointListModel::MapPointListModel(QObject *parent): QAbstractListModel(parent)
{
    // m_Dt = QSharedPointer<MapPointListModelDt>::create(); [理论上也可以，不会造成二次释放]

    m_Dt = QSharedPointer<MapPointListModelDt>::create();
    // m_Dt = new MapPointListModelDt();
    qDebug() << "MapPointListModel crated" << Qt::endl;

    m_Dt->roleName.insert(nameRole, "name");
    m_Dt->roleName.insert(indexRole, "index");
    m_Dt->roleName.insert(yawRole, "yaw");
    m_Dt->roleName.insert(posRole, "pos");
    m_Dt->roleName.insert(isErrorRole, "isError");

    //通过单例来绑定
    // m_Dt->model = &MapPointsModel::GetInstance();

}

MapPointListModel::~MapPointListModel()
{
    qDebug() << "MapPointListModel deleted" << Qt::endl;
}

int MapPointListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_Dt->model->count();
}

int MapPointListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QModelIndex MapPointListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || column != 0)
        return QModelIndex();
    return createIndex(row, column);
}

QVariant MapPointListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_Dt->model->count())
        return QVariant();
    // qDebug() << "fresh = " << m_Dt->model->at(index.row()).name << Qt::endl;
    const MapPoint &point = m_Dt->model->at(index.row());
    switch (role) {
    case nameRole: return point.name;
    case indexRole: return point.index;
    case isErrorRole: return point.isError;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MapPointListModel::roleNames() const
{
    return m_Dt->roleName;
}

Qt::ItemFlags MapPointListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool MapPointListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        bool isChanged = false;
        MapPoint &point = (*m_Dt->model)[index.row()];
        switch (role)
        {
        case nameRole:
            if( point.name != value.toString())
            {
                point.name = value.toString();
                isChanged = true;
            }
            break;
        case indexRole:
            if( point.index != value.toInt())
            {
                point.index = value.toInt();
                isChanged = true;
            }
            break;
        default:
            return false;
        }

        //notify
        if(isChanged)
        {
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

bool MapPointListModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        m_Dt->model->insert(position, MapPoint());
    }
    endInsertRows();
    return true;
}

bool MapPointListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        m_Dt->model->removeAt(position);
    }
    endRemoveRows();
    return true;
}

QVariant MapPointListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QStringLiteral("UUID");
        case 1:
            return QStringLiteral("SECTION1");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

} // namespace dm

//MapPointListModel
namespace dm
{
    void MapPointListModel::append(const QString &name)
    {
        int index = m_Dt->model->count();
        qDebug() << "index = " << index << Qt::endl;
        insertRows(index, 1);
        setData(createIndex(index, 0), name, nameRole);
        setData(createIndex(index, 0), index % 2, indexRole);
    }


} // namespace dm
