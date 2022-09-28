#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QSet>
#include <QtSql/QSqlError>
#include "databaseregistrar.h"
#include "database.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "DatabaseRegistrar:"
#endif // DEBUG_PREFIX

namespace ns {

class DatabaseRegistrarPrivate
{
    Q_DECLARE_PUBLIC(DatabaseRegistrar)

public:
    DatabaseRegistrar* q_ptr;

    QSet<Database*> m_dbs;
    QSqlError m_lastError;

    DatabaseRegistrarPrivate();

    void _q_databaseDestroyed(QObject* obj);
    void _q_error(const QSqlError& e);
};

DatabaseRegistrarPrivate::DatabaseRegistrarPrivate() :
    q_ptr(0)
{
    qRegisterMetaType<QSqlError>("QSqlError");
}

void DatabaseRegistrarPrivate::_q_databaseDestroyed(QObject* obj)
{
    Database* db = qobject_cast<Database*>(obj);
    if (db) {
        m_dbs.remove(db);
    }
}

void DatabaseRegistrarPrivate::_q_error(const QSqlError& e)
{
    Q_Q(DatabaseRegistrar);

    m_lastError = e;

    Database* db = qobject_cast<Database*>(q->sender());
    Q_EMIT q->error(db, e);
}

DatabaseRegistrar::DatabaseRegistrar(QObject* parent) :
    QObject(parent),
    d_ptr(new DatabaseRegistrarPrivate)
{
    d_ptr->q_ptr = this;
}

DatabaseRegistrar::~DatabaseRegistrar()
{
    delete d_ptr;
}

void DatabaseRegistrar::registerDatabase(Database* db)
{
    Q_D(DatabaseRegistrar);

    if (!db) {
        qDebug() << DEBUG_PREFIX << "Not registering null database";
        return;
    }

    if (!d->m_dbs.contains(db)) {
        connect(db, SIGNAL(destroyed(QObject*)), this, SLOT(_q_databaseDestroyed(QObject*)));
        connect(db, SIGNAL(error(QSqlError)), this, SLOT(_q_error(QSqlError)));
        d->m_dbs.insert(db);

        qDebug() << DEBUG_PREFIX << "Database" << db->connectionName() << "registered";
    }
}

void DatabaseRegistrar::unregisterDatabase(Database* db)
{
    Q_D(DatabaseRegistrar);

    if (!db) {
        qDebug() << DEBUG_PREFIX << "Not unregistering null database";
        return;
    }

    if (d->m_dbs.remove(db)) {
        disconnect(db, SIGNAL(destroyed(QObject*)), this, SLOT(_q_databaseDestroyed(QObject*)));
        disconnect(db, SIGNAL(error(QSqlError)), this, SLOT(_q_error(QSqlError)));
        qDebug() << DEBUG_PREFIX << "Database" << db->connectionName() << "unregistered";
    }
}

bool DatabaseRegistrar::isError() const
{
    Q_D(const DatabaseRegistrar);
    return d->m_lastError.isValid();
}

QString DatabaseRegistrar::errorText() const
{
    Q_D(const DatabaseRegistrar);
    return d->m_lastError.text();
}

QSqlError DatabaseRegistrar::lastError() const
{
    Q_D(const DatabaseRegistrar);
    return d->m_lastError;
}

} // namespace ns

#include "moc_databaseregistrar.cpp"
