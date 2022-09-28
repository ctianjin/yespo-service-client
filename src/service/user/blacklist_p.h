// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_BLACKLIST_P_H
#define NS_USER_BLACKLIST_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class BlacklistPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_owner;
    QVariant m_uniqueid;
    QVariant m_blacklistId;
    QVariant m_partnerSysId;
    QVariant m_veSysId;
    QVariant m_displayName;
    QVariant m_smallPhoto;

    BlacklistPrivate();
    BlacklistPrivate(const BlacklistPrivate& rhs);
    explicit BlacklistPrivate(qlonglong id);
    explicit BlacklistPrivate(const QSqlQuery& query);
    explicit BlacklistPrivate(const QSqlRecord& record);
    ~BlacklistPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_BLACKLIST_P_H
