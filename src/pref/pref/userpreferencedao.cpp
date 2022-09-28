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
#include "userpreferencedao_p.h"
#include "userpreference_p.h"
#include "preference.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_PREF_USERPREFERENCE_COLUMNS
#  define NS_PREF_USERPREFERENCE_COLUMNS "app, path, value"
#endif

#ifndef NS_PREF_USERPREFERENCE_COLUMNS_PREFIXED
#  define NS_PREF_USERPREFERENCE_COLUMNS_PREFIXED "user_pref.app, user_pref.path, user_pref.value"
#endif

namespace ns {
namespace Pref {

typedef QueryResult<UserPreference> UserPreferenceResult;
typedef QueryResult<UserPreference, QueryResultCreatorByRecord<UserPreference> > UserPreferenceResultByRecord;
typedef QueryResults<UserPreference> UserPreferenceResults;
typedef QueryResults<UserPreference, QueryResultCreatorByRecord<UserPreference> > UserPreferenceResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

UserPreferenceDaoBase::UserPreferenceDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("user_pref"))
{
}

UserPreferenceDaoBase::~UserPreferenceDaoBase()
{
}

bool UserPreferenceDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE user_pref ( "
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

bool UserPreferenceDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE user_pref"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant UserPreferenceDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int UserPreferenceDaoBase::count(const QString& application, const QString& path)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM user_pref "
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

int UserPreferenceDaoBase::countByExample(const UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM user_pref WHERE "));
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

int UserPreferenceDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM user_pref"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

UserPreference UserPreferenceDaoBase::find(const QString& application, const QString& path)
{
    static const char sql[] =
        "SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return UserPreference();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(application);
    query.addBindValue(path);

    return UserPreferenceResult(db)(query);
}

UserPreference UserPreferenceDaoBase::findByExample(const UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return UserPreference();

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref WHERE "));
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

    return UserPreferenceResult(db)(query);
}

UserPreferenceList UserPreferenceDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref"
        ;

    Database* db = database();
    if (!db)
        return UserPreferenceList();

    return UserPreferenceResults(db)(sqlDecorate(sql));
}

UserPreferenceList UserPreferenceDaoBase::findAllByExample(const UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return UserPreferenceList();

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref WHERE "));
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

    return UserPreferenceResults(db)(query);
}

bool UserPreferenceDaoBase::persist(UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO user_pref ("));
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

bool UserPreferenceDaoBase::merge(const UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO user_pref ("));
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

bool UserPreferenceDaoBase::update(const UserPreference& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const UserPreferencePrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE user_pref SET "));
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

bool UserPreferenceDaoBase::remove(const UserPreference& entity)
{
    static const char sql[] =
        "DELETE FROM user_pref "
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

bool UserPreferenceDaoBase::removeAll()
{
    static const char sql[] =
        "DELETE FROM user_pref"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool UserPreferenceDaoBase::refresh(UserPreference& entity)
{
    static const char sql[] =
        "SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref "
         "WHERE app=? AND path=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const UserPreferencePrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_application);
    query.addBindValue(e->m_path);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = UserPreference(query);
    return true;
}

QString UserPreferenceDaoBase::sqlAddSort(const QString& sql) const
{
    QString ret = sql;
    ret += QLatin1String(" ORDER BY app, path");
    return ret;
}

//$BEGIN-CUSTOM$
UserPreferenceDao::UserPreferenceDao(Database* db) :
    UserPreferenceDaoBase(db)
{
}

UserPreferenceDao::~UserPreferenceDao()
{
}

bool UserPreferenceDao::createTable()
{
    return UserPreferenceDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_user_pref_xx ON user_pref ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!UserPreferenceDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool UserPreferenceDao::dropTable()
{
    return UserPreferenceDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

QList<QString> UserPreferenceDao::findAllPathsByApplication(const QString& application)
{
    static const char sql[] =
        "SELECT path FROM user_pref "
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

QList<QString> UserPreferenceDao::findAllPathsByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "SELECT path FROM user_pref "
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

UserPreferenceList UserPreferenceDao::findAllByApplication(const QString& application)
{
    static const char sql[] =
        "SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref "
         "WHERE app=?"
        ;

    Database* db = database();
    if (!db)
        return UserPreferenceList();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);

    return UserPreferenceResults(db)(query);
}

UserPreferenceList UserPreferenceDao::findAllByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "SELECT "NS_PREF_USERPREFERENCE_COLUMNS" FROM user_pref "
         "WHERE app=? AND path LIKE ?"
        ;

    Database* db = database();
    if (!db)
        return UserPreferenceList();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    query.addBindValue(application);
    query.addBindValue(prefix + QLatin1Char('%'));

    return UserPreferenceResults(db)(query);
}

bool UserPreferenceDao::removeAllByApplication(const QString& application)
{
    static const char sql[] =
        "DELETE FROM user_pref "
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

bool UserPreferenceDao::removeAllByPathPrefix(const QString& application, const QString& prefix)
{
    static const char sql[] =
        "DELETE FROM user_pref "
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
