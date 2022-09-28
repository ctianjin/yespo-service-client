// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CALLRECORD_P_H
#define NS_USER_CALLRECORD_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class CallRecordPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_owner;
    QVariant m_cdrId;
    QVariant m_meetCdrId;
    QVariant m_smallPhoto;
    QVariant m_displayName;
    QVariant m_startCallTime;
    QVariant m_callTime;
    QVariant m_isAnswered;
    QVariant m_callMode;
    QVariant m_cdrType;
    QVariant m_callerVeSysId;
    QVariant m_callerId;
    QVariant m_callerVeLoginId;
    QVariant m_callerDisplayName;
    QVariant m_callerMobile;
    QVariant m_callerSmallPhoto;
    QVariant m_callerLargePhoto;
    QVariant m_isPstnCall;
    QVariant m_callCountry;
    QVariant m_calleeVeSysId;
    QVariant m_calleeId;
    QVariant m_calleeVeLoginId;
    QVariant m_calleeDisplayName;
    QVariant m_calleeMobile;
    QVariant m_calleeSmallPhoto;
    QVariant m_calleeLargePhoto;
    QVariant m_partnerSysId;
    QVariant m_partnerId;
    QVariant m_partnerLoginId;
    QVariant m_partnerDisplayName;
    QVariant m_partnerSmallPhoto;
    QVariant m_partnerLargePhoto;
    QVariant m_isFree;
    QVariant m_partnerLevel;
    QVariant m_language1;
    QVariant m_language2;
    QVariant m_isSuccess;

    CallRecordPrivate();
    CallRecordPrivate(const CallRecordPrivate& rhs);
    explicit CallRecordPrivate(qlonglong id);
    explicit CallRecordPrivate(const QSqlQuery& query);
    explicit CallRecordPrivate(const QSqlRecord& record);
    ~CallRecordPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_CALLRECORD_P_H
