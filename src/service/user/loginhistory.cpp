// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "loginhistory.h"
#include "loginhistory_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

LoginHistoryPrivate::LoginHistoryPrivate() :
    QSharedData()
{
}

LoginHistoryPrivate::LoginHistoryPrivate(const LoginHistoryPrivate& rhs) :
    QSharedData(rhs),
    m_userName(rhs.m_userName),
    m_password(rhs.m_password),
    m_rememberPassword(rhs.m_rememberPassword),
    m_loginTime(rhs.m_loginTime)
{
}

LoginHistoryPrivate::LoginHistoryPrivate(const QString& userName) :
    QSharedData(),
    m_userName(userName)
{
}

LoginHistoryPrivate::LoginHistoryPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_userName(query.value(0)),
    m_password(query.value(1)),
    m_rememberPassword(query.value(2)),
    m_loginTime(query.value(3))
{
}

LoginHistoryPrivate::LoginHistoryPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_userName(record.value(QLatin1String("user_name"))),
    m_password(record.value(QLatin1String("password"))),
    m_rememberPassword(record.value(QLatin1String("remember_password"))),
    m_loginTime(record.value(QLatin1String("login_time")))
{
}

LoginHistoryPrivate::~LoginHistoryPrivate()
{
}

LoginHistory::LoginHistory() :
    d(new LoginHistoryPrivate)
{
}

LoginHistory::LoginHistory(const LoginHistory& rhs) :
    d(rhs.d)
{
}

LoginHistory::LoginHistory(const QString& userName) :
    d(new LoginHistoryPrivate(userName))
{
}

LoginHistory::LoginHistory(const QSqlQuery& query) :
    d(new LoginHistoryPrivate(query))
{
}

LoginHistory::LoginHistory(const QSqlRecord& record) :
    d(new LoginHistoryPrivate(record))
{
}

LoginHistory::~LoginHistory()
{
}

LoginHistory& LoginHistory::operator=(const LoginHistory& rhs)
{
    d = rhs.d;
    return *this;
}

bool LoginHistory::isValid() const
{
    return d->m_userName.isValid();
}

LoginHistory LoginHistory::identity() const
{
    LoginHistory entity;
    entity.d->m_userName = d->m_userName;
    return entity;
}

bool LoginHistory::identityEqual(const LoginHistory& rhs) const
{
    return d->m_userName == rhs.d->m_userName;
}

bool LoginHistory::isUserNameNull() const
{
    return d->m_userName.isNull();
}

QString LoginHistory::userName() const
{
    return d->m_userName.toString();
}

void LoginHistory::setUserNameNull()
{
    d->m_userName = QVariant(QVariant::String);
}

void LoginHistory::setUserName(const QString& userName)
{
    d->m_userName = userName;
}

bool LoginHistory::isPasswordNull() const
{
    return d->m_password.isNull();
}

QString LoginHistory::password() const
{
    return d->m_password.toString();
}

void LoginHistory::setPasswordNull()
{
    d->m_password = QVariant(QVariant::String);
}

void LoginHistory::setPassword(const QString& password)
{
    d->m_password = password;
}

bool LoginHistory::isRememberPasswordNull() const
{
    return d->m_rememberPassword.isNull();
}

bool LoginHistory::rememberPassword() const
{
    return d->m_rememberPassword.toBool();
}

void LoginHistory::setRememberPasswordNull()
{
    d->m_rememberPassword = QVariant(QVariant::Int);
}

void LoginHistory::setRememberPassword(bool rememberPassword)
{
    d->m_rememberPassword = rememberPassword ? 1 : 0;
}

bool LoginHistory::isLoginTimeNull() const
{
    return d->m_loginTime.isNull();
}

QDateTime LoginHistory::loginTime() const
{
    return d->m_loginTime.isNull() ? QDateTime() : QDateTime::fromTime_t(d->m_loginTime.toUInt());
}

uint LoginHistory::loginTimeValue() const
{
    return d->m_loginTime.toUInt();
}

void LoginHistory::setLoginTimeNull()
{
    d->m_loginTime = QVariant(QVariant::UInt);
}

void LoginHistory::setLoginTime(const QDateTime& loginTime)
{
    if (!loginTime.isValid()) {
        d->m_loginTime = QVariant(QVariant::UInt);
    } else {
        d->m_loginTime = loginTime.toTime_t();
    }
}

void LoginHistory::setLoginTimeValue(uint loginTime)
{
    d->m_loginTime = loginTime;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
