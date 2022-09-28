// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_UPLOADSTATUS_H
#define NS_USER_UPLOADSTATUS_H

#include <QtCore/QMetaType>
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

enum UploadStatus
{
    Idle,
    Uploading,
    Uploaded,
    Unknown = 99
};

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::UploadStatus)

#endif // NS_USER_UPLOADSTATUS_H
