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
#include "callrecorddao_p.h"
#include "callrecord_p.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
#include <QDebug>
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_CALLRECORD_COLUMNS
#  define NS_USER_CALLRECORD_COLUMNS "id, owner, cdr_id, meet_cdr_id, small_photo, display_name, start_call_time, call_time, is_answered, call_mode, cdr_type, caller_ve_sys_id, caller_id, caller_ve_login_id, caller_display_name, caller_mobile, caller_small_photo, caller_large_photo, is_pstn_call, call_country, callee_ve_sys_id, callee_id, callee_ve_login_id, callee_display_name, callee_mobile, callee_small_photo, callee_large_photo, partner_sys_id, partner_id, partner_login_id, partner_display_name, partner_small_photo, partner_large_photo, is_free, partner_level, language_1, language_2, is_success"
#endif

#ifndef NS_USER_CALLRECORD_COLUMNS_PREFIXED
#  define NS_USER_CALLRECORD_COLUMNS_PREFIXED "call_record.id, call_record.owner, call_record.cdr_id, call_record.meet_cdr_id, call_record.small_photo, call_record.display_name, call_record.start_call_time, call_record.call_time, call_record.is_answered, call_record.call_mode, call_record.cdr_type, call_record.caller_ve_sys_id, call_record.caller_id, call_record.caller_ve_login_id, call_record.caller_display_name, call_record.caller_mobile, call_record.caller_small_photo, call_record.caller_large_photo, call_record.is_pstn_call, call_record.call_country, call_record.callee_ve_sys_id, call_record.callee_id, call_record.callee_ve_login_id, call_record.callee_display_name, call_record.callee_mobile, call_record.callee_small_photo, call_record.callee_large_photo, call_record.partner_sys_id, call_record.partner_id, call_record.partner_login_id, call_record.partner_display_name, call_record.partner_small_photo, call_record.partner_large_photo, call_record.is_free, call_record.partner_level, call_record.language_1, call_record.language_2, call_record.is_success"
#endif

namespace ns {
namespace User {

typedef QueryResult<CallRecord> CallRecordResult;
typedef QueryResult<CallRecord, QueryResultCreatorByRecord<CallRecord> > CallRecordResultByRecord;
typedef QueryResults<CallRecord> CallRecordResults;
typedef QueryResults<CallRecord, QueryResultCreatorByRecord<CallRecord> > CallRecordResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

CallRecordDaoBase::CallRecordDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("call_record"))
{
}

CallRecordDaoBase::~CallRecordDaoBase()
{
}

bool CallRecordDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE call_record ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "owner TEXT, "
          "cdr_id TEXT, "
          "meet_cdr_id TEXT, "
          "small_photo TEXT, "
          "display_name TEXT, "
          "start_call_time TEXT, "
          "call_time TEXT, "
          "is_answered TEXT, "
          "call_mode TEXT, "
          "cdr_type TEXT, "
          "caller_ve_sys_id TEXT, "
          "caller_id TEXT, "
          "caller_ve_login_id TEXT, "
          "caller_display_name TEXT, "
          "caller_mobile TEXT, "
          "caller_small_photo TEXT, "
          "caller_large_photo TEXT, "
          "is_pstn_call TEXT, "
          "call_country TEXT, "
          "callee_ve_sys_id TEXT, "
          "callee_id TEXT, "
          "callee_ve_login_id TEXT, "
          "callee_display_name TEXT, "
          "callee_mobile TEXT, "
          "callee_small_photo TEXT, "
          "callee_large_photo TEXT, "
          "partner_sys_id TEXT, "
          "partner_id TEXT, "
          "partner_login_id TEXT, "
          "partner_display_name TEXT, "
          "partner_small_photo TEXT, "
          "partner_large_photo TEXT, "
          "is_free TEXT, "
          "partner_level TEXT, "
          "language_1 TEXT, "
          "language_2 TEXT, "
          "is_success TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool CallRecordDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE call_record"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant CallRecordDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int CallRecordDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM call_record "
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

int CallRecordDaoBase::countByExample(const CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM call_record WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_cdrId.isValid())
        (sqlw += a) += QLatin1String("cdr_id=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_startCallTime.isValid())
        (sqlw += a) += QLatin1String("start_call_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_isAnswered.isValid())
        (sqlw += a) += QLatin1String("is_answered=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_cdrType.isValid())
        (sqlw += a) += QLatin1String("cdr_type=?");
    if (e->m_callerVeSysId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_sys_id=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_callerVeLoginId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_login_id=?");
    if (e->m_callerDisplayName.isValid())
        (sqlw += a) += QLatin1String("caller_display_name=?");
    if (e->m_callerMobile.isValid())
        (sqlw += a) += QLatin1String("caller_mobile=?");
    if (e->m_callerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("caller_small_photo=?");
    if (e->m_callerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("caller_large_photo=?");
    if (e->m_isPstnCall.isValid())
        (sqlw += a) += QLatin1String("is_pstn_call=?");
    if (e->m_callCountry.isValid())
        (sqlw += a) += QLatin1String("call_country=?");
    if (e->m_calleeVeSysId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_sys_id=?");
    if (e->m_calleeId.isValid())
        (sqlw += a) += QLatin1String("callee_id=?");
    if (e->m_calleeVeLoginId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_login_id=?");
    if (e->m_calleeDisplayName.isValid())
        (sqlw += a) += QLatin1String("callee_display_name=?");
    if (e->m_calleeMobile.isValid())
        (sqlw += a) += QLatin1String("callee_mobile=?");
    if (e->m_calleeSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("callee_small_photo=?");
    if (e->m_calleeLargePhoto.isValid())
        (sqlw += a) += QLatin1String("callee_large_photo=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_partnerId.isValid())
        (sqlw += a) += QLatin1String("partner_id=?");
    if (e->m_partnerLoginId.isValid())
        (sqlw += a) += QLatin1String("partner_login_id=?");
    if (e->m_partnerDisplayName.isValid())
        (sqlw += a) += QLatin1String("partner_display_name=?");
    if (e->m_partnerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("partner_small_photo=?");
    if (e->m_partnerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("partner_large_photo=?");
    if (e->m_isFree.isValid())
        (sqlw += a) += QLatin1String("is_free=?");
    if (e->m_partnerLevel.isValid())
        (sqlw += a) += QLatin1String("partner_level=?");
    if (e->m_language1.isValid())
        (sqlw += a) += QLatin1String("language_1=?");
    if (e->m_language2.isValid())
        (sqlw += a) += QLatin1String("language_2=?");
    if (e->m_isSuccess.isValid())
        (sqlw += a) += QLatin1String("is_success=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int CallRecordDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM call_record"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

CallRecord CallRecordDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return CallRecord();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return CallRecordResult(db)(query);
}

CallRecord CallRecordDaoBase::findByExample(const CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return CallRecord();

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_cdrId.isValid())
        (sqlw += a) += QLatin1String("cdr_id=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_startCallTime.isValid())
        (sqlw += a) += QLatin1String("start_call_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_isAnswered.isValid())
        (sqlw += a) += QLatin1String("is_answered=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_cdrType.isValid())
        (sqlw += a) += QLatin1String("cdr_type=?");
    if (e->m_callerVeSysId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_sys_id=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_callerVeLoginId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_login_id=?");
    if (e->m_callerDisplayName.isValid())
        (sqlw += a) += QLatin1String("caller_display_name=?");
    if (e->m_callerMobile.isValid())
        (sqlw += a) += QLatin1String("caller_mobile=?");
    if (e->m_callerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("caller_small_photo=?");
    if (e->m_callerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("caller_large_photo=?");
    if (e->m_isPstnCall.isValid())
        (sqlw += a) += QLatin1String("is_pstn_call=?");
    if (e->m_callCountry.isValid())
        (sqlw += a) += QLatin1String("call_country=?");
    if (e->m_calleeVeSysId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_sys_id=?");
    if (e->m_calleeId.isValid())
        (sqlw += a) += QLatin1String("callee_id=?");
    if (e->m_calleeVeLoginId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_login_id=?");
    if (e->m_calleeDisplayName.isValid())
        (sqlw += a) += QLatin1String("callee_display_name=?");
    if (e->m_calleeMobile.isValid())
        (sqlw += a) += QLatin1String("callee_mobile=?");
    if (e->m_calleeSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("callee_small_photo=?");
    if (e->m_calleeLargePhoto.isValid())
        (sqlw += a) += QLatin1String("callee_large_photo=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_partnerId.isValid())
        (sqlw += a) += QLatin1String("partner_id=?");
    if (e->m_partnerLoginId.isValid())
        (sqlw += a) += QLatin1String("partner_login_id=?");
    if (e->m_partnerDisplayName.isValid())
        (sqlw += a) += QLatin1String("partner_display_name=?");
    if (e->m_partnerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("partner_small_photo=?");
    if (e->m_partnerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("partner_large_photo=?");
    if (e->m_isFree.isValid())
        (sqlw += a) += QLatin1String("is_free=?");
    if (e->m_partnerLevel.isValid())
        (sqlw += a) += QLatin1String("partner_level=?");
    if (e->m_language1.isValid())
        (sqlw += a) += QLatin1String("language_1=?");
    if (e->m_language2.isValid())
        (sqlw += a) += QLatin1String("language_2=?");
    if (e->m_isSuccess.isValid())
        (sqlw += a) += QLatin1String("is_success=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);

    return CallRecordResult(db)(query);
}

CallRecordList CallRecordDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record"
        ;

    Database* db = database();
    if (!db)
        return CallRecordList();

    return CallRecordResults(db)(sqlDecorate(sql));
}

CallRecordList CallRecordDaoBase::findAllByExample(const CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return CallRecordList();

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_cdrId.isValid())
        (sqlw += a) += QLatin1String("cdr_id=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_startCallTime.isValid())
        (sqlw += a) += QLatin1String("start_call_time=?");
    if (e->m_callTime.isValid())
        (sqlw += a) += QLatin1String("call_time=?");
    if (e->m_isAnswered.isValid())
        (sqlw += a) += QLatin1String("is_answered=?");
    if (e->m_callMode.isValid())
        (sqlw += a) += QLatin1String("call_mode=?");
    if (e->m_cdrType.isValid())
        (sqlw += a) += QLatin1String("cdr_type=?");
    if (e->m_callerVeSysId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_sys_id=?");
    if (e->m_callerId.isValid())
        (sqlw += a) += QLatin1String("caller_id=?");
    if (e->m_callerVeLoginId.isValid())
        (sqlw += a) += QLatin1String("caller_ve_login_id=?");
    if (e->m_callerDisplayName.isValid())
        (sqlw += a) += QLatin1String("caller_display_name=?");
    if (e->m_callerMobile.isValid())
        (sqlw += a) += QLatin1String("caller_mobile=?");
    if (e->m_callerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("caller_small_photo=?");
    if (e->m_callerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("caller_large_photo=?");
    if (e->m_isPstnCall.isValid())
        (sqlw += a) += QLatin1String("is_pstn_call=?");
    if (e->m_callCountry.isValid())
        (sqlw += a) += QLatin1String("call_country=?");
    if (e->m_calleeVeSysId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_sys_id=?");
    if (e->m_calleeId.isValid())
        (sqlw += a) += QLatin1String("callee_id=?");
    if (e->m_calleeVeLoginId.isValid())
        (sqlw += a) += QLatin1String("callee_ve_login_id=?");
    if (e->m_calleeDisplayName.isValid())
        (sqlw += a) += QLatin1String("callee_display_name=?");
    if (e->m_calleeMobile.isValid())
        (sqlw += a) += QLatin1String("callee_mobile=?");
    if (e->m_calleeSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("callee_small_photo=?");
    if (e->m_calleeLargePhoto.isValid())
        (sqlw += a) += QLatin1String("callee_large_photo=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_partnerId.isValid())
        (sqlw += a) += QLatin1String("partner_id=?");
    if (e->m_partnerLoginId.isValid())
        (sqlw += a) += QLatin1String("partner_login_id=?");
    if (e->m_partnerDisplayName.isValid())
        (sqlw += a) += QLatin1String("partner_display_name=?");
    if (e->m_partnerSmallPhoto.isValid())
        (sqlw += a) += QLatin1String("partner_small_photo=?");
    if (e->m_partnerLargePhoto.isValid())
        (sqlw += a) += QLatin1String("partner_large_photo=?");
    if (e->m_isFree.isValid())
        (sqlw += a) += QLatin1String("is_free=?");
    if (e->m_partnerLevel.isValid())
        (sqlw += a) += QLatin1String("partner_level=?");
    if (e->m_language1.isValid())
        (sqlw += a) += QLatin1String("language_1=?");
    if (e->m_language2.isValid())
        (sqlw += a) += QLatin1String("language_2=?");
    if (e->m_isSuccess.isValid())
        (sqlw += a) += QLatin1String("is_success=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);

    return CallRecordResults(db)(query);
}

bool CallRecordDaoBase::persist(CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO call_record ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_owner.isValid()) {
        (sql += c) += QLatin1String("owner");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cdrId.isValid()) {
        (sql += c) += QLatin1String("cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_displayName.isValid()) {
        (sql += c) += QLatin1String("display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_startCallTime.isValid()) {
        (sql += c) += QLatin1String("start_call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isAnswered.isValid()) {
        (sql += c) += QLatin1String("is_answered");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callMode.isValid()) {
        (sql += c) += QLatin1String("call_mode");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cdrType.isValid()) {
        (sql += c) += QLatin1String("cdr_type");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerVeSysId.isValid()) {
        (sql += c) += QLatin1String("caller_ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerId.isValid()) {
        (sql += c) += QLatin1String("caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerVeLoginId.isValid()) {
        (sql += c) += QLatin1String("caller_ve_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerDisplayName.isValid()) {
        (sql += c) += QLatin1String("caller_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerMobile.isValid()) {
        (sql += c) += QLatin1String("caller_mobile");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("caller_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerLargePhoto.isValid()) {
        (sql += c) += QLatin1String("caller_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isPstnCall.isValid()) {
        (sql += c) += QLatin1String("is_pstn_call");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callCountry.isValid()) {
        (sql += c) += QLatin1String("call_country");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeVeSysId.isValid()) {
        (sql += c) += QLatin1String("callee_ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeId.isValid()) {
        (sql += c) += QLatin1String("callee_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeVeLoginId.isValid()) {
        (sql += c) += QLatin1String("callee_ve_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeDisplayName.isValid()) {
        (sql += c) += QLatin1String("callee_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeMobile.isValid()) {
        (sql += c) += QLatin1String("callee_mobile");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("callee_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeLargePhoto.isValid()) {
        (sql += c) += QLatin1String("callee_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSysId.isValid()) {
        (sql += c) += QLatin1String("partner_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerId.isValid()) {
        (sql += c) += QLatin1String("partner_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLoginId.isValid()) {
        (sql += c) += QLatin1String("partner_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerDisplayName.isValid()) {
        (sql += c) += QLatin1String("partner_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("partner_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLargePhoto.isValid()) {
        (sql += c) += QLatin1String("partner_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isFree.isValid()) {
        (sql += c) += QLatin1String("is_free");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLevel.isValid()) {
        (sql += c) += QLatin1String("partner_level");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_language1.isValid()) {
        (sql += c) += QLatin1String("language_1");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_language2.isValid()) {
        (sql += c) += QLatin1String("language_2");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isSuccess.isValid()) {
        (sql += c) += QLatin1String("is_success");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool CallRecordDaoBase::merge(const CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO call_record ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_id.isValid()) {
        (sql += c) += QLatin1String("id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_owner.isValid()) {
        (sql += c) += QLatin1String("owner");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cdrId.isValid()) {
        (sql += c) += QLatin1String("cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_displayName.isValid()) {
        (sql += c) += QLatin1String("display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_startCallTime.isValid()) {
        (sql += c) += QLatin1String("start_call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callTime.isValid()) {
        (sql += c) += QLatin1String("call_time");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isAnswered.isValid()) {
        (sql += c) += QLatin1String("is_answered");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callMode.isValid()) {
        (sql += c) += QLatin1String("call_mode");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cdrType.isValid()) {
        (sql += c) += QLatin1String("cdr_type");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerVeSysId.isValid()) {
        (sql += c) += QLatin1String("caller_ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerId.isValid()) {
        (sql += c) += QLatin1String("caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerVeLoginId.isValid()) {
        (sql += c) += QLatin1String("caller_ve_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerDisplayName.isValid()) {
        (sql += c) += QLatin1String("caller_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerMobile.isValid()) {
        (sql += c) += QLatin1String("caller_mobile");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("caller_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callerLargePhoto.isValid()) {
        (sql += c) += QLatin1String("caller_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isPstnCall.isValid()) {
        (sql += c) += QLatin1String("is_pstn_call");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_callCountry.isValid()) {
        (sql += c) += QLatin1String("call_country");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeVeSysId.isValid()) {
        (sql += c) += QLatin1String("callee_ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeId.isValid()) {
        (sql += c) += QLatin1String("callee_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeVeLoginId.isValid()) {
        (sql += c) += QLatin1String("callee_ve_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeDisplayName.isValid()) {
        (sql += c) += QLatin1String("callee_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeMobile.isValid()) {
        (sql += c) += QLatin1String("callee_mobile");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("callee_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_calleeLargePhoto.isValid()) {
        (sql += c) += QLatin1String("callee_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSysId.isValid()) {
        (sql += c) += QLatin1String("partner_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerId.isValid()) {
        (sql += c) += QLatin1String("partner_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLoginId.isValid()) {
        (sql += c) += QLatin1String("partner_login_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerDisplayName.isValid()) {
        (sql += c) += QLatin1String("partner_display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSmallPhoto.isValid()) {
        (sql += c) += QLatin1String("partner_small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLargePhoto.isValid()) {
        (sql += c) += QLatin1String("partner_large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isFree.isValid()) {
        (sql += c) += QLatin1String("is_free");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerLevel.isValid()) {
        (sql += c) += QLatin1String("partner_level");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_language1.isValid()) {
        (sql += c) += QLatin1String("language_1");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_language2.isValid()) {
        (sql += c) += QLatin1String("language_2");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_isSuccess.isValid()) {
        (sql += c) += QLatin1String("is_success");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);

    return db->exec(query);
}

bool CallRecordDaoBase::update(const CallRecord& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const CallRecordPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE call_record SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_owner.isValid())
        (sqlv += c) += QLatin1String("owner=?");
    if (e->m_cdrId.isValid())
        (sqlv += c) += QLatin1String("cdr_id=?");
    if (e->m_meetCdrId.isValid())
        (sqlv += c) += QLatin1String("meet_cdr_id=?");
    if (e->m_smallPhoto.isValid())
        (sqlv += c) += QLatin1String("small_photo=?");
    if (e->m_displayName.isValid())
        (sqlv += c) += QLatin1String("display_name=?");
    if (e->m_startCallTime.isValid())
        (sqlv += c) += QLatin1String("start_call_time=?");
    if (e->m_callTime.isValid())
        (sqlv += c) += QLatin1String("call_time=?");
    if (e->m_isAnswered.isValid())
        (sqlv += c) += QLatin1String("is_answered=?");
    if (e->m_callMode.isValid())
        (sqlv += c) += QLatin1String("call_mode=?");
    if (e->m_cdrType.isValid())
        (sqlv += c) += QLatin1String("cdr_type=?");
    if (e->m_callerVeSysId.isValid())
        (sqlv += c) += QLatin1String("caller_ve_sys_id=?");
    if (e->m_callerId.isValid())
        (sqlv += c) += QLatin1String("caller_id=?");
    if (e->m_callerVeLoginId.isValid())
        (sqlv += c) += QLatin1String("caller_ve_login_id=?");
    if (e->m_callerDisplayName.isValid())
        (sqlv += c) += QLatin1String("caller_display_name=?");
    if (e->m_callerMobile.isValid())
        (sqlv += c) += QLatin1String("caller_mobile=?");
    if (e->m_callerSmallPhoto.isValid())
        (sqlv += c) += QLatin1String("caller_small_photo=?");
    if (e->m_callerLargePhoto.isValid())
        (sqlv += c) += QLatin1String("caller_large_photo=?");
    if (e->m_isPstnCall.isValid())
        (sqlv += c) += QLatin1String("is_pstn_call=?");
    if (e->m_callCountry.isValid())
        (sqlv += c) += QLatin1String("call_country=?");
    if (e->m_calleeVeSysId.isValid())
        (sqlv += c) += QLatin1String("callee_ve_sys_id=?");
    if (e->m_calleeId.isValid())
        (sqlv += c) += QLatin1String("callee_id=?");
    if (e->m_calleeVeLoginId.isValid())
        (sqlv += c) += QLatin1String("callee_ve_login_id=?");
    if (e->m_calleeDisplayName.isValid())
        (sqlv += c) += QLatin1String("callee_display_name=?");
    if (e->m_calleeMobile.isValid())
        (sqlv += c) += QLatin1String("callee_mobile=?");
    if (e->m_calleeSmallPhoto.isValid())
        (sqlv += c) += QLatin1String("callee_small_photo=?");
    if (e->m_calleeLargePhoto.isValid())
        (sqlv += c) += QLatin1String("callee_large_photo=?");
    if (e->m_partnerSysId.isValid())
        (sqlv += c) += QLatin1String("partner_sys_id=?");
    if (e->m_partnerId.isValid())
        (sqlv += c) += QLatin1String("partner_id=?");
    if (e->m_partnerLoginId.isValid())
        (sqlv += c) += QLatin1String("partner_login_id=?");
    if (e->m_partnerDisplayName.isValid())
        (sqlv += c) += QLatin1String("partner_display_name=?");
    if (e->m_partnerSmallPhoto.isValid())
        (sqlv += c) += QLatin1String("partner_small_photo=?");
    if (e->m_partnerLargePhoto.isValid())
        (sqlv += c) += QLatin1String("partner_large_photo=?");
    if (e->m_isFree.isValid())
        (sqlv += c) += QLatin1String("is_free=?");
    if (e->m_partnerLevel.isValid())
        (sqlv += c) += QLatin1String("partner_level=?");
    if (e->m_language1.isValid())
        (sqlv += c) += QLatin1String("language_1=?");
    if (e->m_language2.isValid())
        (sqlv += c) += QLatin1String("language_2=?");
    if (e->m_isSuccess.isValid())
        (sqlv += c) += QLatin1String("is_success=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_cdrId.isValid())
        query.addBindValue(e->m_cdrId);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_startCallTime.isValid())
        query.addBindValue(e->m_startCallTime);
    if (e->m_callTime.isValid())
        query.addBindValue(e->m_callTime);
    if (e->m_isAnswered.isValid())
        query.addBindValue(e->m_isAnswered);
    if (e->m_callMode.isValid())
        query.addBindValue(e->m_callMode);
    if (e->m_cdrType.isValid())
        query.addBindValue(e->m_cdrType);
    if (e->m_callerVeSysId.isValid())
        query.addBindValue(e->m_callerVeSysId);
    if (e->m_callerId.isValid())
        query.addBindValue(e->m_callerId);
    if (e->m_callerVeLoginId.isValid())
        query.addBindValue(e->m_callerVeLoginId);
    if (e->m_callerDisplayName.isValid())
        query.addBindValue(e->m_callerDisplayName);
    if (e->m_callerMobile.isValid())
        query.addBindValue(e->m_callerMobile);
    if (e->m_callerSmallPhoto.isValid())
        query.addBindValue(e->m_callerSmallPhoto);
    if (e->m_callerLargePhoto.isValid())
        query.addBindValue(e->m_callerLargePhoto);
    if (e->m_isPstnCall.isValid())
        query.addBindValue(e->m_isPstnCall);
    if (e->m_callCountry.isValid())
        query.addBindValue(e->m_callCountry);
    if (e->m_calleeVeSysId.isValid())
        query.addBindValue(e->m_calleeVeSysId);
    if (e->m_calleeId.isValid())
        query.addBindValue(e->m_calleeId);
    if (e->m_calleeVeLoginId.isValid())
        query.addBindValue(e->m_calleeVeLoginId);
    if (e->m_calleeDisplayName.isValid())
        query.addBindValue(e->m_calleeDisplayName);
    if (e->m_calleeMobile.isValid())
        query.addBindValue(e->m_calleeMobile);
    if (e->m_calleeSmallPhoto.isValid())
        query.addBindValue(e->m_calleeSmallPhoto);
    if (e->m_calleeLargePhoto.isValid())
        query.addBindValue(e->m_calleeLargePhoto);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_partnerId.isValid())
        query.addBindValue(e->m_partnerId);
    if (e->m_partnerLoginId.isValid())
        query.addBindValue(e->m_partnerLoginId);
    if (e->m_partnerDisplayName.isValid())
        query.addBindValue(e->m_partnerDisplayName);
    if (e->m_partnerSmallPhoto.isValid())
        query.addBindValue(e->m_partnerSmallPhoto);
    if (e->m_partnerLargePhoto.isValid())
        query.addBindValue(e->m_partnerLargePhoto);
    if (e->m_isFree.isValid())
        query.addBindValue(e->m_isFree);
    if (e->m_partnerLevel.isValid())
        query.addBindValue(e->m_partnerLevel);
    if (e->m_language1.isValid())
        query.addBindValue(e->m_language1);
    if (e->m_language2.isValid())
        query.addBindValue(e->m_language2);
    if (e->m_isSuccess.isValid())
        query.addBindValue(e->m_isSuccess);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool CallRecordDaoBase::remove(const CallRecord& entity)
{
    static const char sql[] =
        "DELETE FROM call_record "
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

bool CallRecordDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM call_record"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='call_record'"
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

bool CallRecordDaoBase::refresh(CallRecord& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const CallRecordPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = CallRecord(query);
    return true;
}

//$BEGIN-CUSTOM$
CallRecordDao::CallRecordDao(Database* db) :
    CallRecordDaoBase(db)
{
}

CallRecordDao::~CallRecordDao()
{
}

bool CallRecordDao::createTable()
{
    return CallRecordDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_call_record_xx ON call_record ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!CallRecordDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool CallRecordDao::dropTable()
{
    return CallRecordDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

CallRecordList CallRecordDao::findAllByOwner(const QString& owner)
{
	Database* db = database();
	if (!db)
		return CallRecordList();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("owner=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);

	return CallRecordResults(db)(query);
}

CallRecord CallRecordDao::findRecentByOwner(const QString& owner)
{
	Database* db = database();
	if (!db)
		return CallRecord();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("owner=?");

	sql += sqlw;
	sql += QLatin1String(" ORDER BY cast(cdr_id as int) DESC");

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	query.addBindValue(owner);

	return CallRecordResult(db)(query);
}

CallRecord CallRecordDao::findByOwnerAndMeetCDRId(const QString& owner, const QString& meetCDRId)
{
	Database* db = database();
	if (!db)
		return CallRecord();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");
	(sqlw += a) += QLatin1String("meet_cdr_id=?");

	sql += sqlw;
	sql += QLatin1String(" ORDER BY cdr_id DESC LIMIT 0,1");

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	query.addBindValue(owner);
	query.addBindValue(meetCDRId);

	return CallRecordResult(db)(query);
}

CallRecordList CallRecordDao::callRecordAnswered(const QString& owner, const QString& start, const QString& end)
{
	Database* db = database();
	if (!db)
		return CallRecordList();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");
	(sqlw += a) += QLatin1String("is_answered=?");
	(sqlw += a) += QLatin1String("start_call_time BETWEEN ? AND ?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);
	query.addBindValue(QLatin1String("1"));
	query.addBindValue(start);
	query.addBindValue(end);

	return CallRecordResults(db)(query);
}

CallRecordList CallRecordDao::callRecordSuccess(const QString& owner, const QString& start, const QString& end)
{
	Database* db = database();
	if (!db)
		return CallRecordList();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");
	(sqlw += a) += QLatin1String("is_success=?");
	(sqlw += a) += QLatin1String("start_call_time BETWEEN ? AND ?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);
	query.addBindValue(QLatin1String("1"));
	query.addBindValue(start);
	query.addBindValue(end);

	return CallRecordResults(db)(query);
}

CallRecordList CallRecordDao::callRecordMissed(const QString& owner, const QString& start, const QString& end)
{
	Database* db = database();
	if (!db)
		return CallRecordList();

	QString sql(QLatin1String("SELECT "NS_USER_CALLRECORD_COLUMNS" FROM call_record WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");
	(sqlw += a) += QLatin1String("is_answered=?");
	(sqlw += a) += QLatin1String("start_call_time BETWEEN ? AND ?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);
	query.addBindValue(QLatin1String("2"));
	query.addBindValue(start);
	query.addBindValue(end);

	return CallRecordResults(db)(query);
}

//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
