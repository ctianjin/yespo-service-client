// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "preferencedatabase.h"
#include "transaction.h"
#include "version.h"
// Models
#include "defaultpreferencedao_p.h"
#include "userpreferencedao_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_DEBUG_PREFIX
#  define NS_DEBUG_PREFIX "PreferenceDatabase:"
#endif // NS_DEBUG_PREFIX

namespace ns {
namespace Pref {

PreferenceDatabaseBase::PreferenceDatabaseBase(QObject* parent) :
    SqliteDatabase(defaultConnectionName(), parent)
{
    init();
}

PreferenceDatabaseBase::PreferenceDatabaseBase(const QString& connectionName, QObject* parent) :
    SqliteDatabase(connectionName, parent)
{
    init();
}

PreferenceDatabaseBase::PreferenceDatabaseBase(DriverType driverType, QObject* parent) :
    SqliteDatabase(driverType, defaultConnectionName(), parent)
{
    init();
}

PreferenceDatabaseBase::PreferenceDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent) :
    SqliteDatabase(driverType, connectionName, parent)
{
    init();
}

PreferenceDatabaseBase::~PreferenceDatabaseBase()
{
}

void PreferenceDatabaseBase::init()
{
    setVersion(Version(2, 0, 0));
    setDisplayName(PreferenceDatabase::tr("Preference"));
    setDatabaseName(QString::fromUtf8("pref.dat"));
}

QString PreferenceDatabaseBase::defaultConnectionName()
{
    return QLatin1String("preference");
}

bool PreferenceDatabaseBase::createDatabase()
{
    QSqlQuery dbQuery = query();
    dbQuery.exec(QLatin1String("PRAGMA encoding = \"UTF-8\""));
    dbQuery.exec(QLatin1String("PRAGMA default_cache_size = 8000"));
    dbQuery.finish();

    Transaction trans(this);

    if (!createPropertyTable()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create new database";
        return false;
    }

    if (!createTables()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create new database";
        return false;
    }

    bool result = trans.commit();
    if (result) {
        qDebug() << NS_DEBUG_PREFIX << "New database created";
    } else {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create new database";
    }

    return result;
}

bool PreferenceDatabaseBase::createTables()
{
    if (!createTable<DefaultPreferenceDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"default_pref\"";
        return false;
    }

    if (!createTable<UserPreferenceDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"user_pref\"";
        return false;
    }

    return true;
}

//$BEGIN-CUSTOM$
PreferenceDatabase::PreferenceDatabase(QObject* parent) :
    PreferenceDatabaseBase(parent)
{
    init();
}

PreferenceDatabase::PreferenceDatabase(const QString& connectionName, QObject* parent) :
    PreferenceDatabaseBase(connectionName, parent)
{
    init();
}

PreferenceDatabase::PreferenceDatabase(DriverType driverType, QObject* parent) :
    PreferenceDatabaseBase(driverType, parent)
{
    init();
}

PreferenceDatabase::PreferenceDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    PreferenceDatabaseBase(driverType, connectionName, parent)
{
    init();
}

PreferenceDatabase::~PreferenceDatabase()
{
}

void PreferenceDatabase::init()
{
    // Customize init here
}

bool PreferenceDatabase::openDatabase()
{
    // Customize open here
    return PreferenceDatabaseBase::openDatabase();
}

void PreferenceDatabase::closeDatabase()
{
    // Customize close here
    PreferenceDatabaseBase::closeDatabase();
}

Database::ValidateResult PreferenceDatabase::validateDatabase()
{
    // Customize validate here
    return PreferenceDatabaseBase::validateDatabase();
}

bool PreferenceDatabase::createTables()
{
    // Customize createTables here
    return PreferenceDatabaseBase::createTables();
}

Database::ValidateResult PreferenceDatabase::migrateNewerDatabase(const Version& dbVersion)
{
    // Customize migrateNewerDatabase here
    return PreferenceDatabaseBase::migrateNewerDatabase(dbVersion);
}

Database::ValidateResult PreferenceDatabase::migrateOlderDatabase(const Version& dbVersion)
{
    // Customize migrateOlderDatabase here
    return PreferenceDatabaseBase::migrateOlderDatabase(dbVersion);
}
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns
