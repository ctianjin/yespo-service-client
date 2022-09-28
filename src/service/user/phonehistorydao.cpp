// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "phonehistorydao_p.h"
#include "phonehistory_p.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_PHONEHISTORY_COLUMNS
#  define NS_USER_PHONEHISTORY_COLUMNS "id, user_name, meet_cdr_id, cid, caller_id, deduction_time, call_time, contact_name, small_photo, large_photo, call_status, call_mode, deduction_cost, source_language, target_language, pstn_country, voicera_id, photoFile"
#endif

#ifndef NS_USER_PHONEHISTORY_COLUMNS_PREFIXED
#  define NS_USER_PHONEHISTORY_COLUMNS_PREFIXED "phone_history.id, phone_history.user_name, phone_history.meet_cdr_id, phone_history.cid, phone_history.caller_id, phone_history.deduction_time, phone_history.call_time, phone_history.contact_name, phone_history.small_photo, phone_history.large_photo, phone_history.call_status, phone_history.call_mode, phone_history.deduction_cost, phone_history.source_language, phone_history.target_language, phone_history.pstn_country, phone_history.voicera_id, phone_history.photoFile"
#endif

namespace ns {
namespace User {

typedef QueryResult<PhoneHistory> PhoneHistoryResult;
typedef QueryResult<PhoneHistory, QueryResultCreatorByRecord<PhoneHistory> > PhoneHistoryResultByRecord;
typedef QueryResults<PhoneHistory> PhoneHistoryResults;
typedef QueryResults<PhoneHistory, QueryResultCreatorByRecord<PhoneHistory> > PhoneHistoryResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

PhoneHistoryDaoBase::PhoneHistoryDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("phone_history"))
{
}

PhoneHistoryDaoBase::~PhoneHistoryDaoBase()
{
}

bool PhoneHistoryDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE phone_history ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "user_name TEXT NOT NULL, "
          "meet_cdr_id TEXT NOT NULL, "
          "cid TEXT, "
          "caller_id TEXT, "
          "deduction_time TEXT, "
          "call_time TEXT, "
          "contact_name TEXT, "
          "small_photo TEXT, "
          "large_photo TEXT, "
          "call_status TEXT, "
          "call_mode TEXT, "
          "deduction_cost TEXT, "
          "source_language TEXT, "
          "target_language TEXT, "
          "pstn_country TEXT, "
          "voicera_id TEXT, "
          "photoFile TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool PhoneHistoryDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE phone_history"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant PhoneHistoryDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int PhoneHistoryDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM phone_history "
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

int PhoneHistoryDaoBase::countByExample(const PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM phone_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_deductionTime.isValid())
        (sqlw += a) += QLatin1String("deduction_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_callStatus.isValid())
        (sqlw += a) += QLatin1String("call_status=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_deductionCost.isValid())
        (sqlw += a) += QLatin1String("deduction_cost=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_pstnCountry.isValid())
        (sqlw += a) += QLatin1String("pstn_country=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int PhoneHistoryDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM phone_history"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

PhoneHistory PhoneHistoryDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return PhoneHistory();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return PhoneHistoryResult(db)(query);
}

PhoneHistory PhoneHistoryDaoBase::findByExample(const PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return PhoneHistory();

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_deductionTime.isValid())
        (sqlw += a) += QLatin1String("deduction_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_callStatus.isValid())
        (sqlw += a) += QLatin1String("call_status=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_deductionCost.isValid())
        (sqlw += a) += QLatin1String("deduction_cost=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_pstnCountry.isValid())
        (sqlw += a) += QLatin1String("pstn_country=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);

    return PhoneHistoryResult(db)(query);
}

PhoneHistoryList PhoneHistoryDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history"
        ;

    Database* db = database();
    if (!db)
        return PhoneHistoryList();

    return PhoneHistoryResults(db)(sqlDecorate(sql));
}

PhoneHistoryList PhoneHistoryDaoBase::findAllByExample(const PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return PhoneHistoryList();

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_deductionTime.isValid())
        (sqlw += a) += QLatin1String("deduction_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_callStatus.isValid())
        (sqlw += a) += QLatin1String("call_status=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_deductionCost.isValid())
        (sqlw += a) += QLatin1String("deduction_cost=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_pstnCountry.isValid())
        (sqlw += a) += QLatin1String("pstn_country=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);

    return PhoneHistoryResults(db)(query);
}

bool PhoneHistoryDaoBase::persist(PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO phone_history ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cid.isValid()) {
        (sql += c) += QLatin1String("cid");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerId.isValid()) {
        (sql += c) += QLatin1String("caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_deductionTime.isValid()) {
        (sql += c) += QLatin1String("deduction_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_contactName.isValid()) {
        (sql += c) += QLatin1String("contact_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_largePhoto.isValid()) {
        (sql += c) += QLatin1String("large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callStatus.isValid()) {
        (sql += c) += QLatin1String("call_status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callMode.isValid()) {
        (sql += c) += QLatin1String("call_mode");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_deductionCost.isValid()) {
        (sql += c) += QLatin1String("deduction_cost");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sourceLanguage.isValid()) {
        (sql += c) += QLatin1String("source_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_targetLanguage.isValid()) {
        (sql += c) += QLatin1String("target_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_pstnCountry.isValid()) {
        (sql += c) += QLatin1String("pstn_country");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_voiceraId.isValid()) {
        (sql += c) += QLatin1String("voicera_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photofile.isValid()) {
        (sql += c) += QLatin1String("photoFile");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool PhoneHistoryDaoBase::merge(const PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO phone_history ("));
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
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cid.isValid()) {
        (sql += c) += QLatin1String("cid");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerId.isValid()) {
        (sql += c) += QLatin1String("caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_deductionTime.isValid()) {
        (sql += c) += QLatin1String("deduction_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_contactName.isValid()) {
        (sql += c) += QLatin1String("contact_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_largePhoto.isValid()) {
        (sql += c) += QLatin1String("large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callStatus.isValid()) {
        (sql += c) += QLatin1String("call_status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callMode.isValid()) {
        (sql += c) += QLatin1String("call_mode");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_deductionCost.isValid()) {
        (sql += c) += QLatin1String("deduction_cost");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sourceLanguage.isValid()) {
        (sql += c) += QLatin1String("source_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_targetLanguage.isValid()) {
        (sql += c) += QLatin1String("target_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_pstnCountry.isValid()) {
        (sql += c) += QLatin1String("pstn_country");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_voiceraId.isValid()) {
        (sql += c) += QLatin1String("voicera_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photofile.isValid()) {
        (sql += c) += QLatin1String("photoFile");
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
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);

    return db->exec(query);
}

bool PhoneHistoryDaoBase::update(const PhoneHistory& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const PhoneHistoryPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE phone_history SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_userName.isValid())
        (sqlv += c) += QLatin1String("user_name=?");
    if (e->m_meetCdrId.isValid())
        (sqlv += c) += QLatin1String("meet_cdr_id=?");
    if (e->m_cid.isValid())
        (sqlv += c) += QLatin1String("cid=?");
    if (e->m_callerId.isValid())
        (sqlv += c) += QLatin1String("caller_id=?");
    if (e->m_deductionTime.isValid())
        (sqlv += c) += QLatin1String("deduction_time=?");
    if (e->m_callTime.isValid())
        (sqlv += c) += QLatin1String("call_time=?");
    if (e->m_contactName.isValid())
        (sqlv += c) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlv += c) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlv += c) += QLatin1String("large_photo=?");
    if (e->m_callStatus.isValid())
        (sqlv += c) += QLatin1String("call_status=?");
    if (e->m_callMode.isValid())
        (sqlv += c) += QLatin1String("call_mode=?");
    if (e->m_deductionCost.isValid())
        (sqlv += c) += QLatin1String("deduction_cost=?");
    if (e->m_sourceLanguage.isValid())
        (sqlv += c) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlv += c) += QLatin1String("target_language=?");
    if (e->m_pstnCountry.isValid())
        (sqlv += c) += QLatin1String("pstn_country=?");
    if (e->m_voiceraId.isValid())
        (sqlv += c) += QLatin1String("voicera_id=?");
    if (e->m_photofile.isValid())
        (sqlv += c) += QLatin1String("photoFile=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_deductionTime.isValid())
        query.addBindValue(e->m_deductionTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_callStatus.isValid())
        query.addBindValue(e->m_callStatus);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_deductionCost.isValid())
        query.addBindValue(e->m_deductionCost);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_pstnCountry.isValid())
        query.addBindValue(e->m_pstnCountry);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool PhoneHistoryDaoBase::remove(const PhoneHistory& entity)
{
    static const char sql[] =
        "DELETE FROM phone_history "
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

bool PhoneHistoryDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM phone_history"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='phone_history'"
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

bool PhoneHistoryDaoBase::refresh(PhoneHistory& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const PhoneHistoryPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = PhoneHistory(query);
    return true;
}

//$BEGIN-CUSTOM$
PhoneHistoryDao::PhoneHistoryDao(Database* db) :
    PhoneHistoryDaoBase(db)
{
}

PhoneHistoryDao::~PhoneHistoryDao()
{
}

bool PhoneHistoryDao::createTable()
{
    return PhoneHistoryDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_phone_history_xx ON phone_history ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!PhoneHistoryDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool PhoneHistoryDao::dropTable()
{
    return PhoneHistoryDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

PhoneHistoryList PhoneHistoryDao::findAllByUserName(const QString& userName)
{
	Database* db = database();
	if (!db)
		return PhoneHistoryList();

	QString sql(QLatin1String("SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history WHERE "));
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

	return PhoneHistoryResults(db)(query);
}

bool PhoneHistoryDao::removeByUserNameAndMeetCDRID(const QString& userName, const QString& meetCDRID)
{
	static const char sql[] =
		"DELETE FROM phone_history "
		"WHERE user_name=? AND meet_cdr_id=?"
		;

	Database* db = database();
	if (!db || userName.isEmpty() || meetCDRID.isEmpty())
		return false;

	QSqlQuery query = db->query();
	query.prepare(QLatin1String(sql));
	query.addBindValue(userName);
	query.addBindValue(meetCDRID);

	return db->exec(query);
}

PhoneHistory PhoneHistoryDao::findRecentByUserName(const QString& userName)
{
	Database* db = database();
	if (!db)
		return PhoneHistory();

	QString sql(QLatin1String("SELECT "NS_USER_PHONEHISTORY_COLUMNS" FROM phone_history WHERE user_name=? ORDER BY cid DESC LIMIT 0,1"));
	QString sqlw;
	SqlAnd a;

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	query.addBindValue(userName);

	return PhoneHistoryResult(db)(query);
}

//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
