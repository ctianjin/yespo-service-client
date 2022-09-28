// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_PHONEHISTORY_H
#define NS_USER_PHONEHISTORY_H

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

class PhoneHistory;
class PhoneHistoryPrivate;
typedef QList<PhoneHistory> PhoneHistoryList;

class BUILDSYS_SERVICE_DECL PhoneHistory
{
public:
    PhoneHistory();
    PhoneHistory(const PhoneHistory& rhs);
    explicit PhoneHistory(qlonglong id);
    explicit PhoneHistory(const QSqlQuery& query);
    explicit PhoneHistory(const QSqlRecord& record);
    ~PhoneHistory();

    PhoneHistory& operator=(const PhoneHistory& rhs);

    bool isValid() const;
    PhoneHistory identity() const;
    bool identityEqual(const PhoneHistory& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isUserNameNull() const;
    QString userName() const;
    void setUserNameNull();
    void setUserName(const QString& userName);

    bool isMeetCdrIdNull() const;
    QString meetCdrId() const;
    void setMeetCdrIdNull();
    void setMeetCdrId(const QString& meetCdrId);

    bool isCidNull() const;
    QString cid() const;
    void setCidNull();
    void setCid(const QString& cid);

    bool isCallerIdNull() const;
    QString callerId() const;
    void setCallerIdNull();
    void setCallerId(const QString& callerId);

    bool isDeductionTimeNull() const;
    QString deductionTime() const;
    void setDeductionTimeNull();
    void setDeductionTime(const QString& deductionTime);

    bool isCallTimeNull() const;
    QString callTime() const;
    void setCallTimeNull();
    void setCallTime(const QString& callTime);

    bool isContactNameNull() const;
    QString contactName() const;
    void setContactNameNull();
    void setContactName(const QString& contactName);

    bool isSmallPhotoNull() const;
    QString smallPhoto() const;
    void setSmallPhotoNull();
    void setSmallPhoto(const QString& smallPhoto);

    bool isLargePhotoNull() const;
    QString largePhoto() const;
    void setLargePhotoNull();
    void setLargePhoto(const QString& largePhoto);

    bool isCallStatusNull() const;
    QString callStatus() const;
    void setCallStatusNull();
    void setCallStatus(const QString& callStatus);

    bool isCallModeNull() const;
    QString callMode() const;
    void setCallModeNull();
    void setCallMode(const QString& callMode);

    bool isDeductionCostNull() const;
    QString deductionCost() const;
    void setDeductionCostNull();
    void setDeductionCost(const QString& deductionCost);

    bool isSourceLanguageNull() const;
    QString sourceLanguage() const;
    void setSourceLanguageNull();
    void setSourceLanguage(const QString& sourceLanguage);

    bool isTargetLanguageNull() const;
    QString targetLanguage() const;
    void setTargetLanguageNull();
    void setTargetLanguage(const QString& targetLanguage);

    bool isPstnCountryNull() const;
    QString pstnCountry() const;
    void setPstnCountryNull();
    void setPstnCountry(const QString& pstnCountry);

    bool isVoiceraIdNull() const;
    QString voiceraId() const;
    void setVoiceraIdNull();
    void setVoiceraId(const QString& voiceraId);

    bool isPhotofileNull() const;
    QString photofile() const;
    void setPhotofileNull();
    void setPhotofile(const QString& photofile);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class PhoneHistoryDaoBase;
    friend class PhoneHistoryDao;

    QSharedDataPointer<PhoneHistoryPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::PhoneHistory)
Q_DECLARE_METATYPE(ns::User::PhoneHistoryList)

#endif // NS_USER_PHONEHISTORY_H
