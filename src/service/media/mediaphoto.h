// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIAPHOTO_H
#define NS_MEDIA_MEDIAPHOTO_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "mediastatus.h"
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
namespace Media {

class MediaPhoto;
class MediaPhotoPrivate;
typedef QList<MediaPhoto> MediaPhotoList;

class BUILDSYS_SERVICE_DECL MediaPhoto
{
public:
    MediaPhoto();
    MediaPhoto(const MediaPhoto& rhs);
    explicit MediaPhoto(qlonglong id);
    explicit MediaPhoto(const QSqlQuery& query);
    explicit MediaPhoto(const QSqlRecord& record);
    ~MediaPhoto();

    MediaPhoto& operator=(const MediaPhoto& rhs);

    bool isValid() const;
    MediaPhoto identity() const;
    bool identityEqual(const MediaPhoto& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isDownloadUrlNull() const;
    QString downloadUrl() const;
    void setDownloadUrlNull();
    void setDownloadUrl(const QString& downloadUrl);

    bool isStatusNull() const;
    MediaStatus status() const;
    void setStatusNull();
    void setStatus(MediaStatus status);

    bool isTotalSizeNull() const;
    QString totalSize() const;
    void setTotalSizeNull();
    void setTotalSize(const QString& totalSize);

    bool isAvailableSizeNull() const;
    QString availableSize() const;
    void setAvailableSizeNull();
    void setAvailableSize(const QString& availableSize);

    bool isFilenameNull() const;
    QString filename() const;
    void setFilenameNull();
    void setFilename(const QString& filename);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class MediaPhotoDaoBase;
    friend class MediaPhotoDao;

    QSharedDataPointer<MediaPhotoPrivate> d;
};

} // namespace ns::Media
} // namespace ns

Q_DECLARE_METATYPE(ns::Media::MediaPhoto)
Q_DECLARE_METATYPE(ns::Media::MediaPhotoList)

#endif // NS_MEDIA_MEDIAPHOTO_H
