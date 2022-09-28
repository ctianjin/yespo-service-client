#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include "database.h"
#include "dbglobal.h"

namespace ns {

class SqliteDatabasePrivate;

class BUILDSYS_DB_DECL SqliteDatabase : public ManagedDatabase
{
    Q_OBJECT
    Q_PROPERTY(int busyTimeout READ busyTimeout WRITE setBusyTimeout)
    Q_PROPERTY(bool openReadonly READ isOpenReadonly WRITE setOpenReadonly)
    Q_PROPERTY(bool sharedCacheEnabled READ isSharedCacheEnabled WRITE setSharedCacheEnabled)
    Q_PROPERTY(bool extensionFunctionsEnabled READ isExtensionFunctionsEnabled WRITE setExtensionFunctionsEnabled)

public:
    explicit SqliteDatabase(const QString& connectionName, QObject* parent = 0);
    SqliteDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~SqliteDatabase();

    int busyTimeout() const;
    void setBusyTimeout(int busyTimeout);

    bool isOpenReadonly() const;
    void setOpenReadonly(bool openReadonly);

    bool isSharedCacheEnabled() const;
    void setSharedCacheEnabled(bool sharedCacheEnabled);

    bool isExtensionFunctionsEnabled() const;
    void setExtensionFunctionsEnabled(bool extensionFunctionsEnabled);

    bool vacuum();

    virtual QVariant databaseProperty(const QString& name) const;
    virtual bool setDatabaseProperty(const QString& name, const QVariant& value);

protected:
    SqliteDatabase(SqliteDatabasePrivate& dd, QObject* parent = 0);

    virtual bool openDatabase();
    virtual ValidateResult validateDatabase();

    virtual bool createDatabase();
    virtual ValidateResult migrateDatabase();
    virtual ValidateResult migrateNewerDatabase(const Version& dbVersion);
    virtual ValidateResult migrateOlderDatabase(const Version& dbVersion);

    template <typename Dao>
    bool createTable();

    template <typename Dao>
    bool dropTable();

    bool createPropertyTable();
    bool dropPropertyTable();

    virtual QString buildDatabaseName() const;
    virtual QString sqlAddLimits(const QString& sql, int limit, int offset = -1) const;

    using Database::hostName;
    using Database::setHostName;
    using Database::port;
    using Database::setPort;
    using Database::userName;
    using Database::setUserName;
    using Database::password;
    using Database::setPassword;

private:
    Q_DECLARE_PRIVATE(SqliteDatabase)
    Q_DISABLE_COPY(SqliteDatabase)
};

template <typename Dao>
bool SqliteDatabase::createTable()
{
    Dao dao(this);
    return dao.createTable();
}

template <typename Dao>
bool SqliteDatabase::dropTable()
{
    Dao dao(this);
    return dao.dropTable();
}

} // namespace ns

#endif // SQLITEDATABASE_H
