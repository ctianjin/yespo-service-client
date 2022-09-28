#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include "checkproxymodel.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "CheckProxyModel:"
#endif // DEBUG_PREFIX

namespace ns {

// TODO parent not supported

CheckProxyModel::CheckProxyModel(QObject* parent) :
    QAbstractProxyModel(parent)
{
}

CheckProxyModel::~CheckProxyModel()
{
}

void CheckProxyModel::setSourceModel(QAbstractItemModel* model)
{
    beginResetModel();

    QAbstractItemModel* oldModel = sourceModel();
    if (oldModel) {
        disconnect(oldModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
        disconnect(oldModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
    }

    QAbstractProxyModel::setSourceModel(model);
    QAbstractItemModel* newModel = sourceModel();
    if (newModel) {
        connect(newModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
        connect(newModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
    }

    endResetModel();
}

QModelIndex CheckProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    QAbstractItemModel* model = sourceModel();
    if (!model)
        return QModelIndex();

    if (proxyIndex.model() != this) {
        qWarning() << DEBUG_PREFIX << "Index from wrong model passed to mapToSource";
        return QModelIndex();
    }

    return model->index(proxyIndex.row(), proxyIndex.column(), mapToSource(proxyIndex.parent()));
}

QModelIndex CheckProxyModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    if (!sourceIndex.isValid())
        return QModelIndex();

    QAbstractItemModel* model = sourceModel();
    if (!model)
        return QModelIndex();

    if (sourceIndex.model() != model) {
        qWarning() << DEBUG_PREFIX << "Index from wrong model passed to mapFromSource";
        return QModelIndex();
    }

    return index(sourceIndex.row(), sourceIndex.column(), mapFromSource(sourceIndex.parent()));
}

QModelIndex CheckProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column);
}

QModelIndex CheckProxyModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

int CheckProxyModel::rowCount(const QModelIndex& parent) const
{
    QAbstractItemModel* model = sourceModel();
    if (!model)
        return 0;

    return model->rowCount(mapToSource(parent));
}

int CheckProxyModel::columnCount(const QModelIndex& parent) const
{
    QAbstractItemModel* model = sourceModel();
    if (!model)
        return 0;

    return model->columnCount(mapToSource(parent));
}

Qt::ItemFlags CheckProxyModel::flags(const QModelIndex& index) const
{
    QAbstractItemModel* model = sourceModel();
    if (!model)
        return Qt::NoItemFlags;

    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags itemFlags = model->flags(sourceIndex);
    if (sourceIndex.column() == 0) {
        itemFlags |= Qt::ItemIsUserCheckable;
    }

    return itemFlags;
}

QVariant CheckProxyModel::data(const QModelIndex& index, int role) const
{
    QAbstractItemModel* model = sourceModel();
    if (!model)
        return QVariant();

    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
        return QVariant();

    if (sourceIndex.column() == 0 && role == Qt::CheckStateRole) {
        return m_checkStates.value(sourceIndex.row());
    } else {
        return model->data(sourceIndex, role);
    }
}

bool CheckProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    QAbstractItemModel* model = sourceModel();
    if (!model)
        return false;

    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
        return false;

    if (sourceIndex.column() == 0 && role == Qt::CheckStateRole) {
        int row = sourceIndex.row();
        Qt::CheckState oldValue = m_checkStates.value(row);
        Qt::CheckState newValue = static_cast<Qt::CheckState>(value.toInt());

        if (newValue != oldValue) {
            m_checkStates.insert(row, newValue);
            Q_EMIT dataChanged(index, index);
        }
        return true;

    } else {
        return model->setData(sourceIndex, value, role);
    }
}

Qt::CheckState CheckProxyModel::checkState(int row) const
{
    const QVariant value = data(index(row, 0), Qt::CheckStateRole);
    return static_cast<Qt::CheckState>(value.toInt());
}

bool CheckProxyModel::setCheckState(int row, Qt::CheckState checkState)
{
    return setData(index(row, 0), checkState, Qt::CheckStateRole);
}

void CheckProxyModel::setAllCheckState(Qt::CheckState checkState)
{
    int rows = rowCount();
    for (int i = 0; i < rows; ++i) {
        setCheckState(i, checkState);
    }
}

void CheckProxyModel::resetCheckStates()
{
    beginResetModel();
    m_checkStates.clear();
    endResetModel();
}

} // namespace ns
