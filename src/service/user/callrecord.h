// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CALLRECORD_H
#define NS_USER_CALLRECORD_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
class QString;
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {
namespace User {

class CallRecord;
class CallRecordPrivate;
typedef QList<CallRecord> CallRecordList;

class BUILDSYS_SERVICE_DECL CallRecord
{
public:
    CallRecord();
    CallRecord(const CallRecord& rhs);
    explicit CallRecord(qlonglong id);
    explicit CallRecord(const QSqlQuery& query);
    explicit CallRecord(const QSqlRecord& record);
    ~CallRecord();

    CallRecord& operator=(const CallRecord& rhs);

    bool isValid() const;
    CallRecord identity() const;
    bool identityEqual(const CallRecord& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isOwnerNull() const;
    QString owner() const;
    void setOwnerNull();
    void setOwner(const QString& owner);

    bool isCdrIdNull() const;
    QString cdrId() const;
    void setCdrIdNull();
    void setCdrId(const QString& cdrId);

    bool isMeetCdrIdNull() const;
    QString meetCdrId() const;
    void setMeetCdrIdNull();
    void setMeetCdrId(const QString& meetCdrId);

    bool isSmallPhotoNull() const;
    QString smallPhoto() const;
    void setSmallPhotoNull();
    void setSmallPhoto(const QString& smallPhoto);

    bool isDisplayNameNull() const;
    QString displayName() const;
    void setDisplayNameNull();
    void setDisplayName(const QString& displayName);

    bool isStartCallTimeNull() const;
    QString startCallTime() const;
    void setStartCallTimeNull();
    void setStartCallTime(const QString& startCallTime);

    bool isCallTimeNull() const;
    QString callTime() const;
    void setCallTimeNull();
    void setCallTime(const QString& callTime);

    bool isIsAnsweredNull() const;
    QString isAnswered() const;
    void setIsAnsweredNull();
    void setIsAnswered(const QString& isAnswered);

    bool isCallModeNull() const;
    QString callMode() const;
    void setCallModeNull();
    void setCallMode(const QString& callMode);

    bool isCdrTypeNull() const;
    QString cdrType() const;
    void setCdrTypeNull();
    void setCdrType(const QString& cdrType);

    bool isCallerVeSysIdNull() const;
    QString callerVeSysId() const;
    void setCallerVeSysIdNull();
    void setCallerVeSysId(const QString& callerVeSysId);

    bool isCallerIdNull() const;
    QString callerId() const;
    void setCallerIdNull();
    void setCallerId(const QString& callerId);

    bool isCallerVeLoginIdNull() const;
    QString callerVeLoginId() const;
    void setCallerVeLoginIdNull();
    void setCallerVeLoginId(const QString& callerVeLoginId);

    bool isCallerDisplayNameNull() const;
    QString callerDisplayName() const;
    void setCallerDisplayNameNull();
    void setCallerDisplayName(const QString& callerDisplayName);

    bool isCallerMobileNull() const;
    QString callerMobile() const;
    void setCallerMobileNull();
    void setCallerMobile(const QString& callerMobile);

    bool isCallerSmallPhotoNull() const;
    QString callerSmallPhoto() const;
    void setCallerSmallPhotoNull();
    void setCallerSmallPhoto(const QString& callerSmallPhoto);

    bool isCallerLargePhotoNull() const;
    QString callerLargePhoto() const;
    void setCallerLargePhotoNull();
    void setCallerLargePhoto(const QString& callerLargePhoto);

    bool isIsPstnCallNull() const;
    QString isPstnCall() const;
    void setIsPstnCallNull();
    void setIsPstnCall(const QString& isPstnCall);

    bool isCallCountryNull() const;
    QString callCountry() const;
    void setCallCountryNull();
    void setCallCountry(const QString& callCountry);

    bool isCalleeVeSysIdNull() const;
    QString calleeVeSysId() const;
    void setCalleeVeSysIdNull();
    void setCalleeVeSysId(const QString& calleeVeSysId);

    bool isCalleeIdNull() const;
    QString calleeId() const;
    void setCalleeIdNull();
    void setCalleeId(const QString& calleeId);

    bool isCalleeVeLoginIdNull() const;
    QString calleeVeLoginId() const;
    void setCalleeVeLoginIdNull();
    void setCalleeVeLoginId(const QString& calleeVeLoginId);

    bool isCalleeDisplayNameNull() const;
    QString calleeDisplayName() const;
    void setCalleeDisplayNameNull();
    void setCalleeDisplayName(const QString& calleeDisplayName);

    bool isCalleeMobileNull() const;
    QString calleeMobile() const;
    void setCalleeMobileNull();
    void setCalleeMobile(const QString& calleeMobile);

    bool isCalleeSmallPhotoNull() const;
    QString calleeSmallPhoto() const;
    void setCalleeSmallPhotoNull();
    void setCalleeSmallPhoto(const QString& calleeSmallPhoto);

    bool isCalleeLargePhotoNull() const;
    QString calleeLargePhoto() const;
    void setCalleeLargePhotoNull();
    void setCalleeLargePhoto(const QString& calleeLargePhoto);

    bool isPartnerSysIdNull() const;
    QString partnerSysId() const;
    void setPartnerSysIdNull();
    void setPartnerSysId(const QString& partnerSysId);

    bool isPartnerIdNull() const;
    QString partnerId() const;
    void setPartnerIdNull();
    void setPartnerId(const QString& partnerId);

    bool isPartnerLoginIdNull() const;
    QString partnerLoginId() const;
    void setPartnerLoginIdNull();
    void setPartnerLoginId(const QString& partnerLoginId);

    bool isPartnerDisplayNameNull() const;
    QString partnerDisplayName() const;
    void setPartnerDisplayNameNull();
    void setPartnerDisplayName(const QString& partnerDisplayName);

    bool isPartnerSmallPhotoNull() const;
    QString partnerSmallPhoto() const;
    void setPartnerSmallPhotoNull();
    void setPartnerSmallPhoto(const QString& partnerSmallPhoto);

    bool isPartnerLargePhotoNull() const;
    QString partnerLargePhoto() const;
    void setPartnerLargePhotoNull();
    void setPartnerLargePhoto(const QString& partnerLargePhoto);

    bool isIsFreeNull() const;
    QString isFree() const;
    void setIsFreeNull();
    void setIsFree(const QString& isFree);

    bool isPartnerLevelNull() const;
    QString partnerLevel() const;
    void setPartnerLevelNull();
    void setPartnerLevel(const QString& partnerLevel);

    bool isLanguage1Null() const;
    QString language1() const;
    void setLanguage1Null();
    void setLanguage1(const QString& language1);

    bool isLanguage2Null() const;
    QString language2() const;
    void setLanguage2Null();
    void setLanguage2(const QString& language2);

    bool isIsSuccessNull() const;
    QString isSuccess() const;
    void setIsSuccessNull();
    void setIsSuccess(const QString& isSuccess);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class CallRecordDaoBase;
    friend class CallRecordDao;

    QSharedDataPointer<CallRecordPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::CallRecord)
Q_DECLARE_METATYPE(ns::User::CallRecordList)

#endif // NS_USER_CALLRECORD_H
