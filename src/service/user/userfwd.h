// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_USERFWD_H
#define NS_USER_USERFWD_H

#include <QtCore/QtGlobal>
#include "calltype.h"
#include "uploadstatus.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {
namespace User {

class UserDatabase;

class LoginHistory;
class LoginHistoryDao;
typedef QList<LoginHistory> LoginHistoryList;

class CallRecord;
class CallRecordDao;
typedef QList<CallRecord> CallRecordList;

class Screenshot;
class ScreenshotDao;
typedef QList<Screenshot> ScreenshotList;

class Blacklist;
class BlacklistDao;
typedef QList<Blacklist> BlacklistList;

class Contact;
class ContactDao;
typedef QList<Contact> ContactList;

class PhoneHistory;
class PhoneHistoryDao;
typedef QList<PhoneHistory> PhoneHistoryList;

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_USERFWD_H
