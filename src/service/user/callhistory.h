// $Id$
/*
 * Copyright (C) 2014 Qpid Network
 * All rights reserved.
 *
 * Contact: Development Department <addison@qpidnetwork.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef QPID_USER_CALLHISTORY_H
#define QPID_USER_CALLHISTORY_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "calltype.h"
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
class QDateTime;
class QSqlQuery;
class QSqlRecord;
class QString;
template <typename T> class QList;
QT_END_NAMESPACE

namespace Qpid {
namespace User {

class CallHistory;
class CallHistoryPrivate;
typedef QList<CallHistory> CallHistoryList;

class QPID_SERVICE_DECL CallHistory
{
public:
    CallHistory();
    CallHistory(const CallHistory& rhs);
    explicit CallHistory(qlonglong id);
    explicit CallHistory(const QSqlQuery& query);
    explicit CallHistory(const QSqlRecord& record);
    ~CallHistory();

    CallHistory& operator=(const CallHistory& rhs);

    bool isValid() const;
    CallHistory identity() const;
    bool identityEqual(const CallHistory& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isUserNameNull() const;
    QString userName() const;
    void setUserNameNull();
    void setUserName(const QString& userName);

    bool isCallNameNull() const;
    QString callName() const;
    void setCallNameNull();
    void setCallName(const QString& callName);

    bool isDurationNull() const;
    int duration() const;
    void setDurationNull();
    void setDuration(int duration);

    bool isCallTimeNull() const;
    QDateTime callTime() const;
    void setCallTimeNull();
    void setCallTime(const QDateTime& callTime);

    bool isTypeNull() const;
    CallType type() const;
    void setTypeNull();
    void setType(CallType type);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class CallHistoryDaoBase;
    friend class CallHistoryDao;

    QSharedDataPointer<CallHistoryPrivate> d;
};

} // namespace Qpid::User
} // namespace Qpid

Q_DECLARE_METATYPE(Qpid::User::CallHistory)
Q_DECLARE_METATYPE(Qpid::User::CallHistoryList)

#endif // QPID_USER_CALLHISTORY_H
