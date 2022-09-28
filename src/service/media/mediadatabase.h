// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIADATABASE_H
#define NS_MEDIA_MEDIADATABASE_H

#include "sqlitedatabase.h"
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Media {

class BUILDSYS_SERVICE_DECL MediaDatabaseBase : public SqliteDatabase
{
    Q_OBJECT

public:
    explicit MediaDatabaseBase(QObject* parent = 0);
    explicit MediaDatabaseBase(const QString& connectionName, QObject* parent = 0);
    explicit MediaDatabaseBase(DriverType driverType, QObject* parent = 0);
    MediaDatabaseBase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~MediaDatabaseBase();

    static QString defaultConnectionName();

protected:
    virtual bool createDatabase();
    virtual bool createTables();

private:
    void init();

    Q_DISABLE_COPY(MediaDatabaseBase)
};

//$BEGIN-CUSTOM$
class BUILDSYS_SERVICE_DECL MediaDatabase : public MediaDatabaseBase
{
    Q_OBJECT

public:
    explicit MediaDatabase(QObject* parent = 0);
    explicit MediaDatabase(const QString& connectionName, QObject* parent = 0);
    explicit MediaDatabase(DriverType driverType, QObject* parent = 0);
    MediaDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~MediaDatabase();

protected:
    virtual bool openDatabase();
    virtual void closeDatabase();
    virtual ValidateResult validateDatabase();

    virtual bool createTables();
    virtual ValidateResult migrateNewerDatabase(const Version& dbVersion);
    virtual ValidateResult migrateOlderDatabase(const Version& dbVersion);

private:
    void init();

    Q_DISABLE_COPY(MediaDatabase)
};
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns

#endif // NS_MEDIA_MEDIADATABASE_H
