#ifndef ABSTRACTDATABASEMODEL_P_H
#define ABSTRACTDATABASEMODEL_P_H

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include "abstractdatabasemodel.h"
#include "database.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL DatabaseModelItem
{
public:
    DatabaseModelItem(AbstractEntityDao* dao, QAbstractItemModel* model);
    ~DatabaseModelItem();

    AbstractEntityDao* dao() const;
    QAbstractItemModel* model() const;

    void load();
    void unload();

private:
    QScopedPointer<AbstractEntityDao> m_dao;
    QAbstractItemModel* m_model;
    bool m_loaded;

    Q_DISABLE_COPY(DatabaseModelItem)
};

class BUILDSYS_DB_DECL AbstractDatabaseModelPrivate
{
    Q_DECLARE_PUBLIC(AbstractDatabaseModel)

public:
    AbstractDatabaseModel* q_ptr;

    QPointer<Database> m_db;
    QString m_modelName;
    QList<DatabaseModelItem*> m_items;

    AbstractDatabaseModelPrivate();
    virtual ~AbstractDatabaseModelPrivate();

    void clear();

private:
    Q_DISABLE_COPY(AbstractDatabaseModelPrivate)
};

} // namespace ns

#endif // ABSTRACTDATABASEMODEL_P_H
