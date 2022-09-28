// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "userdatabase.h"
#include "transaction.h"
#include "version.h"
// Models
#include "loginhistorydao_p.h"
#include "callrecorddao_p.h"
#include "screenshotdao_p.h"
#include "blacklistdao_p.h"
#include "contactdao_p.h"
#include "phonehistorydao_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_DEBUG_PREFIX
#  define NS_DEBUG_PREFIX "UserDatabase:"
#endif // NS_DEBUG_PREFIX

namespace ns {
namespace User {

UserDatabaseBase::UserDatabaseBase(QObject* parent) :
    SqliteDatabase(defaultConnectionName(), parent)
{
    init();
}

UserDatabaseBase::UserDatabaseBase(const QString& connectionName, QObject* parent) :
    SqliteDatabase(connectionName, parent)
{
    init();
}

UserDatabaseBase::UserDatabaseBase(DriverType driverType, QObject* parent) :
    SqliteDatabase(driverType, defaultConnectionName(), parent)
{
    init();
}

UserDatabaseBase::UserDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent) :
    SqliteDatabase(driverType, connectionName, parent)
{
    init();
}

UserDatabaseBase::~UserDatabaseBase()
{
}

void UserDatabaseBase::init()
{
    setVersion(Version(1, 0, 5));
    setDisplayName(UserDatabase::tr("User"));
    setDatabaseName(QString::fromUtf8("user.dat"));
}

QString UserDatabaseBase::defaultConnectionName()
{
    return QLatin1String("user");
}

bool UserDatabaseBase::createDatabase()
{
    QSqlQuery dbQuery = query();
    dbQuery.exec(QLatin1String("PRAGMA encoding = \"UTF-16\""));
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

bool UserDatabaseBase::createTables()
{
    if (!createTable<LoginHistoryDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"login_history\"";
        return false;
    }

    if (!createTable<CallRecordDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"call_record\"";
        return false;
    }

    if (!createTable<ScreenshotDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"screenshot\"";
        return false;
    }

    if (!createTable<BlacklistDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"blacklist\"";
        return false;
    }

    if (!createTable<ContactDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"contact\"";
        return false;
    }

    if (!createTable<PhoneHistoryDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"phone_history\"";
        return false;
    }

    return true;
}

//$BEGIN-CUSTOM$
UserDatabase::UserDatabase(QObject* parent) :
    UserDatabaseBase(parent)
{
    init();
}

UserDatabase::UserDatabase(const QString& connectionName, QObject* parent) :
    UserDatabaseBase(connectionName, parent)
{
    init();
}

UserDatabase::UserDatabase(DriverType driverType, QObject* parent) :
    UserDatabaseBase(driverType, parent)
{
    init();
}

UserDatabase::UserDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    UserDatabaseBase(driverType, connectionName, parent)
{
    init();
}

UserDatabase::~UserDatabase()
{
}

void UserDatabase::init()
{
    // Customize init here
}

bool UserDatabase::openDatabase()
{
    // Customize open here
    return UserDatabaseBase::openDatabase();
}

void UserDatabase::closeDatabase()
{
    // Customize close here
    UserDatabaseBase::closeDatabase();
}

Database::ValidateResult UserDatabase::validateDatabase()
{
    // Customize validate here
    return UserDatabaseBase::validateDatabase();
}

bool UserDatabase::createTables()
{
    // Customize createTables here
    return UserDatabaseBase::createTables();
}

Database::ValidateResult UserDatabase::migrateNewerDatabase(const Version& dbVersion)
{
    // Customize migrateNewerDatabase here
    return UserDatabaseBase::migrateNewerDatabase(dbVersion);
}

Database::ValidateResult UserDatabase::migrateOlderDatabase(const Version& dbVersion)
{
	if (dbVersion < Version(1, 0, 4)) {
		BlacklistDao dao(this);
		if (!dao.createTable())
			return ValidateError;
	}
	
	if (dbVersion < Version(1, 0, 5)) {
		Transaction trans(this);

		static const char sql1[] =
			"ALTER TABLE call_record ADD COLUMN caller_ve_login_id TEXT"
			;
		static const char sql2[] =
			"ALTER TABLE call_record ADD COLUMN callee_ve_login_id TEXT"
			;
		static const char sql3[] =
			"ALTER TABLE call_record ADD COLUMN partner_login_id TEXT"
			;

		if (!exec(QLatin1String(sql1)) || !exec(QLatin1String(sql2)) || !exec(QLatin1String(sql3))) {
			return ValidateError;
		}

		bool result = trans.commit();
		if (!result)
			return ValidateError;
	}

    return ValidateCompatible;
}
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
