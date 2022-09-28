// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_BLACKLIST_H
#define NS_USER_BLACKLIST_H

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

class Blacklist;
class BlacklistPrivate;
typedef QList<Blacklist> BlacklistList;

class BUILDSYS_SERVICE_DECL Blacklist
{
public:
    Blacklist();
    Blacklist(const Blacklist& rhs);
    explicit Blacklist(qlonglong id);
    explicit Blacklist(const QSqlQuery& query);
    explicit Blacklist(const QSqlRecord& record);
    ~Blacklist();

    Blacklist& operator=(const Blacklist& rhs);

    bool isValid() const;
    Blacklist identity() const;
    bool identityEqual(const Blacklist& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isOwnerNull() const;
    QString owner() const;
    void setOwnerNull();
    void setOwner(const QString& owner);

    bool isUniqueidNull() const;
    QString uniqueid() const;
    void setUniqueidNull();
    void setUniqueid(const QString& uniqueid);

    bool isBlacklistIdNull() const;
    QString blacklistId() const;
    void setBlacklistIdNull();
    void setBlacklistId(const QString& blacklistId);

    bool isPartnerSysIdNull() const;
    QString partnerSysId() const;
    void setPartnerSysIdNull();
    void setPartnerSysId(const QString& partnerSysId);

    bool isVeSysIdNull() const;
    QString veSysId() const;
    void setVeSysIdNull();
    void setVeSysId(const QString& veSysId);

    bool isDisplayNameNull() const;
    QString displayName() const;
    void setDisplayNameNull();
    void setDisplayName(const QString& displayName);

    bool isSmallPhotoNull() const;
    QString smallPhoto() const;
    void setSmallPhotoNull();
    void setSmallPhoto(const QString& smallPhoto);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class BlacklistDaoBase;
    friend class BlacklistDao;

    QSharedDataPointer<BlacklistPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::Blacklist)
Q_DECLARE_METATYPE(ns::User::BlacklistList)

#endif // NS_USER_BLACKLIST_H
