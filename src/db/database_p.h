#ifndef DATABASE_P_H
#define DATABASE_P_H

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include "database.h"
#include "version.h"
#include "dbglobal.h"

namespace ns {

class Transaction;

class BUILDSYS_DB_DECL DatabasePrivate
{
    Q_DECLARE_PUBLIC(Database)

public:
    Database* q_ptr;

    QSqlDatabase m_sqldb;
    QSqlError m_lastError;

    Database::DriverType m_driverType;
    Version m_version;
    QString m_displayName;
    QString m_location;
    QString m_databaseName;

    DatabasePrivate(Database::DriverType driverType, const QString& connectionName);
    virtual ~DatabasePrivate();

    Transaction* transaction() const;
    void setTransaction(Transaction* transaction);

    static QString toDriverName(Database::DriverType driverType);

private:
    Transaction* m_transaction;

    Q_DISABLE_COPY(DatabasePrivate)
};

class BUILDSYS_DB_DECL ManagedDatabasePrivate : public DatabasePrivate
{
    Q_DECLARE_PUBLIC(ManagedDatabase)

public:
    static const QString idProperty;
    static const QString versionProperty;
    static const QString creatorProperty;
    static const QString creationTimeProperty;

    ManagedDatabasePrivate(Database::DriverType driverType, const QString& connectionName);
    virtual ~ManagedDatabasePrivate();

private:
    Q_DISABLE_COPY(ManagedDatabasePrivate)
};

} // namespace ns

#endif // DATABASE_P_H
