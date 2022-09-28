// $Id$
/*
 * Copyright (C) 2014 Qpid Network
 * All rights reserved.
 *
 * Contact: Development Department <addison@qpidnetwork.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "callhistorydao_p.h"
#include "callhistory_p.h"
#include "calltype.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
#include <QDebug>
//$END-CUSTOM-INCLUDE$

#ifndef QPID_USER_CALLHISTORY_COLUMNS
#  define QPID_USER_CALLHISTORY_COLUMNS "id, user_name, call_name, duration, call_time, type"
#endif

#ifndef QPID_USER_CALLHISTORY_COLUMNS_PREFIXED
#  define QPID_USER_CALLHISTORY_COLUMNS_PREFIXED "call_history.id, call_history.user_name, call_history.call_name, call_history.duration, call_history.call_time, call_history.type"
#endif

namespace Qpid {
namespace User {

typedef QueryResult<CallHistory> CallHistoryResult;
typedef QueryResult<CallHistory, QueryResultCreatorByRecord<CallHistory> > CallHistoryResultByRecord;
typedef QueryResults<CallHistory> CallHistoryResults;
typedef QueryResults<CallHistory, QueryResultCreatorByRecord<CallHistory> > CallHistoryResultsByRecord;

namespace {

QPID_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
QPID_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

CallHistoryDaoBase::CallHistoryDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("call_history"))
{
}

CallHistoryDaoBase::~CallHistoryDaoBase()
{
}

bool CallHistoryDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE call_history ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "user_name TEXT NOT NULL, "
          "call_name TEXT, "
          "duration INTEGER, "
          "call_time TEXT, "
          "type INTEGER NOT NULL "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool CallHistoryDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE call_history"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant CallHistoryDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int CallHistoryDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM call_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return -1;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int CallHistoryDaoBase::countByExample(const CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM call_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_callName.isValid())
        (sqlw += a) += QLatin1String("call_name=?");
    if (e->m_duration.isValid())
        (sqlw += a) += QLatin1String("duration=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_type.isValid())
        (sqlw += a) += QLatin1String("type=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int CallHistoryDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM call_history"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

CallHistory CallHistoryDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return CallHistory();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return CallHistoryResult(db)(query);
}

CallHistory CallHistoryDaoBase::findByExample(const CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return CallHistory();

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_callName.isValid())
        (sqlw += a) += QLatin1String("call_name=?");
    if (e->m_duration.isValid())
        (sqlw += a) += QLatin1String("duration=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_type.isValid())
        (sqlw += a) += QLatin1String("type=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);

    return CallHistoryResult(db)(query);
}

CallHistoryList CallHistoryDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history"
        ;

    Database* db = database();
    if (!db)
        return CallHistoryList();

    return CallHistoryResults(db)(sqlDecorate(sql));
}

CallHistoryList CallHistoryDaoBase::findAllByExample(const CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return CallHistoryList();

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_callName.isValid())
        (sqlw += a) += QLatin1String("call_name=?");
    if (e->m_duration.isValid())
        (sqlw += a) += QLatin1String("duration=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_type.isValid())
        (sqlw += a) += QLatin1String("type=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);

    return CallHistoryResults(db)(query);
}

bool CallHistoryDaoBase::persist(CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO call_history ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callName.isValid()) {
        (sql += c) += QLatin1String("call_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_duration.isValid()) {
        (sql += c) += QLatin1String("duration");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_type.isValid()) {
        (sql += c) += QLatin1String("type");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool CallHistoryDaoBase::merge(const CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO call_history ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_id.isValid()) {
        (sql += c) += QLatin1String("id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callName.isValid()) {
        (sql += c) += QLatin1String("call_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_duration.isValid()) {
        (sql += c) += QLatin1String("duration");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_type.isValid()) {
        (sql += c) += QLatin1String("type");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);

    return db->exec(query);
}

bool CallHistoryDaoBase::update(const CallHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE call_history SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_userName.isValid())
        (sqlv += c) += QLatin1String("user_name=?");
    if (e->m_callName.isValid())
        (sqlv += c) += QLatin1String("call_name=?");
    if (e->m_duration.isValid())
        (sqlv += c) += QLatin1String("duration=?");
    if (e->m_callTime.isValid())
        (sqlv += c) += QLatin1String("call_time=?");
    if (e->m_type.isValid())
        (sqlv += c) += QLatin1String("type=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_callName.isValid())
        query.addBindValue(e->m_callName);
    if (e->m_duration.isValid())
        query.addBindValue(e->m_duration);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_type.isValid())
        query.addBindValue(e->m_type);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool CallHistoryDaoBase::remove(const CallHistory& entity)
{
    static const char sql[] =
        "DELETE FROM call_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(entity.d->m_id);

    return db->exec(query);
}

bool CallHistoryDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM call_history"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='call_history'"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (resetIdentity) {
        Transaction trans(db);
        if (!db->exec(QLatin1String(sql)))
            return false;
        if (!db->exec(QLatin1String(sqli)))
            return false;
        return trans.commit();
    } else {
        return db->exec(QLatin1String(sql));
    }
}

bool CallHistoryDaoBase::refresh(CallHistory& entity)
{
    static const char sql[] =
        "SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const CallHistoryPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = CallHistory(query);
    return true;
}

//$BEGIN-CUSTOM$
CallHistoryDao::CallHistoryDao(Database* db) :
    CallHistoryDaoBase(db)
{
}

CallHistoryDao::~CallHistoryDao()
{
}

bool CallHistoryDao::createTable()
{
    return CallHistoryDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_call_history_xx ON call_history ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!CallHistoryDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool CallHistoryDao::dropTable()
{
    return CallHistoryDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

CallHistoryList CallHistoryDao::findAllByUsername(const QString& userName)
{
	Database* db = database();
	if (!db)
		return CallHistoryList();

	QString sql(QLatin1String("SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history WHERE "));
	QString sqlw;
	SqlAnd a;
	if (!userName.isEmpty())
		(sqlw += a) += QLatin1String("user_name=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	if (!userName.isEmpty())
		query.addBindValue(userName);

	return CallHistoryResults(db)(query);
}

CallHistoryList CallHistoryDao::findAllByTypeAndDatetime(const CallHistory& entity)
{
	Database* db = database();
	if (!db)
		return CallHistoryList();

	const CallHistoryPrivate* e = entity.d;

	QString sql(QLatin1String("SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history WHERE "));
	QString sqlw;
	SqlAnd a;
	if (e->m_userName.isValid())
		(sqlw += a) += QLatin1String("user_name=?");
	if (e->m_type.isValid())
		(sqlw += a) += QLatin1String("type=?");
	if (e->m_callTime.isValid()) {
		(sqlw += a) += QLatin1String("call_time>=?");
		(sqlw += a) += QLatin1String("call_time<=?");
	}

	sql += sqlw;
	
	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);
	
	if (e->m_userName.isValid())
		query.addBindValue(e->m_userName);
	if (e->m_type.isValid())
		query.addBindValue(e->m_type);
	if (e->m_callTime.isValid()) {
		query.addBindValue(entity.callTime().toString(QLatin1String("yyyy-MM-ddT00:00:00")));
		query.addBindValue(entity.callTime().toString(QLatin1String("yyyy-MM-ddT23:59:59")));
	}

	return CallHistoryResults(db)(query);
}

CallHistory CallHistoryDao::latestCallHistory(const QString& userName)
{
	Database* db = database();
	if (!db)
		return CallHistory();

	QString sql(QLatin1String("SELECT "QPID_USER_CALLHISTORY_COLUMNS" FROM call_history WHERE "));
	QString sqlw;
	SqlAnd a;
	if (!userName.isEmpty())
		(sqlw += a) += QLatin1String("user_name=?");

	sql += sqlw;
	sql += QLatin1String("ORDER BY id DESC LIMIT 0,1");

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	if (!userName.isEmpty())
		query.addBindValue(userName);
	
	return CallHistoryResult(db)(query);
}

//$END-CUSTOM$

} // namespace Qpid::User
} // namespace Qpid
