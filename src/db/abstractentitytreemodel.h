#ifndef ABSTRACTENTITYTREEMODEL_H
#define ABSTRACTENTITYTREEMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include "dbglobal.h"

namespace ns {

class AbstractEntityTreeModelPrivate;

class BUILDSYS_DB_DECL EntityTreeModelItem
{
public:
    explicit EntityTreeModelItem(EntityTreeModelItem* parentItem = 0);
    ~EntityTreeModelItem();

    EntityTreeModelItem* parent() const;

    int childCount() const;
    EntityTreeModelItem* child(int row) const;

    void addChild(EntityTreeModelItem* childItem);
    void removeChild(EntityTreeModelItem* childItem);
    void clear();

    int row() const;

    QVariant entityVariant() const;
    void setEntityVariant(const QVariant& entity);

    template <typename T>
    T entity() const;

    template <typename T>
    void setEntity(const T& entity);

private:
    QVariant m_entity;
    EntityTreeModelItem* m_parentItem;
    QList<EntityTreeModelItem*> m_childItems;

    Q_DISABLE_COPY(EntityTreeModelItem)
};

inline EntityTreeModelItem* EntityTreeModelItem::parent() const
{
    return m_parentItem;
}

inline int EntityTreeModelItem::childCount() const
{
    return m_childItems.count();
}

inline EntityTreeModelItem* EntityTreeModelItem::child(int row) const
{
    return m_childItems.value(row);
}

inline QVariant EntityTreeModelItem::entityVariant() const
{
    return m_entity;
}

inline void EntityTreeModelItem::setEntityVariant(const QVariant& entity)
{
    m_entity = entity;
}

template <typename T>
inline T EntityTreeModelItem::entity() const
{
    return m_entity.value<T>();
}

template <typename T>
inline void EntityTreeModelItem::setEntity(const T& entity)
{
    m_entity.setValue(entity);
}

class BUILDSYS_DB_DECL AbstractEntityTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString modelName READ modelName WRITE setModelName)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
    Q_PROPERTY(QList<int> displayFields READ displayFields WRITE setDisplayFields)
    Q_PROPERTY(QVariant entities READ entities WRITE setEntities)

public:
    enum Roles
    {
        FilterRole = Qt::UserRole + 1
    };

    explicit AbstractEntityTreeModel(int totalFields, QObject* parent = 0);
    virtual ~AbstractEntityTreeModel();

    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    QString modelName() const;
    void setModelName(const QString& modelName);

    bool isEditable() const;
    void setEditable(bool editable);

    QList<int> displayFields() const;
    void setDisplayFields(const QList<int>& fields);

    int indexOfDisplayField(int field) const;

    virtual QVariant entities() const = 0;
    virtual void setEntities(const QVariant& entities) = 0;

protected:
    AbstractEntityTreeModel(AbstractEntityTreeModelPrivate& dd, QObject* parent = 0);

    EntityTreeModelItem* rootItem() const;
    EntityTreeModelItem* itemFromIndex(const QModelIndex& index) const;

    virtual QString fieldName(int field) const = 0;
    virtual QVariant fieldValue(const QVariant& entity, int field) const = 0;

    AbstractEntityTreeModelPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractEntityTreeModel)
    Q_DISABLE_COPY(AbstractEntityTreeModel)
};

} // namespace ns

#endif // ABSTRACTENTITYTREEMODEL_H
