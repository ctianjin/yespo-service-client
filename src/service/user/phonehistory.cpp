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
#include "phonehistory.h"
#include "phonehistory_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

PhoneHistoryPrivate::PhoneHistoryPrivate() :
    QSharedData()
{
}

PhoneHistoryPrivate::PhoneHistoryPrivate(const PhoneHistoryPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_userName(rhs.m_userName),
    m_meetCdrId(rhs.m_meetCdrId),
    m_cid(rhs.m_cid),
    m_callerId(rhs.m_callerId),
    m_deductionTime(rhs.m_deductionTime),
    m_callTime(rhs.m_callTime),
    m_contactName(rhs.m_contactName),
    m_smallPhoto(rhs.m_smallPhoto),
    m_largePhoto(rhs.m_largePhoto),
    m_callStatus(rhs.m_callStatus),
    m_callMode(rhs.m_callMode),
    m_deductionCost(rhs.m_deductionCost),
    m_sourceLanguage(rhs.m_sourceLanguage),
    m_targetLanguage(rhs.m_targetLanguage),
    m_pstnCountry(rhs.m_pstnCountry),
    m_voiceraId(rhs.m_voiceraId),
    m_photofile(rhs.m_photofile)
{
}

PhoneHistoryPrivate::PhoneHistoryPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

PhoneHistoryPrivate::PhoneHistoryPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_userName(query.value(1)),
    m_meetCdrId(query.value(2)),
    m_cid(query.value(3)),
    m_callerId(query.value(4)),
    m_deductionTime(query.value(5)),
    m_callTime(query.value(6)),
    m_contactName(query.value(7)),
    m_smallPhoto(query.value(8)),
    m_largePhoto(query.value(9)),
    m_callStatus(query.value(10)),
    m_callMode(query.value(11)),
    m_deductionCost(query.value(12)),
    m_sourceLanguage(query.value(13)),
    m_targetLanguage(query.value(14)),
    m_pstnCountry(query.value(15)),
    m_voiceraId(query.value(16)),
    m_photofile(query.value(17))
{
}

PhoneHistoryPrivate::PhoneHistoryPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_userName(record.value(QLatin1String("user_name"))),
    m_meetCdrId(record.value(QLatin1String("meet_cdr_id"))),
    m_cid(record.value(QLatin1String("cid"))),
    m_callerId(record.value(QLatin1String("caller_id"))),
    m_deductionTime(record.value(QLatin1String("deduction_time"))),
    m_callTime(record.value(QLatin1String("call_time"))),
    m_contactName(record.value(QLatin1String("contact_name"))),
    m_smallPhoto(record.value(QLatin1String("small_photo"))),
    m_largePhoto(record.value(QLatin1String("large_photo"))),
    m_callStatus(record.value(QLatin1String("call_status"))),
    m_callMode(record.value(QLatin1String("call_mode"))),
    m_deductionCost(record.value(QLatin1String("deduction_cost"))),
    m_sourceLanguage(record.value(QLatin1String("source_language"))),
    m_targetLanguage(record.value(QLatin1String("target_language"))),
    m_pstnCountry(record.value(QLatin1String("pstn_country"))),
    m_voiceraId(record.value(QLatin1String("voicera_id"))),
    m_photofile(record.value(QLatin1String("photoFile")))
{
}

PhoneHistoryPrivate::~PhoneHistoryPrivate()
{
}

PhoneHistory::PhoneHistory() :
    d(new PhoneHistoryPrivate)
{
}

PhoneHistory::PhoneHistory(const PhoneHistory& rhs) :
    d(rhs.d)
{
}

PhoneHistory::PhoneHistory(qlonglong id) :
    d(new PhoneHistoryPrivate(id))
{
}

PhoneHistory::PhoneHistory(const QSqlQuery& query) :
    d(new PhoneHistoryPrivate(query))
{
}

PhoneHistory::PhoneHistory(const QSqlRecord& record) :
    d(new PhoneHistoryPrivate(record))
{
}

PhoneHistory::~PhoneHistory()
{
}

PhoneHistory& PhoneHistory::operator=(const PhoneHistory& rhs)
{
    d = rhs.d;
    return *this;
}

bool PhoneHistory::isValid() const
{
    return d->m_id.isValid();
}

PhoneHistory PhoneHistory::identity() const
{
    PhoneHistory entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool PhoneHistory::identityEqual(const PhoneHistory& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool PhoneHistory::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong PhoneHistory::id() const
{
    return d->m_id.toLongLong();
}

void PhoneHistory::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void PhoneHistory::setId(qlonglong id)
{
    d->m_id = id;
}

bool PhoneHistory::isUserNameNull() const
{
    return d->m_userName.isNull();
}

QString PhoneHistory::userName() const
{
    return d->m_userName.toString();
}

void PhoneHistory::setUserNameNull()
{
    d->m_userName = QVariant(QVariant::String);
}

void PhoneHistory::setUserName(const QString& userName)
{
    d->m_userName = userName;
}

bool PhoneHistory::isMeetCdrIdNull() const
{
    return d->m_meetCdrId.isNull();
}

QString PhoneHistory::meetCdrId() const
{
    return d->m_meetCdrId.toString();
}

void PhoneHistory::setMeetCdrIdNull()
{
    d->m_meetCdrId = QVariant(QVariant::String);
}

void PhoneHistory::setMeetCdrId(const QString& meetCdrId)
{
    d->m_meetCdrId = meetCdrId;
}

bool PhoneHistory::isCidNull() const
{
    return d->m_cid.isNull();
}

QString PhoneHistory::cid() const
{
    return d->m_cid.toString();
}

void PhoneHistory::setCidNull()
{
    d->m_cid = QVariant(QVariant::String);
}

void PhoneHistory::setCid(const QString& cid)
{
    d->m_cid = cid;
}

bool PhoneHistory::isCallerIdNull() const
{
    return d->m_callerId.isNull();
}

QString PhoneHistory::callerId() const
{
    return d->m_callerId.toString();
}

void PhoneHistory::setCallerIdNull()
{
    d->m_callerId = QVariant(QVariant::String);
}

void PhoneHistory::setCallerId(const QString& callerId)
{
    d->m_callerId = callerId;
}

bool PhoneHistory::isDeductionTimeNull() const
{
    return d->m_deductionTime.isNull();
}

QString PhoneHistory::deductionTime() const
{
    return d->m_deductionTime.toString();
}

void PhoneHistory::setDeductionTimeNull()
{
    d->m_deductionTime = QVariant(QVariant::String);
}

void PhoneHistory::setDeductionTime(const QString& deductionTime)
{
    d->m_deductionTime = deductionTime;
}

bool PhoneHistory::isCallTimeNull() const
{
    return d->m_callTime.isNull();
}

QString PhoneHistory::callTime() const
{
    return d->m_callTime.toString();
}

void PhoneHistory::setCallTimeNull()
{
    d->m_callTime = QVariant(QVariant::String);
}

void PhoneHistory::setCallTime(const QString& callTime)
{
    d->m_callTime = callTime;
}

bool PhoneHistory::isContactNameNull() const
{
    return d->m_contactName.isNull();
}

QString PhoneHistory::contactName() const
{
    return d->m_contactName.toString();
}

void PhoneHistory::setContactNameNull()
{
    d->m_contactName = QVariant(QVariant::String);
}

void PhoneHistory::setContactName(const QString& contactName)
{
    d->m_contactName = contactName;
}

bool PhoneHistory::isSmallPhotoNull() const
{
    return d->m_smallPhoto.isNull();
}

QString PhoneHistory::smallPhoto() const
{
    return d->m_smallPhoto.toString();
}

void PhoneHistory::setSmallPhotoNull()
{
    d->m_smallPhoto = QVariant(QVariant::String);
}

void PhoneHistory::setSmallPhoto(const QString& smallPhoto)
{
    d->m_smallPhoto = smallPhoto;
}

bool PhoneHistory::isLargePhotoNull() const
{
    return d->m_largePhoto.isNull();
}

QString PhoneHistory::largePhoto() const
{
    return d->m_largePhoto.toString();
}

void PhoneHistory::setLargePhotoNull()
{
    d->m_largePhoto = QVariant(QVariant::String);
}

void PhoneHistory::setLargePhoto(const QString& largePhoto)
{
    d->m_largePhoto = largePhoto;
}

bool PhoneHistory::isCallStatusNull() const
{
    return d->m_callStatus.isNull();
}

QString PhoneHistory::callStatus() const
{
    return d->m_callStatus.toString();
}

void PhoneHistory::setCallStatusNull()
{
    d->m_callStatus = QVariant(QVariant::String);
}

void PhoneHistory::setCallStatus(const QString& callStatus)
{
    d->m_callStatus = callStatus;
}

bool PhoneHistory::isCallModeNull() const
{
    return d->m_callMode.isNull();
}

QString PhoneHistory::callMode() const
{
    return d->m_callMode.toString();
}

void PhoneHistory::setCallModeNull()
{
    d->m_callMode = QVariant(QVariant::String);
}

void PhoneHistory::setCallMode(const QString& callMode)
{
    d->m_callMode = callMode;
}

bool PhoneHistory::isDeductionCostNull() const
{
    return d->m_deductionCost.isNull();
}

QString PhoneHistory::deductionCost() const
{
    return d->m_deductionCost.toString();
}

void PhoneHistory::setDeductionCostNull()
{
    d->m_deductionCost = QVariant(QVariant::String);
}

void PhoneHistory::setDeductionCost(const QString& deductionCost)
{
    d->m_deductionCost = deductionCost;
}

bool PhoneHistory::isSourceLanguageNull() const
{
    return d->m_sourceLanguage.isNull();
}

QString PhoneHistory::sourceLanguage() const
{
    return d->m_sourceLanguage.toString();
}

void PhoneHistory::setSourceLanguageNull()
{
    d->m_sourceLanguage = QVariant(QVariant::String);
}

void PhoneHistory::setSourceLanguage(const QString& sourceLanguage)
{
    d->m_sourceLanguage = sourceLanguage;
}

bool PhoneHistory::isTargetLanguageNull() const
{
    return d->m_targetLanguage.isNull();
}

QString PhoneHistory::targetLanguage() const
{
    return d->m_targetLanguage.toString();
}

void PhoneHistory::setTargetLanguageNull()
{
    d->m_targetLanguage = QVariant(QVariant::String);
}

void PhoneHistory::setTargetLanguage(const QString& targetLanguage)
{
    d->m_targetLanguage = targetLanguage;
}

bool PhoneHistory::isPstnCountryNull() const
{
    return d->m_pstnCountry.isNull();
}

QString PhoneHistory::pstnCountry() const
{
    return d->m_pstnCountry.toString();
}

void PhoneHistory::setPstnCountryNull()
{
    d->m_pstnCountry = QVariant(QVariant::String);
}

void PhoneHistory::setPstnCountry(const QString& pstnCountry)
{
    d->m_pstnCountry = pstnCountry;
}

bool PhoneHistory::isVoiceraIdNull() const
{
    return d->m_voiceraId.isNull();
}

QString PhoneHistory::voiceraId() const
{
    return d->m_voiceraId.toString();
}

void PhoneHistory::setVoiceraIdNull()
{
    d->m_voiceraId = QVariant(QVariant::String);
}

void PhoneHistory::setVoiceraId(const QString& voiceraId)
{
    d->m_voiceraId = voiceraId;
}

bool PhoneHistory::isPhotofileNull() const
{
    return d->m_photofile.isNull();
}

QString PhoneHistory::photofile() const
{
    return d->m_photofile.toString();
}

void PhoneHistory::setPhotofileNull()
{
    d->m_photofile = QVariant(QVariant::String);
}

void PhoneHistory::setPhotofile(const QString& photofile)
{
    d->m_photofile = photofile;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
