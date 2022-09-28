// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "defaultpreferencedao_p.h"
#include "defaultpreference_p.h"
#include "preference.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_PREF_DEFAULTPREFERENCE_COLUMNS
#  define NS_PREF_DEFAULTPREFERENCE_COLUMNS "app, path, value"
#endif

#ifndef NS_PREF_DEFAULTPREFERENCE_COLUMNS_PREFIXED
#  define NS_PREF_DEFAULTPREFERENCE_COLUMNS_PREFIXED "default_pref.app, default_pref.path, default_pref.value"
#endif

namespace ns {
namespace Pref {

typedef QueryResult<DefaultPreference> DefaultPreferenceResult;
typedef QueryResult<DefaultPreference, QueryResultCreatorByRecord<DefaultPreference> > DefaultPreferenceResultByRecord;
typedef QueryResults<DefaultPreference> DefaultPreferenceResults;
typedef QueryResults<DefaultPreference, QueryResultCreatorByRecord<DefaultPreference> > DefaultPreferenceResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

DefaultPreferenceDaoBase::DefaultPreferenceDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("default_pref"))
{
}

DefaultPreferenceDaoBase::~DefaultPreferenceDaoBase()
{
}

bool DefaultPreferenceDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE default_pref ( "
          "app TEXT NOT NULL DEFAULT(''), "
          "path TEXT NOT NULL, "
          "value, "
          "PRIMARY KEY ( app, path ) "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool DefaultPreferenceDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE default_pref"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant DefaultPreferenceDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int DefaultPreferenceDaoBase::count(const QString& application, const QString& path)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM default_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return -1;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(application);
    query.addBindValue(path);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int DefaultPreferenceDaoBase::countByExample(const DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM default_pref WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_application.isValid())
        (sqlw += a) += QLatin1String("app=?");
    if (e->m_path.isValid())
        (sqlw += a) += QLatin1String("path=?");
    if (e->m_value.isValid())
        (sqlw += a) += QLatin1String("value=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_application.isValid())
        query.addBindValue(e->m_application);
    if (e->m_path.isValid())
        query.addBindValue(e->m_path);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int DefaultPreferenceDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM default_pref"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

DefaultPreference DefaultPreferenceDaoBase::find(const QString& application, const QString& path)
{
    static const char sql[] =
        "SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return DefaultPreference();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(application);
    query.addBindValue(path);

    return DefaultPreferenceResult(db)(query);
}

DefaultPreference DefaultPreferenceDaoBase::findByExample(const DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return DefaultPreference();

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_application.isValid())
        (sqlw += a) += QLatin1String("app=?");
    if (e->m_path.isValid())
        (sqlw += a) += QLatin1String("path=?");
    if (e->m_value.isValid())
        (sqlw += a) += QLatin1String("value=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_application.isValid())
        query.addBindValue(e->m_application);
    if (e->m_path.isValid())
        query.addBindValue(e->m_path);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);

    return DefaultPreferenceResult(db)(query);
}

DefaultPreferenceList DefaultPreferenceDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref"
        ;

    Database* db = database();
    if (!db)
        return DefaultPreferenceList();

    return DefaultPreferenceResults(db)(sqlDecorate(sql));
}

DefaultPreferenceList DefaultPreferenceDaoBase::findAllByExample(const DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return DefaultPreferenceList();

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_application.isValid())
        (sqlw += a) += QLatin1String("app=?");
    if (e->m_path.isValid())
        (sqlw += a) += QLatin1String("path=?");
    if (e->m_value.isValid())
        (sqlw += a) += QLatin1String("value=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_application.isValid())
        query.addBindValue(e->m_application);
    if (e->m_path.isValid())
        query.addBindValue(e->m_path);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);

    return DefaultPreferenceResults(db)(query);
}

bool DefaultPreferenceDaoBase::persist(DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO default_pref ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_application.isValid()) {
        (sql += c) += QLatin1String("app");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_path.isValid()) {
        (sql += c) += QLatin1String("path");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_value.isValid()) {
        (sql += c) += QLatin1String("value");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_application.isValid())
        query.addBindValue(e->m_application);
    if (e->m_path.isValid())
        query.addBindValue(e->m_path);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);

    if (db->exec(query)) {
        return true;
    } else {
        return false;
    }
}

bool DefaultPreferenceDaoBase::merge(const DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO default_pref ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_application.isValid()) {
        (sql += c) += QLatin1String("app");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_path.isValid()) {
        (sql += c) += QLatin1String("path");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_value.isValid()) {
        (sql += c) += QLatin1String("value");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_application.isValid())
        query.addBindValue(e->m_application);
    if (e->m_path.isValid())
        query.addBindValue(e->m_path);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);

    return db->exec(query);
}

bool DefaultPreferenceDaoBase::update(const DefaultPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const DefaultPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE default_pref SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_value.isValid())
        (sqlv += c) += QLatin1String("value=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE app=? AND path=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_value.isValid())
        query.addBindValue(e->m_value);
    query.addBindValue(e->m_application);
    query.addBindValue(e->m_path);

    return db->exec(query);
}

bool DefaultPreferenceDaoBase::remove(const DefaultPreference& entity)
{
    static const char sql[] =
        "DELETE FROM default_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(entity.d->m_application);
    query.addBindValue(entity.d->m_path);

    return db->exec(query);
}

bool DefaultPreferenceDaoBase::removeAll()
{
    static const char sql[] =
        "DELETE FROM default_pref"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool DefaultPreferenceDaoBase::refresh(DefaultPreference& entity)
{
    static const char sql[] =
        "SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const DefaultPreferencePrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_application);
    query.addBindValue(e->m_path);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = DefaultPreference(query);
    return true;
}

QString DefaultPreferenceDaoBase::sqlAddSort(const QString& sql) const
{
    QString ret = sql;
    ret += QLatin1String(" ORDER BY app, path");
    return ret;
}

//$BEGIN-CUSTOM$
DefaultPreferenceDao::DefaultPreferenceDao(Database* db) :
    DefaultPreferenceDaoBase(db)
{
}

DefaultPreferenceDao::~DefaultPreferenceDao()
{
}

bool DefaultPreferenceDao::createTable()
{
    return DefaultPreferenceDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_default_pref_xx ON default_pref ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!DefaultPreferenceDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool DefaultPreferenceDao::dropTable()
{
    return DefaultPreferenceDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

QList<QString> DefaultPreferenceDao::findAllPathsByApplication(const QString& application)
{
    static const char sql[] =
        "SELECT path FROM default_pref "
         "WHERE app=?"
        ;

    Database* db = database();
    if (!db)
        return QList<QString>();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);

    return QueryValues<QString>(db)(query);
}

QList<QString> DefaultPreferenceDao::findAllPathsByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "SELECT path FROM default_pref "
         "WHERE app=? AND path LIKE ?"
        ;

    Database* db = database();
    if (!db)
        return QList<QString>();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);
    query.addBindValue(prefix + QLatin1Char('%'));

    return QueryValues<QString>(db)(query);
}

DefaultPreferenceList DefaultPreferenceDao::findAllByApplication(const QString& application)
{
    static const char sql[] =
        "SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref "
         "WHERE app=?"
        ;

    Database* db = database();
    if (!db)
        return DefaultPreferenceList();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);

    return DefaultPreferenceResults(db)(query);
}

DefaultPreferenceList DefaultPreferenceDao::findAllByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "SELECT "NS_PREF_DEFAULTPREFERENCE_COLUMNS" FROM default_pref "
         "WHERE app=? AND path LIKE ?"
        ;

    Database* db = database();
    if (!db)
        return DefaultPreferenceList();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);
    query.addBindValue(prefix + QLatin1Char('%'));

    return DefaultPreferenceResults(db)(query);
}

bool DefaultPreferenceDao::removeAllByApplication(const QString& application)
{
    static const char sql[] =
        "DELETE FROM default_pref "
         "WHERE app=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(application);

    return db->exec(query);
}

bool DefaultPreferenceDao::removeAllByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "DELETE FROM default_pref "
         "WHERE app=? AND path LIKE ?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(application);
    query.addBindValue(prefix + QLatin1Char('%'));

    return db->exec(query);
}
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns
