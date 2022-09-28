#include "abstractentitytreemodel.h"
#include "abstractentitytreemodel_p.h"

namespace ns {

EntityTreeModelItem::EntityTreeModelItem(EntityTreeModelItem* parentItem) :
    m_parentItem(parentItem)
{
}

EntityTreeModelItem::~EntityTreeModelItem()
{
    qDeleteAll(m_childItems);
}

void EntityTreeModelItem::addChild(EntityTreeModelItem* childItem)
{
    m_childItems.append(childItem);
}

void EntityTreeModelItem::removeChild(EntityTreeModelItem* childItem)
{
    if (m_childItems.removeOne(childItem)) {
        delete childItem;
    }
}

void EntityTreeModelItem::clear()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

int EntityTreeModelItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<EntityTreeModelItem*>(this));
    }
    return 0;
}

AbstractEntityTreeModelPrivate::AbstractEntityTreeModelPrivate(int totalFields) :
    q_ptr(0),
    m_editable(false),
    m_totalFields(totalFields),
    m_rootItem(new EntityTreeModelItem)
{
    resetDisplayFields();
}

AbstractEntityTreeModelPrivate::~AbstractEntityTreeModelPrivate()
{
    delete m_rootItem;
}

void AbstractEntityTreeModelPrivate::resetDisplayFields()
{
    m_displayFields.clear();
    for (int i = 0; i < m_totalFields; ++i) {
        m_displayFields << i;
    }
}

QVariant AbstractEntityTreeModelPrivate::columnValue(EntityTreeModelItem* item, int column) const
{
    Q_Q(const AbstractEntityTreeModel);

    Q_ASSERT(item);

    if (column >= 0 && column < m_displayFields.size()) {
        return q->fieldValue(item->entityVariant(), m_displayFields[column]);
    } else {
        return QVariant();
    }
}

QString AbstractEntityTreeModelPrivate::filterString(EntityTreeModelItem* item, int column) const
{
    Q_ASSERT(item);

    QString ret = columnValue(item, column).toString();
    for (int i = 0; i < item->childCount(); ++i) {
        ret += QLatin1String(" "); // FIXME Better separators?
        ret += filterString(item->child(i), column);
    }

    return ret;
}

AbstractEntityTreeModel::AbstractEntityTreeModel(int totalFields, QObject* parent) :
    QAbstractItemModel(parent),
    d_ptr(new AbstractEntityTreeModelPrivate(totalFields))
{
    d_ptr->q_ptr = this;
}

AbstractEntityTreeModel::AbstractEntityTreeModel(AbstractEntityTreeModelPrivate& dd, QObject* parent) :
    QAbstractItemModel(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractEntityTreeModel::~AbstractEntityTreeModel()
{
    delete d_ptr;
}

QModelIndex AbstractEntityTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_D(const AbstractEntityTreeModel);

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    EntityTreeModelItem* parentItem;
    if (!parent.isValid()) {
        parentItem = d->m_rootItem;
    } else {
        parentItem = static_cast<EntityTreeModelItem*>(parent.internalPointer());
    }

    EntityTreeModelItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex AbstractEntityTreeModel::parent(const QModelIndex& child) const
{
    Q_D(const AbstractEntityTreeModel);

    if (!child.isValid()) {
        return QModelIndex();
    }

    EntityTreeModelItem* childItem = static_cast<EntityTreeModelItem*>(child.internalPointer());
    EntityTreeModelItem* parentItem = childItem->parent();

    if (parentItem == d->m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

Qt::ItemFlags AbstractEntityTreeModel::flags(const QModelIndex& index) const
{
    Q_D(const AbstractEntityTreeModel);

    if (!index.isValid()) {
        return 0;
    }

    Qt::ItemFlags ret = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (d->m_editable) {
        ret |= Qt::ItemIsEditable;
    }
    return ret;
}

int AbstractEntityTreeModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const AbstractEntityTreeModel);

    if (parent.column() > 0) {
        return 0;
    }

    EntityTreeModelItem* parentItem;
    if (!parent.isValid())
        parentItem = d->m_rootItem;
    else
        parentItem = static_cast<EntityTreeModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int AbstractEntityTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    Q_D(const AbstractEntityTreeModel);
    return d->m_displayFields.size();
}

QVariant AbstractEntityTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const AbstractEntityTreeModel);

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section >= 0 && section < d->m_displayFields.size()) {
            return fieldName(d->m_displayFields[section]);
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant AbstractEntityTreeModel::data(const QModelIndex& index, int role) const
{
    Q_D(const AbstractEntityTreeModel);

    if (!index.isValid()) {
        return QVariant();
    }

    EntityTreeModelItem* item = static_cast<EntityTreeModelItem*>(index.internalPointer());

    if (role == Qt::DisplayRole || (d->m_editable && role == Qt::EditRole)) {
        return d->columnValue(item, index.column());
    } else if (role == FilterRole) {
        return d->filterString(item, index.column());
    }

    return QVariant();
}

QString AbstractEntityTreeModel::modelName() const
{
    Q_D(const AbstractEntityTreeModel);
    return d->m_modelName;
}

void AbstractEntityTreeModel::setModelName(const QString& modelName)
{
    Q_D(AbstractEntityTreeModel);
    d->m_modelName = modelName;
}

bool AbstractEntityTreeModel::isEditable() const
{
    Q_D(const AbstractEntityTreeModel);
    return d->m_editable;
}

void AbstractEntityTreeModel::setEditable(bool editable)
{
    Q_D(AbstractEntityTreeModel);
    d->m_editable = editable;
}

QList<int> AbstractEntityTreeModel::displayFields() const
{
    Q_D(const AbstractEntityTreeModel);
    return d->m_displayFields;
}

void AbstractEntityTreeModel::setDisplayFields(const QList<int>& fields)
{
    Q_D(AbstractEntityTreeModel);

    beginResetModel();

    if (fields.isEmpty()) {
        d->resetDisplayFields();
    } else {
        d->m_displayFields = fields;
    }

    endResetModel();
}

int AbstractEntityTreeModel::indexOfDisplayField(int field) const
{
    Q_D(const AbstractEntityTreeModel);

    int index = 0;
    foreach (int f, d->m_displayFields) {
        if (f == field) {
            return index;
        }
        ++index;
    }

    return -1;
}

EntityTreeModelItem* AbstractEntityTreeModel::rootItem() const
{
    Q_D(const AbstractEntityTreeModel);
    return d->m_rootItem;
}

EntityTreeModelItem* AbstractEntityTreeModel::itemFromIndex(const QModelIndex& index) const
{
    Q_D(const AbstractEntityTreeModel);

    if (!index.isValid()) {
        return d->m_rootItem;
    }

    EntityTreeModelItem* item = static_cast<EntityTreeModelItem*>(index.internalPointer());
    Q_ASSERT(item);
    return item;
}

} // namespace ns
