#ifndef MAPPOINTSLISTMODEL_H_
#define MAPPOINTSLISTMODEL_H_

#include <QAbstractListModel>
#include <QSharedPointer>

namespace dm {
class MapPointsModel;
    class MapPointListModelDt;
    class MapPointListModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        explicit MapPointListModel(QObject *parent = nullptr);
        virtual ~MapPointListModel();
    public:
        void append(const QString& name);
    public:
        QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override; //好像没啥用

        int rowCount(const QModelIndex &parent = QModelIndex())                                  const override;
        int columnCount(const QModelIndex &parent = QModelIndex())                               const override;
        QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex())    const override;
        QVariant data(const QModelIndex &index, int role)                                        const override;
        QHash<int,QByteArray> roleNames()                                                        const override;

        Qt::ItemFlags flags(const QModelIndex &index)                                            const override;
        bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole)          override;
        bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex())              override;
        bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex())              override;
    private:
        QSharedPointer<MapPointListModelDt> m_Dt;
    };
} // namespace dm

#endif // MAPPOINTSLISTMODEL_H_
