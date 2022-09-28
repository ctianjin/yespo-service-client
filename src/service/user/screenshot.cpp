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
#include "screenshot.h"
#include "screenshot_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

ScreenshotPrivate::ScreenshotPrivate() :
    QSharedData()
{
}

ScreenshotPrivate::ScreenshotPrivate(const ScreenshotPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_owner(rhs.m_owner),
    m_status(rhs.m_status),
    m_meetCdrId(rhs.m_meetCdrId),
    m_photo(rhs.m_photo)
{
}

ScreenshotPrivate::ScreenshotPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

ScreenshotPrivate::ScreenshotPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_owner(query.value(1)),
    m_status(query.value(2)),
    m_meetCdrId(query.value(3)),
    m_photo(query.value(4))
{
}

ScreenshotPrivate::ScreenshotPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_owner(record.value(QLatin1String("owner"))),
    m_status(record.value(QLatin1String("status"))),
    m_meetCdrId(record.value(QLatin1String("meet_cdr_id"))),
    m_photo(record.value(QLatin1String("photo")))
{
}

ScreenshotPrivate::~ScreenshotPrivate()
{
}

Screenshot::Screenshot() :
    d(new ScreenshotPrivate)
{
}

Screenshot::Screenshot(const Screenshot& rhs) :
    d(rhs.d)
{
}

Screenshot::Screenshot(qlonglong id) :
    d(new ScreenshotPrivate(id))
{
}

Screenshot::Screenshot(const QSqlQuery& query) :
    d(new ScreenshotPrivate(query))
{
}

Screenshot::Screenshot(const QSqlRecord& record) :
    d(new ScreenshotPrivate(record))
{
}

Screenshot::~Screenshot()
{
}

Screenshot& Screenshot::operator=(const Screenshot& rhs)
{
    d = rhs.d;
    return *this;
}

bool Screenshot::isValid() const
{
    return d->m_id.isValid();
}

Screenshot Screenshot::identity() const
{
    Screenshot entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool Screenshot::identityEqual(const Screenshot& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool Screenshot::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong Screenshot::id() const
{
    return d->m_id.toLongLong();
}

void Screenshot::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void Screenshot::setId(qlonglong id)
{
    d->m_id = id;
}

bool Screenshot::isOwnerNull() const
{
    return d->m_owner.isNull();
}

QString Screenshot::owner() const
{
    return d->m_owner.toString();
}

void Screenshot::setOwnerNull()
{
    d->m_owner = QVariant(QVariant::String);
}

void Screenshot::setOwner(const QString& owner)
{
    d->m_owner = owner;
}

bool Screenshot::isStatusNull() const
{
    return d->m_status.isNull();
}

UploadStatus Screenshot::status() const
{
    return static_cast<UploadStatus>(d->m_status.toInt());
}

void Screenshot::setStatusNull()
{
    d->m_status = QVariant(QVariant::Int);
}

void Screenshot::setStatus(UploadStatus status)
{
    d->m_status = static_cast<int>(status);
}

bool Screenshot::isMeetCdrIdNull() const
{
    return d->m_meetCdrId.isNull();
}

QString Screenshot::meetCdrId() const
{
    return d->m_meetCdrId.toString();
}

void Screenshot::setMeetCdrIdNull()
{
    d->m_meetCdrId = QVariant(QVariant::String);
}

void Screenshot::setMeetCdrId(const QString& meetCdrId)
{
    d->m_meetCdrId = meetCdrId;
}

bool Screenshot::isPhotoNull() const
{
    return d->m_photo.isNull();
}

QString Screenshot::photo() const
{
    return d->m_photo.toString();
}

void Screenshot::setPhotoNull()
{
    d->m_photo = QVariant(QVariant::String);
}

void Screenshot::setPhoto(const QString& photo)
{
    d->m_photo = photo;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
