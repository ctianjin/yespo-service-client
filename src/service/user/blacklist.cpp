// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "blacklist.h"
#include "blacklist_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

BlacklistPrivate::BlacklistPrivate() :
    QSharedData()
{
}

BlacklistPrivate::BlacklistPrivate(const BlacklistPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_owner(rhs.m_owner),
    m_uniqueid(rhs.m_uniqueid),
    m_blacklistId(rhs.m_blacklistId),
    m_partnerSysId(rhs.m_partnerSysId),
    m_veSysId(rhs.m_veSysId),
    m_displayName(rhs.m_displayName),
    m_smallPhoto(rhs.m_smallPhoto)
{
}

BlacklistPrivate::BlacklistPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

BlacklistPrivate::BlacklistPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_owner(query.value(1)),
    m_uniqueid(query.value(2)),
    m_blacklistId(query.value(3)),
    m_partnerSysId(query.value(4)),
    m_veSysId(query.value(5)),
    m_displayName(query.value(6)),
    m_smallPhoto(query.value(7))
{
}

BlacklistPrivate::BlacklistPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_owner(record.value(QLatin1String("owner"))),
    m_uniqueid(record.value(QLatin1String("uniqueId"))),
    m_blacklistId(record.value(QLatin1String("blacklist_id"))),
    m_partnerSysId(record.value(QLatin1String("partner_sys_id"))),
    m_veSysId(record.value(QLatin1String("ve_sys_id"))),
    m_displayName(record.value(QLatin1String("display_name"))),
    m_smallPhoto(record.value(QLatin1String("small_photo")))
{
}

BlacklistPrivate::~BlacklistPrivate()
{
}

Blacklist::Blacklist() :
    d(new BlacklistPrivate)
{
}

Blacklist::Blacklist(const Blacklist& rhs) :
    d(rhs.d)
{
}

Blacklist::Blacklist(qlonglong id) :
    d(new BlacklistPrivate(id))
{
}

Blacklist::Blacklist(const QSqlQuery& query) :
    d(new BlacklistPrivate(query))
{
}

Blacklist::Blacklist(const QSqlRecord& record) :
    d(new BlacklistPrivate(record))
{
}

Blacklist::~Blacklist()
{
}

Blacklist& Blacklist::operator=(const Blacklist& rhs)
{
    d = rhs.d;
    return *this;
}

bool Blacklist::isValid() const
{
    return d->m_id.isValid();
}

Blacklist Blacklist::identity() const
{
    Blacklist entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool Blacklist::identityEqual(const Blacklist& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool Blacklist::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong Blacklist::id() const
{
    return d->m_id.toLongLong();
}

void Blacklist::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void Blacklist::setId(qlonglong id)
{
    d->m_id = id;
}

bool Blacklist::isOwnerNull() const
{
    return d->m_owner.isNull();
}

QString Blacklist::owner() const
{
    return d->m_owner.toString();
}

void Blacklist::setOwnerNull()
{
    d->m_owner = QVariant(QVariant::String);
}

void Blacklist::setOwner(const QString& owner)
{
    d->m_owner = owner;
}

bool Blacklist::isUniqueidNull() const
{
    return d->m_uniqueid.isNull();
}

QString Blacklist::uniqueid() const
{
    return d->m_uniqueid.toString();
}

void Blacklist::setUniqueidNull()
{
    d->m_uniqueid = QVariant(QVariant::String);
}

void Blacklist::setUniqueid(const QString& uniqueid)
{
    d->m_uniqueid = uniqueid;
}

bool Blacklist::isBlacklistIdNull() const
{
    return d->m_blacklistId.isNull();
}

QString Blacklist::blacklistId() const
{
    return d->m_blacklistId.toString();
}

void Blacklist::setBlacklistIdNull()
{
    d->m_blacklistId = QVariant(QVariant::String);
}

void Blacklist::setBlacklistId(const QString& blacklistId)
{
    d->m_blacklistId = blacklistId;
}

bool Blacklist::isPartnerSysIdNull() const
{
    return d->m_partnerSysId.isNull();
}

QString Blacklist::partnerSysId() const
{
    return d->m_partnerSysId.toString();
}

void Blacklist::setPartnerSysIdNull()
{
    d->m_partnerSysId = QVariant(QVariant::String);
}

void Blacklist::setPartnerSysId(const QString& partnerSysId)
{
    d->m_partnerSysId = partnerSysId;
}

bool Blacklist::isVeSysIdNull() const
{
    return d->m_veSysId.isNull();
}

QString Blacklist::veSysId() const
{
    return d->m_veSysId.toString();
}

void Blacklist::setVeSysIdNull()
{
    d->m_veSysId = QVariant(QVariant::String);
}

void Blacklist::setVeSysId(const QString& veSysId)
{
    d->m_veSysId = veSysId;
}

bool Blacklist::isDisplayNameNull() const
{
    return d->m_displayName.isNull();
}

QString Blacklist::displayName() const
{
    return d->m_displayName.toString();
}

void Blacklist::setDisplayNameNull()
{
    d->m_displayName = QVariant(QVariant::String);
}

void Blacklist::setDisplayName(const QString& displayName)
{
    d->m_displayName = displayName;
}

bool Blacklist::isSmallPhotoNull() const
{
    return d->m_smallPhoto.isNull();
}

QString Blacklist::smallPhoto() const
{
    return d->m_smallPhoto.toString();
}

void Blacklist::setSmallPhotoNull()
{
    d->m_smallPhoto = QVariant(QVariant::String);
}

void Blacklist::setSmallPhoto(const QString& smallPhoto)
{
    d->m_smallPhoto = smallPhoto;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
