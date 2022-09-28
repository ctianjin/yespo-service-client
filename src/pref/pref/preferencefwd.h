// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_PREFERENCEFWD_H
#define NS_PREF_PREFERENCEFWD_H

#include <QtCore/QtGlobal>
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {
namespace Pref {

class PreferenceDatabase;

class DefaultPreference;
class DefaultPreferenceDao;
typedef QList<DefaultPreference> DefaultPreferenceList;

class UserPreference;
class UserPreferenceDao;
typedef QList<UserPreference> UserPreferenceList;

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_PREFERENCEFWD_H
