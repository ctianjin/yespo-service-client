// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIASTATUS_H
#define NS_MEDIA_MEDIASTATUS_H

#include <QtCore/QMetaType>
#include "serviceglobal.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Media {

enum MediaStatus
{
    NotAvailableStatus,
    WaitingStatus,
    DownloadingStatus,
    PausedStatus,
    ReadyStatus,
    ErrorStatus = 99
};

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns

Q_DECLARE_METATYPE(ns::Media::MediaStatus)

#endif // NS_MEDIA_MEDIASTATUS_H
