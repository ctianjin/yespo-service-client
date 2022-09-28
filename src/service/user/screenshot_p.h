// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_SCREENSHOT_P_H
#define NS_USER_SCREENSHOT_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class ScreenshotPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_owner;
    QVariant m_status;
    QVariant m_meetCdrId;
    QVariant m_photo;

    ScreenshotPrivate();
    ScreenshotPrivate(const ScreenshotPrivate& rhs);
    explicit ScreenshotPrivate(qlonglong id);
    explicit ScreenshotPrivate(const QSqlQuery& query);
    explicit ScreenshotPrivate(const QSqlRecord& record);
    ~ScreenshotPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_SCREENSHOT_P_H
