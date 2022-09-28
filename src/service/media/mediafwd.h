// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIAFWD_H
#define NS_MEDIA_MEDIAFWD_H

#include <QtCore/QtGlobal>
#include "mediastatus.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {
namespace Media {

class MediaDatabase;

class MediaPhoto;
class MediaPhotoDao;
typedef QList<MediaPhoto> MediaPhotoList;

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns

#endif // NS_MEDIA_MEDIAFWD_H
