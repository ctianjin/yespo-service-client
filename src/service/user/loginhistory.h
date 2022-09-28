// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_LOGINHISTORY_H
#define NS_USER_LOGINHISTORY_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
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

namespace ns {
namespace User {

class LoginHistory;
class LoginHistoryPrivate;
typedef QList<LoginHistory> LoginHistoryList;

class BUILDSYS_SERVICE_DECL LoginHistory
{
public:
    LoginHistory();
    LoginHistory(const LoginHistory& rhs);
    explicit LoginHistory(const QString& userName);
    explicit LoginHistory(const QSqlQuery& query);
    explicit LoginHistory(const QSqlRecord& record);
    ~LoginHistory();

    LoginHistory& operator=(const LoginHistory& rhs);

    bool isValid() const;
    LoginHistory identity() const;
    bool identityEqual(const LoginHistory& rhs) const;

    bool isUserNameNull() const;
    QString userName() const;
    void setUserNameNull();
    void setUserName(const QString& userName);

    bool isPasswordNull() const;
    QString password() const;
    void setPasswordNull();
    void setPassword(const QString& password);

    bool isRememberPasswordNull() const;
    bool rememberPassword() const;
    void setRememberPasswordNull();
    void setRememberPassword(bool rememberPassword);

    bool isLoginTimeNull() const;
    QDateTime loginTime() const;
    uint loginTimeValue() const;
    void setLoginTimeNull();
    void setLoginTime(const QDateTime& loginTime);
    void setLoginTimeValue(uint loginTime);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class LoginHistoryDaoBase;
    friend class LoginHistoryDao;

    QSharedDataPointer<LoginHistoryPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::LoginHistory)
Q_DECLARE_METATYPE(ns::User::LoginHistoryList)

#endif // NS_USER_LOGINHISTORY_H
