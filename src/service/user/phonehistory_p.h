// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_PHONEHISTORY_P_H
#define NS_USER_PHONEHISTORY_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class PhoneHistoryPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_userName;
    QVariant m_meetCdrId;
    QVariant m_cid;
    QVariant m_callerId;
    QVariant m_deductionTime;
    QVariant m_callTime;
    QVariant m_contactName;
    QVariant m_smallPhoto;
    QVariant m_largePhoto;
    QVariant m_callStatus;
    QVariant m_callMode;
    QVariant m_deductionCost;
    QVariant m_sourceLanguage;
    QVariant m_targetLanguage;
    QVariant m_pstnCountry;
    QVariant m_voiceraId;
    QVariant m_photofile;

    PhoneHistoryPrivate();
    PhoneHistoryPrivate(const PhoneHistoryPrivate& rhs);
    explicit PhoneHistoryPrivate(qlonglong id);
    explicit PhoneHistoryPrivate(const QSqlQuery& query);
    explicit PhoneHistoryPrivate(const QSqlRecord& record);
    ~PhoneHistoryPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_PHONEHISTORY_P_H
