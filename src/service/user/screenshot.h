// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_SCREENSHOT_H
#define NS_USER_SCREENSHOT_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "uploadstatus.h"
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

class Screenshot;
class ScreenshotPrivate;
typedef QList<Screenshot> ScreenshotList;

class BUILDSYS_SERVICE_DECL Screenshot
{
public:
    Screenshot();
    Screenshot(const Screenshot& rhs);
    explicit Screenshot(qlonglong id);
    explicit Screenshot(const QSqlQuery& query);
    explicit Screenshot(const QSqlRecord& record);
    ~Screenshot();

    Screenshot& operator=(const Screenshot& rhs);

    bool isValid() const;
    Screenshot identity() const;
    bool identityEqual(const Screenshot& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isOwnerNull() const;
    QString owner() const;
    void setOwnerNull();
    void setOwner(const QString& owner);

    bool isStatusNull() const;
    UploadStatus status() const;
    void setStatusNull();
    void setStatus(UploadStatus status);

    bool isMeetCdrIdNull() const;
    QString meetCdrId() const;
    void setMeetCdrIdNull();
    void setMeetCdrId(const QString& meetCdrId);

    bool isPhotoNull() const;
    QString photo() const;
    void setPhotoNull();
    void setPhoto(const QString& photo);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class ScreenshotDaoBase;
    friend class ScreenshotDao;

    QSharedDataPointer<ScreenshotPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::Screenshot)
Q_DECLARE_METATYPE(ns::User::ScreenshotList)

#endif // NS_USER_SCREENSHOT_H
