#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "database.h"
#include "database_p.h"
#include "signalblocker.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Database:"
#endif // DEBUG_PREFIX

namespace ns {

const QString ManagedDatabasePrivate::idProperty = QLatin1String("db.id");
const QString ManagedDatabasePrivate::versionProperty = QLatin1String("db.version");
const QString ManagedDatabasePrivate::creatorProperty = QLatin1String("db.creator");
const QString ManagedDatabasePrivate::creationTimeProperty = QLatin1String("db.creation_time");

DatabasePrivate::DatabasePrivate(Database::DriverType driverType, const QString& connectionName) :
    q_ptr(0),
    m_driverType(driverType),
    m_transaction(0)
{
    const QString driverName = toDriverName(m_driverType);
    m_sqldb = QSqlDatabase::addDatabase(driverName, connectionName);
    if (m_sqldb.isValid()) {
        qDebug() << DEBUG_PREFIX << "Database added (" << driverName << connectionName << ")";
    } else {
        qWarning() << DEBUG_PREFIX << "Database failed to add (" << driverName << connectionName << ")";
    }
}

DatabasePrivate::~DatabasePrivate()
{
    if (m_sqldb.isValid()) {
        const QString connectionName = m_sqldb.connectionName();
        m_sqldb = QSqlDatabase();

        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << DEBUG_PREFIX << "Database removed (" << connectionName << ")";
    }
}

QString DatabasePrivate::toDriverName(Database::DriverType driverType)
{
    QString ret;
    switch (driverType) {
        case Database::Db2Driver:
            ret = QLatin1String("QDB2");
            break;
        case Database::IbaseDriver:
            ret = QLatin1String("QIBASE");
            break;
        case Database::MysqlDriver:
            ret = QLatin1String("QMYSQL");
            break;
        case Database::OdbcDriver:
            ret = QLatin1String("QODBC");
            break;
        case Database::OracleDriver:
            ret = QLatin1String("QOCI");
            break;
        case Database::PostgreDriver:
            ret = QLatin1String("QPSQL");
            break;
        case Database::SqliteDriver:
            ret = QLatin1String("QSQLITE");
            break;
        case Database::TdsDriver:
            ret = QLatin1String("QTDS");
            break;
        default:
            Q_ASSERT_X(false, "Database::toDriverName", "Bad driver type");
            break;
    }
    return ret;
}

Transaction* DatabasePrivate::transaction() const
{
    return m_transaction;
}

void DatabasePrivate::setTransaction(Transaction* transaction)
{
    m_transaction = transaction;
}

Database::Database(DriverType driverType, const QString& connectionName, QObject* parent) :
    QObject(parent),
    d_ptr(new DatabasePrivate(driverType, connectionName))
{
    d_ptr->q_ptr = this;
}

Database::Database(DatabasePrivate& dd, QObject* parent) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

Database::~Database()
{
    delete d_ptr;
}

QSqlDatabase Database::sqlDatabase() const
{
    Q_D(const Database);
    return d->m_sqldb;
}

Database::DriverType Database::driverType() const
{
    Q_D(const Database);
    return d->m_driverType;
}

QString Database::connectionName() const
{
    Q_D(const Database);
    return d->m_sqldb.connectionName();
}

Version Database::version() const
{
    Q_D(const Database);
    return d->m_version;
}

void Database::setVersion(const Version& version)
{
    Q_D(Database);
    d->m_version = version;
}

QString Database::displayName() const
{
    Q_D(const Database);
    return d->m_displayName;
}

void Database::setDisplayName(const QString& displayName)
{
    Q_D(Database);
    d->m_displayName = displayName;
}

QString Database::hostName() const
{
    Q_D(const Database);
    return d->m_sqldb.hostName();
}

void Database::setHostName(const QString& hostName)
{
    Q_D(Database);
    d->m_sqldb.setHostName(hostName);
}

int Database::port() const
{
    Q_D(const Database);
    return d->m_sqldb.port();
}

void Database::setPort(int port)
{
    Q_D(Database);
    d->m_sqldb.setPort(port);
}

QString Database::location() const
{
    Q_D(const Database);
    return d->m_location;
}

void Database::setLocation(const QString& location)
{
    Q_D(Database);
    d->m_location = location;
}

QString Database::databaseName() const
{
    Q_D(const Database);
    return d->m_databaseName;
}

void Database::setDatabaseName(const QString& databaseName)
{
    Q_D(Database);
    d->m_databaseName = databaseName;
}

QString Database::userName() const
{
    Q_D(const Database);
    return d->m_sqldb.userName();
}

void Database::setUserName(const QString& userName)
{
    Q_D(Database);
    d->m_sqldb.setUserName(userName);
}

QString Database::password() const
{
    Q_D(const Database);
    return d->m_sqldb.password();
}

void Database::setPassword(const QString& password)
{
    Q_D(Database);
    d->m_sqldb.setPassword(password);
}

bool Database::isOpen() const
{
    Q_D(const Database);
    return d->m_sqldb.isOpen();
}

bool Database::open()
{
    Q_D(Database);

    if (isOpen()) {
        qDebug() << DEBUG_PREFIX << "Database" << connectionName() << "is already open";
        return true;
    }

    if (!openDatabase()) {
        check(d->m_sqldb);
        qCritical() << DEBUG_PREFIX << "Failed to open database" << connectionName();

        // Close the sql database if opened
        if (d->m_sqldb.isOpen()) {
            d->m_sqldb.close();
        }

        return false;
    }

    qDebug() << DEBUG_PREFIX << "Database" << connectionName() << "opened";
    Q_EMIT opened();

    return true;
}

void Database::close()
{
    if (!isOpen()) {
        qDebug() << DEBUG_PREFIX << "Database" << connectionName() << "is not open";
        return;
    }

    closeDatabase();

    qDebug() << DEBUG_PREFIX << "Database" << connectionName() << "closed";
    Q_EMIT closed();
}

Database::ValidateResult Database::validate()
{
    // Signals will be blocked during validate
    SignalBlocker blocker(this);

    return validateDatabase();
}

bool Database::isError() const
{
    Q_D(const Database);
    return d->m_lastError.isValid();
}

QString Database::errorText() const
{
    Q_D(const Database);
    return d->m_lastError.text();
}

QSqlError Database::lastError() const
{
    Q_D(const Database);
    return d->m_lastError;
}

QSqlQuery Database::query() const
{
    Q_D(const Database);
    return QSqlQuery(d->m_sqldb);
}

bool Database::exec(const QString& sql)
{
    Q_D(const Database);

    QSqlQuery query(d->m_sqldb);
    bool ret = query.exec(sql);
#if defined(BUILDSYS_DB_LOG_SQL) && !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << DEBUG_PREFIX << query.executedQuery();
#endif
    check(query);
    return ret;
}

bool Database::exec(QSqlQuery query)
{
    bool ret = query.exec();
#if defined(BUILDSYS_DB_LOG_SQL) && !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << DEBUG_PREFIX << query.executedQuery();
#endif
    check(query);
    return ret;
}

bool Database::openDatabase()
{
    Q_D(Database);

    Q_ASSERT(!d->m_sqldb.isOpen());

    d->m_sqldb.setDatabaseName(buildDatabaseName());
    return d->m_sqldb.open();
}

void Database::closeDatabase()
{
    Q_D(Database);

    Q_ASSERT(d->m_sqldb.isOpen());
    d->m_sqldb.close();
}

Database::ValidateResult Database::validateDatabase()
{
    return ValidateOk;
}

bool Database::check(const QSqlDatabase& sqldb)
{
    return check(sqldb.lastError());
}

bool Database::check(const QSqlQuery& query)
{
    return check(query.lastError());
}

bool Database::check(const QSqlError& e)
{
    Q_D(Database);

    d->m_lastError = e;
    if (!d->m_lastError.isValid()) {
        return true;
    }

    Q_EMIT error(d->m_lastError);
    return false;
}

QString Database::buildDatabaseName() const
{
    Q_D(const Database);
    return d->m_databaseName;
}

QString Database::sqlAddLimits(const QString& sql, int limit, int offset) const
{
    Q_UNUSED(limit);
    Q_UNUSED(offset);

    qWarning() << DEBUG_PREFIX << "Limit/offset is not supported";
    return sql;
}

ManagedDatabasePrivate::ManagedDatabasePrivate(Database::DriverType driverType, const QString& connectionName) :
    DatabasePrivate(driverType, connectionName)
{
}

ManagedDatabasePrivate::~ManagedDatabasePrivate()
{
}

ManagedDatabase::ManagedDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    Database(*new ManagedDatabasePrivate(driverType, connectionName), parent)
{
}

ManagedDatabase::ManagedDatabase(ManagedDatabasePrivate& dd, QObject* parent) :
    Database(dd, parent)
{
}

ManagedDatabase::~ManagedDatabase()
{
}

QUuid ManagedDatabase::databaseId() const
{
    Q_D(const ManagedDatabase);
    return QUuid(databaseProperty(d->idProperty).toString());
}

bool ManagedDatabase::setDatabaseId(const QUuid& databaseId)
{
    Q_D(const ManagedDatabase);
    return setDatabaseProperty(d->idProperty, databaseId.toString());
}

Version ManagedDatabase::databaseVersion() const
{
    Q_D(const ManagedDatabase);
    return Version(databaseProperty(d->versionProperty).toString());
}

bool ManagedDatabase::setDatabaseVersion(const Version& databaseVersion)
{
    Q_D(const ManagedDatabase);
    return setDatabaseProperty(d->versionProperty, databaseVersion.toString());
}

QString ManagedDatabase::databaseCreator() const
{
    Q_D(const ManagedDatabase);
    return databaseProperty(d->creatorProperty).toString();
}

bool ManagedDatabase::setDatabaseCreator(const QString& databaseCreator)
{
    Q_D(const ManagedDatabase);
    return setDatabaseProperty(d->creatorProperty, databaseCreator);
}

QDateTime ManagedDatabase::databaseCreationTime() const
{
    Q_D(const ManagedDatabase);
    return databaseProperty(d->creationTimeProperty).toDateTime();
}

bool ManagedDatabase::setDatabaseCreationTime(const QDateTime& databaseCreationTime)
{
    Q_D(const ManagedDatabase);
    return setDatabaseProperty(d->creationTimeProperty, databaseCreationTime);
}

} // namespace ns

#include "moc_database.cpp"
