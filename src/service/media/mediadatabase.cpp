// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "mediadatabase.h"
#include "transaction.h"
#include "version.h"
// Models
#include "mediaphotodao_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_DEBUG_PREFIX
#  define NS_DEBUG_PREFIX "MediaDatabase:"
#endif // NS_DEBUG_PREFIX

namespace ns {
namespace Media {

MediaDatabaseBase::MediaDatabaseBase(QObject* parent) :
    SqliteDatabase(defaultConnectionName(), parent)
{
    init();
}

MediaDatabaseBase::MediaDatabaseBase(const QString& connectionName, QObject* parent) :
    SqliteDatabase(connectionName, parent)
{
    init();
}

MediaDatabaseBase::MediaDatabaseBase(DriverType driverType, QObject* parent) :
    SqliteDatabase(driverType, defaultConnectionName(), parent)
{
    init();
}

MediaDatabaseBase::MediaDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent) :
    SqliteDatabase(driverType, connectionName, parent)
{
    init();
}

MediaDatabaseBase::~MediaDatabaseBase()
{
}

void MediaDatabaseBase::init()
{
    setVersion(Version(1, 0, 0));
    setDisplayName(MediaDatabase::tr("Media"));
    setDatabaseName(QString::fromUtf8("media.dat"));
}

QString MediaDatabaseBase::defaultConnectionName()
{
    return QLatin1String("media");
}

bool MediaDatabaseBase::createDatabase()
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

bool MediaDatabaseBase::createTables()
{
    if (!createTable<MediaPhotoDao>()) {
        qCritical() << NS_DEBUG_PREFIX << "Failed to create table \"media_photo\"";
        return false;
    }

    return true;
}

//$BEGIN-CUSTOM$
MediaDatabase::MediaDatabase(QObject* parent) :
    MediaDatabaseBase(parent)
{
    init();
}

MediaDatabase::MediaDatabase(const QString& connectionName, QObject* parent) :
    MediaDatabaseBase(connectionName, parent)
{
    init();
}

MediaDatabase::MediaDatabase(DriverType driverType, QObject* parent) :
    MediaDatabaseBase(driverType, parent)
{
    init();
}

MediaDatabase::MediaDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    MediaDatabaseBase(driverType, connectionName, parent)
{
    init();
}

MediaDatabase::~MediaDatabase()
{
}

void MediaDatabase::init()
{
    // Customize init here
}

bool MediaDatabase::openDatabase()
{
    // Customize open here
    return MediaDatabaseBase::openDatabase();
}

void MediaDatabase::closeDatabase()
{
    // Customize close here
    MediaDatabaseBase::closeDatabase();
}

Database::ValidateResult MediaDatabase::validateDatabase()
{
    // Customize validate here
    return MediaDatabaseBase::validateDatabase();
}

bool MediaDatabase::createTables()
{
    // Customize createTables here
    return MediaDatabaseBase::createTables();
}

Database::ValidateResult MediaDatabase::migrateNewerDatabase(const Version& dbVersion)
{
    // Customize migrateNewerDatabase here
    return MediaDatabaseBase::migrateNewerDatabase(dbVersion);
}

Database::ValidateResult MediaDatabase::migrateOlderDatabase(const Version& dbVersion)
{
    // Customize migrateOlderDatabase here
    return MediaDatabaseBase::migrateOlderDatabase(dbVersion);
}
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns
