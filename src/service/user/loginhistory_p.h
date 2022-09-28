// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_LOGINHISTORY_P_H
#define NS_USER_LOGINHISTORY_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class LoginHistoryPrivate : public QSharedData
{
public:
    QVariant m_userName;
    QVariant m_password;
    QVariant m_rememberPassword;
    QVariant m_loginTime;

    LoginHistoryPrivate();
    LoginHistoryPrivate(const LoginHistoryPrivate& rhs);
    explicit LoginHistoryPrivate(const QString& userName);
    explicit LoginHistoryPrivate(const QSqlQuery& query);
    explicit LoginHistoryPrivate(const QSqlRecord& record);
    ~LoginHistoryPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_LOGINHISTORY_P_H
