#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QDateTime;
class QVariant;
class QSqlDatabase;
class QSqlError;
class QSqlQuery;
#if QT_VERSION >= 0x050000
class QUuid;
#else
struct QUuid;
#endif
QT_END_NAMESPACE

namespace ns {

class DatabasePrivate;
class ManagedDatabasePrivate;
class Version;

class BUILDSYS_DB_DECL Database : public QObject
{
    Q_OBJECT
    Q_ENUMS(DriverType ValidateResult)
    Q_PROPERTY(Version version READ version WRITE setVersion)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString location READ location WRITE setLocation)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    enum DriverType
    {
        Db2Driver,
        IbaseDriver,
        MysqlDriver,
        OdbcDriver,
        OracleDriver,
        PostgreDriver,
        SqliteDriver,
        TdsDriver
    };

    enum ValidateResult
    {
        ValidateOk,
        ValidateNew,
        ValidateCompatible,
        ValidateIncompatible,
        ValidateError
    };

    Database(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~Database();

    QSqlDatabase sqlDatabase() const;

    DriverType driverType() const;
    QString connectionName() const;

    Version version() const;
    void setVersion(const Version& version);

    QString displayName() const;
    void setDisplayName(const QString& displayName);

    QString hostName() const;
    void setHostName(const QString& hostName);

    int port() const;
    void setPort(int port);

    QString location() const;
    void setLocation(const QString& location);

    QString databaseName() const;
    void setDatabaseName(const QString& databaseName);

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    bool isOpen() const;
    bool open();
    void close();
    ValidateResult validate();

    bool isError() const;
    QString errorText() const;
    QSqlError lastError() const;

    QSqlQuery query() const;

    virtual bool exec(const QString& sql);
    virtual bool exec(QSqlQuery query);

Q_SIGNALS:
    void opened();
    void closed();
    void error(const QSqlError& e);

protected:
    Database(DatabasePrivate& dd, QObject* parent = 0);

    virtual bool openDatabase();
    virtual void closeDatabase();
    virtual ValidateResult validateDatabase();

    bool check(const QSqlDatabase& sqldb);
    bool check(const QSqlQuery& query);
    bool check(const QSqlError& e);

    virtual QString buildDatabaseName() const;
    virtual QString sqlAddLimits(const QString& sql, int limit, int offset = -1) const;

    DatabasePrivate* const d_ptr;

private:
    friend class AbstractEntityDao;
    friend class Transaction;

    Q_DECLARE_PRIVATE(Database)
    Q_DISABLE_COPY(Database)
};

class BUILDSYS_DB_DECL ManagedDatabase : public Database
{
    Q_OBJECT
    Q_PROPERTY(QUuid databaseId READ databaseId WRITE setDatabaseId)
    Q_PROPERTY(Version databaseVersion READ databaseVersion WRITE setDatabaseVersion)
    Q_PROPERTY(QString databaseCreator READ databaseCreator WRITE setDatabaseCreator)
    Q_PROPERTY(QDateTime databaseCreationTime READ databaseCreationTime WRITE setDatabaseCreationTime)

public:
    ManagedDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~ManagedDatabase();

    QUuid databaseId() const;
    bool setDatabaseId(const QUuid& databaseId);

    Version databaseVersion() const;
    bool setDatabaseVersion(const Version& databaseVersion);

    QString databaseCreator() const;
    bool setDatabaseCreator(const QString& databaseCreator);

    QDateTime databaseCreationTime() const;
    bool setDatabaseCreationTime(const QDateTime& databaseCreationTime);

    virtual QVariant databaseProperty(const QString& name) const = 0;
    virtual bool setDatabaseProperty(const QString& name, const QVariant& value) = 0;

protected:
    ManagedDatabase(ManagedDatabasePrivate& dd, QObject* parent = 0);

private:
    Q_DECLARE_PRIVATE(ManagedDatabase)
    Q_DISABLE_COPY(ManagedDatabase)
};

} // namespace ns

#endif // DATABASE_H
