// $Id$
/*
 * Copyright (C) 2014 Qpid Network
 * All rights reserved.
 *
 * Contact: Development Department <addison@qpidnetwork.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef QPID_USER_CALLHISTORY_P_H
#define QPID_USER_CALLHISTORY_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace Qpid {
namespace User {

class CallHistoryPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_userName;
    QVariant m_callName;
    QVariant m_duration;
    QVariant m_callTime;
    QVariant m_type;

    CallHistoryPrivate();
    CallHistoryPrivate(const CallHistoryPrivate& rhs);
    explicit CallHistoryPrivate(qlonglong id);
    explicit CallHistoryPrivate(const QSqlQuery& query);
    explicit CallHistoryPrivate(const QSqlRecord& record);
    ~CallHistoryPrivate();
};

} // namespace Qpid::User
} // namespace Qpid

#endif // QPID_USER_CALLHISTORY_P_H
