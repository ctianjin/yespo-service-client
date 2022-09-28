#include <QtCore/QVariant>
#include "abstractentitymodel.h"
#include "abstractentitymodel_p.h"

namespace ns {

AbstractEntityModelPrivate::AbstractEntityModelPrivate(int totalFields) :
    q_ptr(0),
    m_editable(false),
    m_totalFields(totalFields)
{
    resetDisplayFields();
}

AbstractEntityModelPrivate::~AbstractEntityModelPrivate()
{
}

void AbstractEntityModelPrivate::resetDisplayFields()
{
    m_displayFields.clear();
    for (int i = 0; i < m_totalFields; ++i) {
        m_displayFields << i;
    }
}

AbstractEntityModel::AbstractEntityModel(int totalFields, QObject* parent) :
    QAbstractTableModel(parent),
    d_ptr(new AbstractEntityModelPrivate(totalFields))
{
    d_ptr->q_ptr = this;
}

AbstractEntityModel::AbstractEntityModel(AbstractEntityModelPrivate& dd, QObject* parent) :
    QAbstractTableModel(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractEntityModel::~AbstractEntityModel()
{
    delete d_ptr;
}

Qt::ItemFlags AbstractEntityModel::flags(const QModelIndex& index) const
{
    Q_D(const AbstractEntityModel);

    if (!index.isValid()) {
        return 0;
    }

    Qt::ItemFlags ret = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (d->m_editable) {
        ret |= Qt::ItemIsEditable;
    }
    return ret;
}

int AbstractEntityModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return count();
}

int AbstractEntityModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    Q_D(const AbstractEntityModel);
    return d->m_displayFields.size();
}

QVariant AbstractEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const AbstractEntityModel);

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section >= 0 && section < d->m_displayFields.size()) {
            return fieldName(d->m_displayFields[section]);
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant AbstractEntityModel::data(const QModelIndex& index, int role) const
{
    Q_D(const AbstractEntityModel);

    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    if (row < 0 || row >= count()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || (d->m_editable && role == Qt::EditRole)) {
        int column = index.column();
        if (column >= 0 && column < d->m_displayFields.size()) {
            return fieldValue(row, d->m_displayFields[column]);
        }
    }

    return QVariant();
}

QString AbstractEntityModel::modelName() const
{
    Q_D(const AbstractEntityModel);
    return d->m_modelName;
}

void AbstractEntityModel::setModelName(const QString& modelName)
{
    Q_D(AbstractEntityModel);
    d->m_modelName = modelName;
}

bool AbstractEntityModel::isEditable() const
{
    Q_D(const AbstractEntityModel);
    return d->m_editable;
}

void AbstractEntityModel::setEditable(bool editable)
{
    Q_D(AbstractEntityModel);
    d->m_editable = editable;
}

QList<int> AbstractEntityModel::displayFields() const
{
    Q_D(const AbstractEntityModel);
    return d->m_displayFields;
}

void AbstractEntityModel::setDisplayFields(const QList<int>& fields)
{
    Q_D(AbstractEntityModel);

    beginResetModel();

    if (fields.isEmpty()) {
        d->resetDisplayFields();
    } else {
        d->m_displayFields = fields;
    }

    endResetModel();
}

int AbstractEntityModel::indexOfDisplayField(int field) const
{
    Q_D(const AbstractEntityModel);

    int index = 0;
    foreach (int f, d->m_displayFields) {
        if (f == field) {
            return index;
        }
        ++index;
    }

    return -1;
}

} // namespace ns
