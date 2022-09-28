// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIAPHOTO_P_H
#define NS_MEDIA_MEDIAPHOTO_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace Media {

class MediaPhotoPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_downloadUrl;
    QVariant m_status;
    QVariant m_totalSize;
    QVariant m_availableSize;
    QVariant m_filename;

    MediaPhotoPrivate();
    MediaPhotoPrivate(const MediaPhotoPrivate& rhs);
    explicit MediaPhotoPrivate(qlonglong id);
    explicit MediaPhotoPrivate(const QSqlQuery& query);
    explicit MediaPhotoPrivate(const QSqlRecord& record);
    ~MediaPhotoPrivate();
};

} // namespace ns::Media
} // namespace ns

#endif // NS_MEDIA_MEDIAPHOTO_P_H
