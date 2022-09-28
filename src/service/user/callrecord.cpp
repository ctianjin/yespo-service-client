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
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "callrecord.h"
#include "callrecord_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

CallRecordPrivate::CallRecordPrivate() :
    QSharedData()
{
}

CallRecordPrivate::CallRecordPrivate(const CallRecordPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_owner(rhs.m_owner),
    m_cdrId(rhs.m_cdrId),
    m_meetCdrId(rhs.m_meetCdrId),
    m_smallPhoto(rhs.m_smallPhoto),
    m_displayName(rhs.m_displayName),
    m_startCallTime(rhs.m_startCallTime),
    m_callTime(rhs.m_callTime),
    m_isAnswered(rhs.m_isAnswered),
    m_callMode(rhs.m_callMode),
    m_cdrType(rhs.m_cdrType),
    m_callerVeSysId(rhs.m_callerVeSysId),
    m_callerId(rhs.m_callerId),
    m_callerVeLoginId(rhs.m_callerVeLoginId),
    m_callerDisplayName(rhs.m_callerDisplayName),
    m_callerMobile(rhs.m_callerMobile),
    m_callerSmallPhoto(rhs.m_callerSmallPhoto),
    m_callerLargePhoto(rhs.m_callerLargePhoto),
    m_isPstnCall(rhs.m_isPstnCall),
    m_callCountry(rhs.m_callCountry),
    m_calleeVeSysId(rhs.m_calleeVeSysId),
    m_calleeId(rhs.m_calleeId),
    m_calleeVeLoginId(rhs.m_calleeVeLoginId),
    m_calleeDisplayName(rhs.m_calleeDisplayName),
    m_calleeMobile(rhs.m_calleeMobile),
    m_calleeSmallPhoto(rhs.m_calleeSmallPhoto),
    m_calleeLargePhoto(rhs.m_calleeLargePhoto),
    m_partnerSysId(rhs.m_partnerSysId),
    m_partnerId(rhs.m_partnerId),
    m_partnerLoginId(rhs.m_partnerLoginId),
    m_partnerDisplayName(rhs.m_partnerDisplayName),
    m_partnerSmallPhoto(rhs.m_partnerSmallPhoto),
    m_partnerLargePhoto(rhs.m_partnerLargePhoto),
    m_isFree(rhs.m_isFree),
    m_partnerLevel(rhs.m_partnerLevel),
    m_language1(rhs.m_language1),
    m_language2(rhs.m_language2),
    m_isSuccess(rhs.m_isSuccess)
{
}

CallRecordPrivate::CallRecordPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

CallRecordPrivate::CallRecordPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_owner(query.value(1)),
    m_cdrId(query.value(2)),
    m_meetCdrId(query.value(3)),
    m_smallPhoto(query.value(4)),
    m_displayName(query.value(5)),
    m_startCallTime(query.value(6)),
    m_callTime(query.value(7)),
    m_isAnswered(query.value(8)),
    m_callMode(query.value(9)),
    m_cdrType(query.value(10)),
    m_callerVeSysId(query.value(11)),
    m_callerId(query.value(12)),
    m_callerVeLoginId(query.value(13)),
    m_callerDisplayName(query.value(14)),
    m_callerMobile(query.value(15)),
    m_callerSmallPhoto(query.value(16)),
    m_callerLargePhoto(query.value(17)),
    m_isPstnCall(query.value(18)),
    m_callCountry(query.value(19)),
    m_calleeVeSysId(query.value(20)),
    m_calleeId(query.value(21)),
    m_calleeVeLoginId(query.value(22)),
    m_calleeDisplayName(query.value(23)),
    m_calleeMobile(query.value(24)),
    m_calleeSmallPhoto(query.value(25)),
    m_calleeLargePhoto(query.value(26)),
    m_partnerSysId(query.value(27)),
    m_partnerId(query.value(28)),
    m_partnerLoginId(query.value(29)),
    m_partnerDisplayName(query.value(30)),
    m_partnerSmallPhoto(query.value(31)),
    m_partnerLargePhoto(query.value(32)),
    m_isFree(query.value(33)),
    m_partnerLevel(query.value(34)),
    m_language1(query.value(35)),
    m_language2(query.value(36)),
    m_isSuccess(query.value(37))
{
}

CallRecordPrivate::CallRecordPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_owner(record.value(QLatin1String("owner"))),
    m_cdrId(record.value(QLatin1String("cdr_id"))),
    m_meetCdrId(record.value(QLatin1String("meet_cdr_id"))),
    m_smallPhoto(record.value(QLatin1String("small_photo"))),
    m_displayName(record.value(QLatin1String("display_name"))),
    m_startCallTime(record.value(QLatin1String("start_call_time"))),
    m_callTime(record.value(QLatin1String("call_time"))),
    m_isAnswered(record.value(QLatin1String("is_answered"))),
    m_callMode(record.value(QLatin1String("call_mode"))),
    m_cdrType(record.value(QLatin1String("cdr_type"))),
    m_callerVeSysId(record.value(QLatin1String("caller_ve_sys_id"))),
    m_callerId(record.value(QLatin1String("caller_id"))),
    m_callerVeLoginId(record.value(QLatin1String("caller_ve_login_id"))),
    m_callerDisplayName(record.value(QLatin1String("caller_display_name"))),
    m_callerMobile(record.value(QLatin1String("caller_mobile"))),
    m_callerSmallPhoto(record.value(QLatin1String("caller_small_photo"))),
    m_callerLargePhoto(record.value(QLatin1String("caller_large_photo"))),
    m_isPstnCall(record.value(QLatin1String("is_pstn_call"))),
    m_callCountry(record.value(QLatin1String("call_country"))),
    m_calleeVeSysId(record.value(QLatin1String("callee_ve_sys_id"))),
    m_calleeId(record.value(QLatin1String("callee_id"))),
    m_calleeVeLoginId(record.value(QLatin1String("callee_ve_login_id"))),
    m_calleeDisplayName(record.value(QLatin1String("callee_display_name"))),
    m_calleeMobile(record.value(QLatin1String("callee_mobile"))),
    m_calleeSmallPhoto(record.value(QLatin1String("callee_small_photo"))),
    m_calleeLargePhoto(record.value(QLatin1String("callee_large_photo"))),
    m_partnerSysId(record.value(QLatin1String("partner_sys_id"))),
    m_partnerId(record.value(QLatin1String("partner_id"))),
    m_partnerLoginId(record.value(QLatin1String("partner_login_id"))),
    m_partnerDisplayName(record.value(QLatin1String("partner_display_name"))),
    m_partnerSmallPhoto(record.value(QLatin1String("partner_small_photo"))),
    m_partnerLargePhoto(record.value(QLatin1String("partner_large_photo"))),
    m_isFree(record.value(QLatin1String("is_free"))),
    m_partnerLevel(record.value(QLatin1String("partner_level"))),
    m_language1(record.value(QLatin1String("language_1"))),
    m_language2(record.value(QLatin1String("language_2"))),
    m_isSuccess(record.value(QLatin1String("is_success")))
{
}

CallRecordPrivate::~CallRecordPrivate()
{
}

CallRecord::CallRecord() :
    d(new CallRecordPrivate)
{
}

CallRecord::CallRecord(const CallRecord& rhs) :
    d(rhs.d)
{
}

CallRecord::CallRecord(qlonglong id) :
    d(new CallRecordPrivate(id))
{
}

CallRecord::CallRecord(const QSqlQuery& query) :
    d(new CallRecordPrivate(query))
{
}

CallRecord::CallRecord(const QSqlRecord& record) :
    d(new CallRecordPrivate(record))
{
}

CallRecord::~CallRecord()
{
}

CallRecord& CallRecord::operator=(const CallRecord& rhs)
{
    d = rhs.d;
    return *this;
}

bool CallRecord::isValid() const
{
    return d->m_id.isValid();
}

CallRecord CallRecord::identity() const
{
    CallRecord entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool CallRecord::identityEqual(const CallRecord& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool CallRecord::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong CallRecord::id() const
{
    return d->m_id.toLongLong();
}

void CallRecord::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void CallRecord::setId(qlonglong id)
{
    d->m_id = id;
}

bool CallRecord::isOwnerNull() const
{
    return d->m_owner.isNull();
}

QString CallRecord::owner() const
{
    return d->m_owner.toString();
}

void CallRecord::setOwnerNull()
{
    d->m_owner = QVariant(QVariant::String);
}

void CallRecord::setOwner(const QString& owner)
{
    d->m_owner = owner;
}

bool CallRecord::isCdrIdNull() const
{
    return d->m_cdrId.isNull();
}

QString CallRecord::cdrId() const
{
    return d->m_cdrId.toString();
}

void CallRecord::setCdrIdNull()
{
    d->m_cdrId = QVariant(QVariant::String);
}

void CallRecord::setCdrId(const QString& cdrId)
{
    d->m_cdrId = cdrId;
}

bool CallRecord::isMeetCdrIdNull() const
{
    return d->m_meetCdrId.isNull();
}

QString CallRecord::meetCdrId() const
{
    return d->m_meetCdrId.toString();
}

void CallRecord::setMeetCdrIdNull()
{
    d->m_meetCdrId = QVariant(QVariant::String);
}

void CallRecord::setMeetCdrId(const QString& meetCdrId)
{
    d->m_meetCdrId = meetCdrId;
}

bool CallRecord::isSmallPhotoNull() const
{
    return d->m_smallPhoto.isNull();
}

QString CallRecord::smallPhoto() const
{
    return d->m_smallPhoto.toString();
}

void CallRecord::setSmallPhotoNull()
{
    d->m_smallPhoto = QVariant(QVariant::String);
}

void CallRecord::setSmallPhoto(const QString& smallPhoto)
{
    d->m_smallPhoto = smallPhoto;
}

bool CallRecord::isDisplayNameNull() const
{
    return d->m_displayName.isNull();
}

QString CallRecord::displayName() const
{
    return d->m_displayName.toString();
}

void CallRecord::setDisplayNameNull()
{
    d->m_displayName = QVariant(QVariant::String);
}

void CallRecord::setDisplayName(const QString& displayName)
{
    d->m_displayName = displayName;
}

bool CallRecord::isStartCallTimeNull() const
{
    return d->m_startCallTime.isNull();
}

QString CallRecord::startCallTime() const
{
    return d->m_startCallTime.toString();
}

void CallRecord::setStartCallTimeNull()
{
    d->m_startCallTime = QVariant(QVariant::String);
}

void CallRecord::setStartCallTime(const QString& startCallTime)
{
    d->m_startCallTime = startCallTime;
}

bool CallRecord::isCallTimeNull() const
{
    return d->m_callTime.isNull();
}

QString CallRecord::callTime() const
{
    return d->m_callTime.toString();
}

void CallRecord::setCallTimeNull()
{
    d->m_callTime = QVariant(QVariant::String);
}

void CallRecord::setCallTime(const QString& callTime)
{
    d->m_callTime = callTime;
}

bool CallRecord::isIsAnsweredNull() const
{
    return d->m_isAnswered.isNull();
}

QString CallRecord::isAnswered() const
{
    return d->m_isAnswered.toString();
}

void CallRecord::setIsAnsweredNull()
{
    d->m_isAnswered = QVariant(QVariant::String);
}

void CallRecord::setIsAnswered(const QString& isAnswered)
{
    d->m_isAnswered = isAnswered;
}

bool CallRecord::isCallModeNull() const
{
    return d->m_callMode.isNull();
}

QString CallRecord::callMode() const
{
    return d->m_callMode.toString();
}

void CallRecord::setCallModeNull()
{
    d->m_callMode = QVariant(QVariant::String);
}

void CallRecord::setCallMode(const QString& callMode)
{
    d->m_callMode = callMode;
}

bool CallRecord::isCdrTypeNull() const
{
    return d->m_cdrType.isNull();
}

QString CallRecord::cdrType() const
{
    return d->m_cdrType.toString();
}

void CallRecord::setCdrTypeNull()
{
    d->m_cdrType = QVariant(QVariant::String);
}

void CallRecord::setCdrType(const QString& cdrType)
{
    d->m_cdrType = cdrType;
}

bool CallRecord::isCallerVeSysIdNull() const
{
    return d->m_callerVeSysId.isNull();
}

QString CallRecord::callerVeSysId() const
{
    return d->m_callerVeSysId.toString();
}

void CallRecord::setCallerVeSysIdNull()
{
    d->m_callerVeSysId = QVariant(QVariant::String);
}

void CallRecord::setCallerVeSysId(const QString& callerVeSysId)
{
    d->m_callerVeSysId = callerVeSysId;
}

bool CallRecord::isCallerIdNull() const
{
    return d->m_callerId.isNull();
}

QString CallRecord::callerId() const
{
    return d->m_callerId.toString();
}

void CallRecord::setCallerIdNull()
{
    d->m_callerId = QVariant(QVariant::String);
}

void CallRecord::setCallerId(const QString& callerId)
{
    d->m_callerId = callerId;
}

bool CallRecord::isCallerVeLoginIdNull() const
{
    return d->m_callerVeLoginId.isNull();
}

QString CallRecord::callerVeLoginId() const
{
    return d->m_callerVeLoginId.toString();
}

void CallRecord::setCallerVeLoginIdNull()
{
    d->m_callerVeLoginId = QVariant(QVariant::String);
}

void CallRecord::setCallerVeLoginId(const QString& callerVeLoginId)
{
    d->m_callerVeLoginId = callerVeLoginId;
}

bool CallRecord::isCallerDisplayNameNull() const
{
    return d->m_callerDisplayName.isNull();
}

QString CallRecord::callerDisplayName() const
{
    return d->m_callerDisplayName.toString();
}

void CallRecord::setCallerDisplayNameNull()
{
    d->m_callerDisplayName = QVariant(QVariant::String);
}

void CallRecord::setCallerDisplayName(const QString& callerDisplayName)
{
    d->m_callerDisplayName = callerDisplayName;
}

bool CallRecord::isCallerMobileNull() const
{
    return d->m_callerMobile.isNull();
}

QString CallRecord::callerMobile() const
{
    return d->m_callerMobile.toString();
}

void CallRecord::setCallerMobileNull()
{
    d->m_callerMobile = QVariant(QVariant::String);
}

void CallRecord::setCallerMobile(const QString& callerMobile)
{
    d->m_callerMobile = callerMobile;
}

bool CallRecord::isCallerSmallPhotoNull() const
{
    return d->m_callerSmallPhoto.isNull();
}

QString CallRecord::callerSmallPhoto() const
{
    return d->m_callerSmallPhoto.toString();
}

void CallRecord::setCallerSmallPhotoNull()
{
    d->m_callerSmallPhoto = QVariant(QVariant::String);
}

void CallRecord::setCallerSmallPhoto(const QString& callerSmallPhoto)
{
    d->m_callerSmallPhoto = callerSmallPhoto;
}

bool CallRecord::isCallerLargePhotoNull() const
{
    return d->m_callerLargePhoto.isNull();
}

QString CallRecord::callerLargePhoto() const
{
    return d->m_callerLargePhoto.toString();
}

void CallRecord::setCallerLargePhotoNull()
{
    d->m_callerLargePhoto = QVariant(QVariant::String);
}

void CallRecord::setCallerLargePhoto(const QString& callerLargePhoto)
{
    d->m_callerLargePhoto = callerLargePhoto;
}

bool CallRecord::isIsPstnCallNull() const
{
    return d->m_isPstnCall.isNull();
}

QString CallRecord::isPstnCall() const
{
    return d->m_isPstnCall.toString();
}

void CallRecord::setIsPstnCallNull()
{
    d->m_isPstnCall = QVariant(QVariant::String);
}

void CallRecord::setIsPstnCall(const QString& isPstnCall)
{
    d->m_isPstnCall = isPstnCall;
}

bool CallRecord::isCallCountryNull() const
{
    return d->m_callCountry.isNull();
}

QString CallRecord::callCountry() const
{
    return d->m_callCountry.toString();
}

void CallRecord::setCallCountryNull()
{
    d->m_callCountry = QVariant(QVariant::String);
}

void CallRecord::setCallCountry(const QString& callCountry)
{
    d->m_callCountry = callCountry;
}

bool CallRecord::isCalleeVeSysIdNull() const
{
    return d->m_calleeVeSysId.isNull();
}

QString CallRecord::calleeVeSysId() const
{
    return d->m_calleeVeSysId.toString();
}

void CallRecord::setCalleeVeSysIdNull()
{
    d->m_calleeVeSysId = QVariant(QVariant::String);
}

void CallRecord::setCalleeVeSysId(const QString& calleeVeSysId)
{
    d->m_calleeVeSysId = calleeVeSysId;
}

bool CallRecord::isCalleeIdNull() const
{
    return d->m_calleeId.isNull();
}

QString CallRecord::calleeId() const
{
    return d->m_calleeId.toString();
}

void CallRecord::setCalleeIdNull()
{
    d->m_calleeId = QVariant(QVariant::String);
}

void CallRecord::setCalleeId(const QString& calleeId)
{
    d->m_calleeId = calleeId;
}

bool CallRecord::isCalleeVeLoginIdNull() const
{
    return d->m_calleeVeLoginId.isNull();
}

QString CallRecord::calleeVeLoginId() const
{
    return d->m_calleeVeLoginId.toString();
}

void CallRecord::setCalleeVeLoginIdNull()
{
    d->m_calleeVeLoginId = QVariant(QVariant::String);
}

void CallRecord::setCalleeVeLoginId(const QString& calleeVeLoginId)
{
    d->m_calleeVeLoginId = calleeVeLoginId;
}

bool CallRecord::isCalleeDisplayNameNull() const
{
    return d->m_calleeDisplayName.isNull();
}

QString CallRecord::calleeDisplayName() const
{
    return d->m_calleeDisplayName.toString();
}

void CallRecord::setCalleeDisplayNameNull()
{
    d->m_calleeDisplayName = QVariant(QVariant::String);
}

void CallRecord::setCalleeDisplayName(const QString& calleeDisplayName)
{
    d->m_calleeDisplayName = calleeDisplayName;
}

bool CallRecord::isCalleeMobileNull() const
{
    return d->m_calleeMobile.isNull();
}

QString CallRecord::calleeMobile() const
{
    return d->m_calleeMobile.toString();
}

void CallRecord::setCalleeMobileNull()
{
    d->m_calleeMobile = QVariant(QVariant::String);
}

void CallRecord::setCalleeMobile(const QString& calleeMobile)
{
    d->m_calleeMobile = calleeMobile;
}

bool CallRecord::isCalleeSmallPhotoNull() const
{
    return d->m_calleeSmallPhoto.isNull();
}

QString CallRecord::calleeSmallPhoto() const
{
    return d->m_calleeSmallPhoto.toString();
}

void CallRecord::setCalleeSmallPhotoNull()
{
    d->m_calleeSmallPhoto = QVariant(QVariant::String);
}

void CallRecord::setCalleeSmallPhoto(const QString& calleeSmallPhoto)
{
    d->m_calleeSmallPhoto = calleeSmallPhoto;
}

bool CallRecord::isCalleeLargePhotoNull() const
{
    return d->m_calleeLargePhoto.isNull();
}

QString CallRecord::calleeLargePhoto() const
{
    return d->m_calleeLargePhoto.toString();
}

void CallRecord::setCalleeLargePhotoNull()
{
    d->m_calleeLargePhoto = QVariant(QVariant::String);
}

void CallRecord::setCalleeLargePhoto(const QString& calleeLargePhoto)
{
    d->m_calleeLargePhoto = calleeLargePhoto;
}

bool CallRecord::isPartnerSysIdNull() const
{
    return d->m_partnerSysId.isNull();
}

QString CallRecord::partnerSysId() const
{
    return d->m_partnerSysId.toString();
}

void CallRecord::setPartnerSysIdNull()
{
    d->m_partnerSysId = QVariant(QVariant::String);
}

void CallRecord::setPartnerSysId(const QString& partnerSysId)
{
    d->m_partnerSysId = partnerSysId;
}

bool CallRecord::isPartnerIdNull() const
{
    return d->m_partnerId.isNull();
}

QString CallRecord::partnerId() const
{
    return d->m_partnerId.toString();
}

void CallRecord::setPartnerIdNull()
{
    d->m_partnerId = QVariant(QVariant::String);
}

void CallRecord::setPartnerId(const QString& partnerId)
{
    d->m_partnerId = partnerId;
}

bool CallRecord::isPartnerLoginIdNull() const
{
    return d->m_partnerLoginId.isNull();
}

QString CallRecord::partnerLoginId() const
{
    return d->m_partnerLoginId.toString();
}

void CallRecord::setPartnerLoginIdNull()
{
    d->m_partnerLoginId = QVariant(QVariant::String);
}

void CallRecord::setPartnerLoginId(const QString& partnerLoginId)
{
    d->m_partnerLoginId = partnerLoginId;
}

bool CallRecord::isPartnerDisplayNameNull() const
{
    return d->m_partnerDisplayName.isNull();
}

QString CallRecord::partnerDisplayName() const
{
    return d->m_partnerDisplayName.toString();
}

void CallRecord::setPartnerDisplayNameNull()
{
    d->m_partnerDisplayName = QVariant(QVariant::String);
}

void CallRecord::setPartnerDisplayName(const QString& partnerDisplayName)
{
    d->m_partnerDisplayName = partnerDisplayName;
}

bool CallRecord::isPartnerSmallPhotoNull() const
{
    return d->m_partnerSmallPhoto.isNull();
}

QString CallRecord::partnerSmallPhoto() const
{
    return d->m_partnerSmallPhoto.toString();
}

void CallRecord::setPartnerSmallPhotoNull()
{
    d->m_partnerSmallPhoto = QVariant(QVariant::String);
}

void CallRecord::setPartnerSmallPhoto(const QString& partnerSmallPhoto)
{
    d->m_partnerSmallPhoto = partnerSmallPhoto;
}

bool CallRecord::isPartnerLargePhotoNull() const
{
    return d->m_partnerLargePhoto.isNull();
}

QString CallRecord::partnerLargePhoto() const
{
    return d->m_partnerLargePhoto.toString();
}

void CallRecord::setPartnerLargePhotoNull()
{
    d->m_partnerLargePhoto = QVariant(QVariant::String);
}

void CallRecord::setPartnerLargePhoto(const QString& partnerLargePhoto)
{
    d->m_partnerLargePhoto = partnerLargePhoto;
}

bool CallRecord::isIsFreeNull() const
{
    return d->m_isFree.isNull();
}

QString CallRecord::isFree() const
{
    return d->m_isFree.toString();
}

void CallRecord::setIsFreeNull()
{
    d->m_isFree = QVariant(QVariant::String);
}

void CallRecord::setIsFree(const QString& isFree)
{
    d->m_isFree = isFree;
}

bool CallRecord::isPartnerLevelNull() const
{
    return d->m_partnerLevel.isNull();
}

QString CallRecord::partnerLevel() const
{
    return d->m_partnerLevel.toString();
}

void CallRecord::setPartnerLevelNull()
{
    d->m_partnerLevel = QVariant(QVariant::String);
}

void CallRecord::setPartnerLevel(const QString& partnerLevel)
{
    d->m_partnerLevel = partnerLevel;
}

bool CallRecord::isLanguage1Null() const
{
    return d->m_language1.isNull();
}

QString CallRecord::language1() const
{
    return d->m_language1.toString();
}

void CallRecord::setLanguage1Null()
{
    d->m_language1 = QVariant(QVariant::String);
}

void CallRecord::setLanguage1(const QString& language1)
{
    d->m_language1 = language1;
}

bool CallRecord::isLanguage2Null() const
{
    return d->m_language2.isNull();
}

QString CallRecord::language2() const
{
    return d->m_language2.toString();
}

void CallRecord::setLanguage2Null()
{
    d->m_language2 = QVariant(QVariant::String);
}

void CallRecord::setLanguage2(const QString& language2)
{
    d->m_language2 = language2;
}

bool CallRecord::isIsSuccessNull() const
{
    return d->m_isSuccess.isNull();
}

QString CallRecord::isSuccess() const
{
    return d->m_isSuccess.toString();
}

void CallRecord::setIsSuccessNull()
{
    d->m_isSuccess = QVariant(QVariant::String);
}

void CallRecord::setIsSuccess(const QString& isSuccess)
{
    d->m_isSuccess = isSuccess;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
