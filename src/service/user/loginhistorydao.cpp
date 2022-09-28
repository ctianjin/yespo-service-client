// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "loginhistorydao_p.h"
#include "loginhistory_p.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_LOGINHISTORY_COLUMNS
#  define NS_USER_LOGINHISTORY_COLUMNS "user_name, password, remember_password, login_time"
#endif

#ifndef NS_USER_LOGINHISTORY_COLUMNS_PREFIXED
#  define NS_USER_LOGINHISTORY_COLUMNS_PREFIXED "login_history.user_name, login_history.password, login_history.remember_password, login_history.login_time"
#endif

namespace ns {
namespace User {

typedef QueryResult<LoginHistory> LoginHistoryResult;
typedef QueryResult<LoginHistory, QueryResultCreatorByRecord<LoginHistory> > LoginHistoryResultByRecord;
typedef QueryResults<LoginHistory> LoginHistoryResults;
typedef QueryResults<LoginHistory, QueryResultCreatorByRecord<LoginHistory> > LoginHistoryResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

LoginHistoryDaoBase::LoginHistoryDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("login_history"))
{
}

LoginHistoryDaoBase::~LoginHistoryDaoBase()
{
}

bool LoginHistoryDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE login_history ( "
          "user_name TEXT PRIMARY KEY NOT NULL, "
          "password TEXT, "
          "remember_password INTEGER NOT NULL DEFAULT(0), "
          "login_time INTEGER "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool LoginHistoryDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE login_history"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant LoginHistoryDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int LoginHistoryDaoBase::count(const QString& userName)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM login_history "
         "WHERE user_name=?"
        ;

    Database* db = database();
    if (!db)
        return -1;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(userName);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int LoginHistoryDaoBase::countByExample(const LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM login_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_password.isValid())
        (sqlw += a) += QLatin1String("password=?");
    if (e->m_rememberPassword.isValid())
        (sqlw += a) += QLatin1String("remember_password=?");
    if (e->m_loginTime.isValid())
        (sqlw += a) += QLatin1String("login_time=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int LoginHistoryDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM login_history"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

LoginHistory LoginHistoryDaoBase::find(const QString& userName)
{
    static const char sql[] =
        "SELECT "NS_USER_LOGINHISTORY_COLUMNS" FROM login_history "
         "WHERE user_name=?"
        ;

    Database* db = database();
    if (!db)
        return LoginHistory();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(userName);

    return LoginHistoryResult(db)(query);
}

LoginHistory LoginHistoryDaoBase::findByExample(const LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return LoginHistory();

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_LOGINHISTORY_COLUMNS" FROM login_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_password.isValid())
        (sqlw += a) += QLatin1String("password=?");
    if (e->m_rememberPassword.isValid())
        (sqlw += a) += QLatin1String("remember_password=?");
    if (e->m_loginTime.isValid())
        (sqlw += a) += QLatin1String("login_time=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);

    return LoginHistoryResult(db)(query);
}

LoginHistoryList LoginHistoryDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_LOGINHISTORY_COLUMNS" FROM login_history"
        ;

    Database* db = database();
    if (!db)
        return LoginHistoryList();

    return LoginHistoryResults(db)(sqlDecorate(sql));
}

LoginHistoryList LoginHistoryDaoBase::findAllByExample(const LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return LoginHistoryList();

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_LOGINHISTORY_COLUMNS" FROM login_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_password.isValid())
        (sqlw += a) += QLatin1String("password=?");
    if (e->m_rememberPassword.isValid())
        (sqlw += a) += QLatin1String("remember_password=?");
    if (e->m_loginTime.isValid())
        (sqlw += a) += QLatin1String("login_time=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);

    return LoginHistoryResults(db)(query);
}

bool LoginHistoryDaoBase::persist(LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO login_history ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_password.isValid()) {
        (sql += c) += QLatin1String("password");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_rememberPassword.isValid()) {
        (sql += c) += QLatin1String("remember_password");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_loginTime.isValid()) {
        (sql += c) += QLatin1String("login_time");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);

    if (db->exec(query)) {
        return true;
    } else {
        return false;
    }
}

bool LoginHistoryDaoBase::merge(const LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO login_history ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_password.isValid()) {
        (sql += c) += QLatin1String("password");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_rememberPassword.isValid()) {
        (sql += c) += QLatin1String("remember_password");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_loginTime.isValid()) {
        (sql += c) += QLatin1String("login_time");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);

    return db->exec(query);
}

bool LoginHistoryDaoBase::update(const LoginHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const LoginHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE login_history SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_password.isValid())
        (sqlv += c) += QLatin1String("password=?");
    if (e->m_rememberPassword.isValid())
        (sqlv += c) += QLatin1String("remember_password=?");
    if (e->m_loginTime.isValid())
        (sqlv += c) += QLatin1String("login_time=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE user_name=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_password.isValid())
        query.addBindValue(e->m_password);
    if (e->m_rememberPassword.isValid())
        query.addBindValue(e->m_rememberPassword);
    if (e->m_loginTime.isValid())
        query.addBindValue(e->m_loginTime);
    query.addBindValue(e->m_userName);

    return db->exec(query);
}

bool LoginHistoryDaoBase::remove(const LoginHistory& entity)
{
    static const char sql[] =
        "DELETE FROM login_history "
         "WHERE user_name=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(entity.d->m_userName);

    return db->exec(query);
}

bool LoginHistoryDaoBase::removeAll()
{
    static const char sql[] =
        "DELETE FROM login_history"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool LoginHistoryDaoBase::refresh(LoginHistory& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_LOGINHISTORY_COLUMNS" FROM login_history "
         "WHERE user_name=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const LoginHistoryPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_userName);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = LoginHistory(query);
    return true;
}

//$BEGIN-CUSTOM$
LoginHistoryDao::LoginHistoryDao(Database* db) :
    LoginHistoryDaoBase(db)
{
}

LoginHistoryDao::~LoginHistoryDao()
{
}

bool LoginHistoryDao::createTable()
{
    return LoginHistoryDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_login_history_xx ON login_history ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!LoginHistoryDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool LoginHistoryDao::dropTable()
{
    return LoginHistoryDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
