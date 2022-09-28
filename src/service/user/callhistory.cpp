// $Id$
/*
 * Copyright (C) 2014 Qpid Network
 * All rights reserved.
 *
 * Contact: Development Department <addison@qpidnetwork.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "callhistory.h"
#include "callhistory_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace Qpid {
namespace User {

CallHistoryPrivate::CallHistoryPrivate() :
    QSharedData()
{
}

CallHistoryPrivate::CallHistoryPrivate(const CallHistoryPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_userName(rhs.m_userName),
    m_callName(rhs.m_callName),
    m_duration(rhs.m_duration),
    m_callTime(rhs.m_callTime),
    m_type(rhs.m_type)
{
}

CallHistoryPrivate::CallHistoryPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

CallHistoryPrivate::CallHistoryPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_userName(query.value(1)),
    m_callName(query.value(2)),
    m_duration(query.value(3)),
    m_callTime(query.value(4)),
    m_type(query.value(5))
{
}

CallHistoryPrivate::CallHistoryPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_userName(record.value(QLatin1String("user_name"))),
    m_callName(record.value(QLatin1String("call_name"))),
    m_duration(record.value(QLatin1String("duration"))),
    m_callTime(record.value(QLatin1String("call_time"))),
    m_type(record.value(QLatin1String("type")))
{
}

CallHistoryPrivate::~CallHistoryPrivate()
{
}

CallHistory::CallHistory() :
    d(new CallHistoryPrivate)
{
}

CallHistory::CallHistory(const CallHistory& rhs) :
    d(rhs.d)
{
}

CallHistory::CallHistory(qlonglong id) :
    d(new CallHistoryPrivate(id))
{
}

CallHistory::CallHistory(const QSqlQuery& query) :
    d(new CallHistoryPrivate(query))
{
}

CallHistory::CallHistory(const QSqlRecord& record) :
    d(new CallHistoryPrivate(record))
{
}

CallHistory::~CallHistory()
{
}

CallHistory& CallHistory::operator=(const CallHistory& rhs)
{
    d = rhs.d;
    return *this;
}

bool CallHistory::isValid() const
{
    return d->m_id.isValid();
}

CallHistory CallHistory::identity() const
{
    CallHistory entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool CallHistory::identityEqual(const CallHistory& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool CallHistory::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong CallHistory::id() const
{
    return d->m_id.toLongLong();
}

void CallHistory::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void CallHistory::setId(qlonglong id)
{
    d->m_id = id;
}

bool CallHistory::isUserNameNull() const
{
    return d->m_userName.isNull();
}

QString CallHistory::userName() const
{
    return d->m_userName.toString();
}

void CallHistory::setUserNameNull()
{
    d->m_userName = QVariant(QVariant::String);
}

void CallHistory::setUserName(const QString& userName)
{
    d->m_userName = userName;
}

bool CallHistory::isCallNameNull() const
{
    return d->m_callName.isNull();
}

QString CallHistory::callName() const
{
    return d->m_callName.toString();
}

void CallHistory::setCallNameNull()
{
    d->m_callName = QVariant(QVariant::String);
}

void CallHistory::setCallName(const QString& callName)
{
    d->m_callName = callName;
}

bool CallHistory::isDurationNull() const
{
    return d->m_duration.isNull();
}

int CallHistory::duration() const
{
    return d->m_duration.toInt();
}

void CallHistory::setDurationNull()
{
    d->m_duration = QVariant(QVariant::Int);
}

void CallHistory::setDuration(int duration)
{
    d->m_duration = duration;
}

bool CallHistory::isCallTimeNull() const
{
    return d->m_callTime.isNull();
}

QDateTime CallHistory::callTime() const
{
    return d->m_callTime.toDateTime();
}

void CallHistory::setCallTimeNull()
{
    d->m_callTime = QVariant(QVariant::DateTime);
}

void CallHistory::setCallTime(const QDateTime& callTime)
{
    d->m_callTime = callTime;
}

bool CallHistory::isTypeNull() const
{
    return d->m_type.isNull();
}

CallType CallHistory::type() const
{
    return static_cast<CallType>(d->m_type.toInt());
}

void CallHistory::setTypeNull()
{
    d->m_type = QVariant(QVariant::Int);
}

void CallHistory::setType(CallType type)
{
    d->m_type = static_cast<int>(type);
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace Qpid::User
} // namespace Qpid
