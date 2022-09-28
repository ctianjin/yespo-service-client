// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "mediaphoto.h"
#include "mediaphoto_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Media {

MediaPhotoPrivate::MediaPhotoPrivate() :
    QSharedData()
{
}

MediaPhotoPrivate::MediaPhotoPrivate(const MediaPhotoPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_downloadUrl(rhs.m_downloadUrl),
    m_status(rhs.m_status),
    m_totalSize(rhs.m_totalSize),
    m_availableSize(rhs.m_availableSize),
    m_filename(rhs.m_filename)
{
}

MediaPhotoPrivate::MediaPhotoPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

MediaPhotoPrivate::MediaPhotoPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_downloadUrl(query.value(1)),
    m_status(query.value(2)),
    m_totalSize(query.value(3)),
    m_availableSize(query.value(4)),
    m_filename(query.value(5))
{
}

MediaPhotoPrivate::MediaPhotoPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_downloadUrl(record.value(QLatin1String("download_url"))),
    m_status(record.value(QLatin1String("status"))),
    m_totalSize(record.value(QLatin1String("total_size"))),
    m_availableSize(record.value(QLatin1String("available_size"))),
    m_filename(record.value(QLatin1String("fileName")))
{
}

MediaPhotoPrivate::~MediaPhotoPrivate()
{
}

MediaPhoto::MediaPhoto() :
    d(new MediaPhotoPrivate)
{
}

MediaPhoto::MediaPhoto(const MediaPhoto& rhs) :
    d(rhs.d)
{
}

MediaPhoto::MediaPhoto(qlonglong id) :
    d(new MediaPhotoPrivate(id))
{
}

MediaPhoto::MediaPhoto(const QSqlQuery& query) :
    d(new MediaPhotoPrivate(query))
{
}

MediaPhoto::MediaPhoto(const QSqlRecord& record) :
    d(new MediaPhotoPrivate(record))
{
}

MediaPhoto::~MediaPhoto()
{
}

MediaPhoto& MediaPhoto::operator=(const MediaPhoto& rhs)
{
    d = rhs.d;
    return *this;
}

bool MediaPhoto::isValid() const
{
    return d->m_id.isValid();
}

MediaPhoto MediaPhoto::identity() const
{
    MediaPhoto entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool MediaPhoto::identityEqual(const MediaPhoto& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool MediaPhoto::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong MediaPhoto::id() const
{
    return d->m_id.toLongLong();
}

void MediaPhoto::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void MediaPhoto::setId(qlonglong id)
{
    d->m_id = id;
}

bool MediaPhoto::isDownloadUrlNull() const
{
    return d->m_downloadUrl.isNull();
}

QString MediaPhoto::downloadUrl() const
{
    return d->m_downloadUrl.toString();
}

void MediaPhoto::setDownloadUrlNull()
{
    d->m_downloadUrl = QVariant(QVariant::String);
}

void MediaPhoto::setDownloadUrl(const QString& downloadUrl)
{
    d->m_downloadUrl = downloadUrl;
}

bool MediaPhoto::isStatusNull() const
{
    return d->m_status.isNull();
}

MediaStatus MediaPhoto::status() const
{
    return static_cast<MediaStatus>(d->m_status.toInt());
}

void MediaPhoto::setStatusNull()
{
    d->m_status = QVariant(QVariant::Int);
}

void MediaPhoto::setStatus(MediaStatus status)
{
    d->m_status = static_cast<int>(status);
}

bool MediaPhoto::isTotalSizeNull() const
{
    return d->m_totalSize.isNull();
}

QString MediaPhoto::totalSize() const
{
    return d->m_totalSize.toString();
}

void MediaPhoto::setTotalSizeNull()
{
    d->m_totalSize = QVariant(QVariant::String);
}

void MediaPhoto::setTotalSize(const QString& totalSize)
{
    d->m_totalSize = totalSize;
}

bool MediaPhoto::isAvailableSizeNull() const
{
    return d->m_availableSize.isNull();
}

QString MediaPhoto::availableSize() const
{
    return d->m_availableSize.toString();
}

void MediaPhoto::setAvailableSizeNull()
{
    d->m_availableSize = QVariant(QVariant::String);
}

void MediaPhoto::setAvailableSize(const QString& availableSize)
{
    d->m_availableSize = availableSize;
}

bool MediaPhoto::isFilenameNull() const
{
    return d->m_filename.isNull();
}

QString MediaPhoto::filename() const
{
    return d->m_filename.toString();
}

void MediaPhoto::setFilenameNull()
{
    d->m_filename = QVariant(QVariant::String);
}

void MediaPhoto::setFilename(const QString& filename)
{
    d->m_filename = filename;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns
