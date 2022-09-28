// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_PREFERENCEDATABASE_H
#define NS_PREF_PREFERENCEDATABASE_H

#include "sqlitedatabase.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Pref {

class PreferenceDatabaseBase : public SqliteDatabase
{
    Q_OBJECT

public:
    explicit PreferenceDatabaseBase(QObject* parent = 0);
    explicit PreferenceDatabaseBase(const QString& connectionName, QObject* parent = 0);
    explicit PreferenceDatabaseBase(DriverType driverType, QObject* parent = 0);
    PreferenceDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~PreferenceDatabaseBase();

    static QString defaultConnectionName();

protected:
    virtual bool createDatabase();
    virtual bool createTables();

private:
    void init();

    Q_DISABLE_COPY(PreferenceDatabaseBase)
};

//$BEGIN-CUSTOM$
class PreferenceDatabase : public PreferenceDatabaseBase
{
    Q_OBJECT

public:
    explicit PreferenceDatabase(QObject* parent = 0);
    explicit PreferenceDatabase(const QString& connectionName, QObject* parent = 0);
    explicit PreferenceDatabase(DriverType driverType, QObject* parent = 0);
    PreferenceDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~PreferenceDatabase();

protected:
    virtual bool openDatabase();
    virtual void closeDatabase();
    virtual ValidateResult validateDatabase();

    virtual bool createTables();
    virtual ValidateResult migrateNewerDatabase(const Version& dbVersion);
    virtual ValidateResult migrateOlderDatabase(const Version& dbVersion);

private:
    void init();

    Q_DISABLE_COPY(PreferenceDatabase)
};
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_PREFERENCEDATABASE_H
