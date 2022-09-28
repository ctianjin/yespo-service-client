// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_USERDATABASE_H
#define NS_USER_USERDATABASE_H

#include "sqlitedatabase.h"
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class BUILDSYS_SERVICE_DECL UserDatabaseBase : public SqliteDatabase
{
    Q_OBJECT

public:
    explicit UserDatabaseBase(QObject* parent = 0);
    explicit UserDatabaseBase(const QString& connectionName, QObject* parent = 0);
    explicit UserDatabaseBase(DriverType driverType, QObject* parent = 0);
    UserDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~UserDatabaseBase();

    static QString defaultConnectionName();

protected:
    virtual bool createDatabase();
    virtual bool createTables();

private:
    void init();

    Q_DISABLE_COPY(UserDatabaseBase)
};

//$BEGIN-CUSTOM$
class BUILDSYS_SERVICE_DECL UserDatabase : public UserDatabaseBase
{
    Q_OBJECT

public:
    explicit UserDatabase(QObject* parent = 0);
    explicit UserDatabase(const QString& connectionName, QObject* parent = 0);
    explicit UserDatabase(DriverType driverType, QObject* parent = 0);
    UserDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~UserDatabase();

protected:
    virtual bool openDatabase();
    virtual void closeDatabase();
    virtual ValidateResult validateDatabase();

    virtual bool createTables();
    virtual ValidateResult migrateNewerDatabase(const Version& dbVersion);
    virtual ValidateResult migrateOlderDatabase(const Version& dbVersion);

private:
    void init();

    Q_DISABLE_COPY(UserDatabase)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_USERDATABASE_H
