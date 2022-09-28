#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "sqlitedatabase.h"
#include "sqlitedatabase_p.h"
#include "transaction.h"
#include "stringseparator.h"
#include "version.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "SqliteDatabase:"
#endif // DEBUG_PREFIX

namespace ns {

SqliteDatabasePrivate::SqliteDatabasePrivate(const QString& connectionName) :
    ManagedDatabasePrivate(Database::SqliteDriver, connectionName)
{
    init();
}

SqliteDatabasePrivate::SqliteDatabasePrivate(Database::DriverType driverType, const QString& connectionName) :
    ManagedDatabasePrivate(driverType, connectionName)
{
    init();
}

SqliteDatabasePrivate::~SqliteDatabasePrivate()
{
}

void SqliteDatabasePrivate::init()
{
    m_busyTimeout = 0;
    m_openReadonly = false;
    m_sharedCacheEnabled = false;
    m_extensionFunctionsEnabled = false;
}

SqliteDatabase::SqliteDatabase(const QString& connectionName, QObject* parent) :
    ManagedDatabase(*new SqliteDatabasePrivate(connectionName), parent)
{
}

SqliteDatabase::SqliteDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    ManagedDatabase(*new SqliteDatabasePrivate(driverType, connectionName), parent)
{
}

SqliteDatabase::SqliteDatabase(SqliteDatabasePrivate& dd, QObject* parent) :
    ManagedDatabase(dd, parent)
{
}

SqliteDatabase::~SqliteDatabase()
{
}

int SqliteDatabase::busyTimeout() const
{
    Q_D(const SqliteDatabase);
    return d->m_busyTimeout;
}

void SqliteDatabase::setBusyTimeout(int busyTimeout)
{
    Q_D(SqliteDatabase);
    d->m_busyTimeout = busyTimeout;
}

bool SqliteDatabase::isOpenReadonly() const
{
    Q_D(const SqliteDatabase);
    return d->m_openReadonly;
}

void SqliteDatabase::setOpenReadonly(bool openReadonly)
{
    Q_D(SqliteDatabase);
    d->m_openReadonly = openReadonly;
}

bool SqliteDatabase::isSharedCacheEnabled() const
{
    Q_D(const SqliteDatabase);
    return d->m_sharedCacheEnabled;
}

void SqliteDatabase::setSharedCacheEnabled(bool sharedCacheEnabled)
{
    Q_D(SqliteDatabase);
    d->m_sharedCacheEnabled = sharedCacheEnabled;
}

bool SqliteDatabase::isExtensionFunctionsEnabled() const
{
    Q_D(const SqliteDatabase);
    return d->m_extensionFunctionsEnabled;
}

void SqliteDatabase::setExtensionFunctionsEnabled(bool extensionFunctionsEnabled)
{
    Q_D(SqliteDatabase);
    d->m_extensionFunctionsEnabled = extensionFunctionsEnabled;
}

bool SqliteDatabase::vacuum()
{
    static const char sql[] =
        "VACUUM"
        ;

    Q_D(const SqliteDatabase);

    QSqlQuery query(d->m_sqldb);

    query.exec(QLatin1String(sql));
    return check(query);
}

QVariant SqliteDatabase::databaseProperty(const QString& name) const
{
    static const char sql[] =
        "SELECT value FROM sys_prop WHERE name=?"
        ;

    Q_D(const SqliteDatabase);

    QSqlQuery query(d->m_sqldb);

    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(name);

    query.exec();
    if (const_cast<SqliteDatabase*>(this)->check(query)) {
        if (query.next())
            return query.value(0);
    }
    return QVariant();
}

bool SqliteDatabase::setDatabaseProperty(const QString& name, const QVariant& value)
{
    static const char sql[] =
        "INSERT OR REPLACE INTO sys_prop (name, value) VALUES(?, ?)"
        ;

    Q_D(const SqliteDatabase);

    QSqlQuery query(d->m_sqldb);

    query.prepare(QLatin1String(sql));
    query.addBindValue(name);
    query.addBindValue(value);

    query.exec();
    return check(query);
}

bool SqliteDatabase::openDatabase()
{
    Q_D(SqliteDatabase);

    Q_ASSERT(!d->m_sqldb.isOpen());

    // Set database connect options
    QString options;
    StringSeparator sep(";");

    if (d->m_busyTimeout > 0) {
        options += sep;
        options += QLatin1String("QSQLITE_BUSY_TIMEOUT=");
        options += QString::number(d->m_busyTimeout);
    }
    if (d->m_openReadonly) {
        options += sep;
        options += QLatin1String("QSQLITE_OPEN_READONLY");
    }
    if (d->m_sharedCacheEnabled) {
        options += sep;
        options += QLatin1String("QSQLITE_ENABLE_SHARED_CACHE");
    }
    if (d->m_extensionFunctionsEnabled) {
        options += sep;
        options += QLatin1String("QSQLITE_ENABLE_EXTENSION_FUNCTIONS");
    }

    d->m_sqldb.setConnectOptions(options);

    // Do open the database
    if (!Database::openDatabase()) {
        return false;
    }

    // Enable foreign keys
    QSqlQuery query(d->m_sqldb);
    if (!query.exec(QLatin1String("PRAGMA foreign_keys = ON;"))) {
        // Ignore the error
        qWarning() << DEBUG_PREFIX << "Failed to enable foreign keys on" << connectionName();
    }

    return true;
}

Database::ValidateResult SqliteDatabase::validateDatabase()
{
    Q_D(SqliteDatabase);

    // Check if need create new database
    // TODO Should check the data validity of the database, e.g. if it has valid id
    bool create = false;

    QDir location(d->m_location);
    if (!location.exists()) {
        location.mkpath(location.absolutePath());
    }

    QFileInfo fileInfo(buildDatabaseName());
    if (!fileInfo.exists() || fileInfo.size() == 0) {
        create = true;
    }

    // Open the database
    bool previousOpen = isOpen();
    if (!previousOpen) {
        if (!open()) {
            return ValidateError;
        }
    }

    ValidateResult result;
    if (create) {
        result = createDatabase() ? ValidateNew : ValidateError;
    } else {
        // TODO: Consider check for valid version before migration
        result = migrateDatabase();
    }

    if (!previousOpen) {
        close();
    }

    return result;
}

bool SqliteDatabase::createDatabase()
{
    Transaction trans(this);

    if (!createPropertyTable())
        return false;

    return trans.commit();
}

Database::ValidateResult SqliteDatabase::migrateDatabase()
{
    const Version currentVersion = version();
    const Version dbVersion = databaseVersion();

    if (dbVersion == currentVersion) {
        qDebug() << DEBUG_PREFIX << "Database is at current version" << currentVersion;
        return Database::ValidateOk;
    }

    Transaction trans(this);

    ValidateResult result;
    if (dbVersion > currentVersion) {
        result = migrateNewerDatabase(dbVersion);
    } else {
        result = migrateOlderDatabase(dbVersion);
    }

    if (result == ValidateCompatible) {
        setDatabaseVersion(currentVersion);
        if (!trans.commit()) {
            result = ValidateError;
        }
    }

    if (result == ValidateCompatible) {
        qDebug() << DEBUG_PREFIX << "Migrated from version" << dbVersion;
    } else if (result == ValidateIncompatible) {
        qWarning() << DEBUG_PREFIX << "Incompatible version" << dbVersion;
    } else if (result == ValidateError) {
        qCritical() << DEBUG_PREFIX << "Failed to migrate from version" << dbVersion;
    }

    return result;
}

Database::ValidateResult SqliteDatabase::migrateNewerDatabase(const Version& dbVersion)
{
    Q_UNUSED(dbVersion);
    return ValidateIncompatible;
}

Database::ValidateResult SqliteDatabase::migrateOlderDatabase(const Version& dbVersion)
{
    Q_UNUSED(dbVersion);
    return ValidateIncompatible;
}

bool SqliteDatabase::createPropertyTable()
{
    static const char sql[] =
        "CREATE TABLE sys_prop ( name TEXT PRIMARY KEY NOT NULL, value )"
        ;

    Q_ASSERT(isOpen());
    if (!exec(QLatin1String(sql))) {
        qCritical() << DEBUG_PREFIX << "Failed to create table \"sys_prop\"";
        return false;
    }

    if (!setDatabaseId(QUuid::createUuid())) {
        qWarning() << DEBUG_PREFIX << "Failed to set database id";
        return false;
    }

    if (version().isValid()) {
        if (!setDatabaseVersion(version())) {
            qWarning() << DEBUG_PREFIX << "Failed to set database version";
            return false;
        }
    }

    // TODO Grab username and save it
    if (!setDatabaseCreator(QString())) {
        qWarning() << DEBUG_PREFIX << "Failed to set database creator";
        return false;
    }

    if (!setDatabaseCreationTime(QDateTime::currentDateTime())) {
        qWarning() << DEBUG_PREFIX << "Failed to set database creationTime";
        return false;
    }

    return true;
}

bool SqliteDatabase::dropPropertyTable()
{
    static const char sql[] =
        "DROP TABLE sys_prop"
        ;

    Q_ASSERT(isOpen());
    if (!exec(QLatin1String(sql))) {
        qCritical() << DEBUG_PREFIX << "Failed to drop table \"sys_prop\"";
        return false;
    }

    return true;
}

QString SqliteDatabase::buildDatabaseName() const
{
    Q_D(const SqliteDatabase);

    QString path = d->m_databaseName;
    if (path.isEmpty()) {
        (path = d->m_sqldb.connectionName()) += QLatin1String(".dat");
    }
    return QDir(d->m_location).absoluteFilePath(path);
}

QString SqliteDatabase::sqlAddLimits(const QString& sql, int limit, int offset) const
{
    QString ret = sql;

    if (limit >= 0 || offset >= 0) {
        // XXX -1 is ok
        ret += QLatin1String(" LIMIT ");
        ret += QString::number(limit);
        if (offset >= 0) {
            ret += QLatin1String(" OFFSET ");
            ret += QString::number(offset);
        }
    }

    return ret;
}

} // namespace ns
