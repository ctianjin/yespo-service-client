#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include "abstractdatabasemodel.h"
#include "abstractdatabasemodel_p.h"
#include "abstractentitydao.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractDatabaseModel:"
#endif // DEBUG_PREFIX

namespace ns {

namespace {

static const char* const entitiesProperty = "entities";
static const char* const modelNameProperty = "modelName";

} // Anonymous namespace

DatabaseModelItem::DatabaseModelItem(AbstractEntityDao* dao, QAbstractItemModel* model) :
    m_dao(dao),
    m_model(model),
    m_loaded(false)
{
    Q_ASSERT(m_dao);
    Q_ASSERT(m_model);
}

DatabaseModelItem::~DatabaseModelItem()
{
    m_model->deleteLater();
}

AbstractEntityDao* DatabaseModelItem::dao() const
{
    return m_dao.data();
}

QAbstractItemModel* DatabaseModelItem::model() const
{
    return m_model;
}

void DatabaseModelItem::load()
{
    if (m_loaded)
        return;

    if (m_model->setProperty(entitiesProperty, m_dao->entities())) {
        m_loaded = true;
    } else {
        qWarning() << DEBUG_PREFIX
            << "Failed to set entities on model"
            << m_model->property(modelNameProperty).toString();
    }
}

void DatabaseModelItem::unload()
{
    m_loaded = false;
    m_model->setProperty(entitiesProperty, QVariant());
}

AbstractDatabaseModelPrivate::AbstractDatabaseModelPrivate() :
    q_ptr(0)
{
}

AbstractDatabaseModelPrivate::~AbstractDatabaseModelPrivate()
{
    qDeleteAll(m_items);
}

void AbstractDatabaseModelPrivate::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

AbstractDatabaseModel::AbstractDatabaseModel(QObject* parent) :
    QAbstractListModel(parent),
    d_ptr(new AbstractDatabaseModelPrivate)
{
    d_ptr->q_ptr = this;
}

AbstractDatabaseModel::AbstractDatabaseModel(AbstractDatabaseModelPrivate& dd, QObject* parent) :
    QAbstractListModel(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractDatabaseModel::~AbstractDatabaseModel()
{
    delete d_ptr;
}

bool AbstractDatabaseModel::isValid() const
{
    Q_D(const AbstractDatabaseModel);
    return d->m_db != 0;
}

Database* AbstractDatabaseModel::database() const
{
    Q_D(const AbstractDatabaseModel);
    return d->m_db;
}

void AbstractDatabaseModel::setDatabase(Database* db)
{
    Q_D(AbstractDatabaseModel);

    // XXX Update database even the db is the same
#if 0
    if (d->m_db == db)
        return;
#endif

    d->m_db = db;
    setModelName(db ? db->displayName() : QString());

    beginResetModel();

    d->clear();
    if (d->m_db) {
        addModels();
    }

    endResetModel();
}

QString AbstractDatabaseModel::modelName() const
{
    Q_D(const AbstractDatabaseModel);
    return d->m_modelName;
}

void AbstractDatabaseModel::setModelName(const QString& modelName)
{
    Q_D(AbstractDatabaseModel);
    d->m_modelName = modelName;
}

void AbstractDatabaseModel::addModel(AbstractEntityDao* dao, QAbstractItemModel* model)
{
    Q_D(AbstractDatabaseModel);

    d->m_items << new DatabaseModelItem(dao, model);
}

int AbstractDatabaseModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const AbstractDatabaseModel);

    Q_UNUSED(parent);
    return d->m_items.size();
}

QVariant AbstractDatabaseModel::data(const QModelIndex& index, int role) const
{
    Q_D(const AbstractDatabaseModel);

    if (!index.isValid())
        return QVariant();

    DatabaseModelItem* item = d->m_items.value(index.row());
    if (!item) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            return item->model()->property(modelNameProperty);

        case EntityModelRole:
            return QVariant::fromValue(static_cast<QObject*>(item->model()));

        default:
            return QVariant();
    }
}

void AbstractDatabaseModel::refresh()
{
    Q_D(AbstractDatabaseModel);

    beginResetModel();

    foreach (DatabaseModelItem* item, d->m_items) {
        item->unload();
    }

    endResetModel();
}

void AbstractDatabaseModel::refresh(const QModelIndex& index)
{
    Q_D(AbstractDatabaseModel);

    if (!index.isValid())
        return;

    DatabaseModelItem* item = d->m_items.value(index.row());
    if (!item)
        return;

    item->load();

    Q_EMIT dataChanged(index, index);
}

} // namespace ns
