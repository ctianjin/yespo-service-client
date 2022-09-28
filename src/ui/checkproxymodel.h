#ifndef CHECKPROXYMODEL_H
#define CHECKPROXYMODEL_H

#include <QHash>
#include <QAbstractProxyModel>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL CheckProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    explicit CheckProxyModel(QObject* parent = 0);
    virtual ~CheckProxyModel();

    virtual void setSourceModel(QAbstractItemModel* model);

    virtual QModelIndex mapToSource(const QModelIndex& proxyIndex) const;
    virtual QModelIndex mapFromSource(const QModelIndex& sourceIndex) const;

    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    Qt::CheckState checkState(int row) const;
    bool setCheckState(int row, Qt::CheckState checkState);
    void setAllCheckState(Qt::CheckState checkState);

    void resetCheckStates();

private:
    QHash<int, Qt::CheckState> m_checkStates;

    Q_DISABLE_COPY(CheckProxyModel)
};

} // namespace ns

#endif // CHECKPROXYMODEL_H
