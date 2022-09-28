// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CONTACT_P_H
#define NS_USER_CONTACT_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace User {

class ContactPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariant m_userName;
    QVariant m_cid;
    QVariant m_addBookId;
    QVariant m_contactName;
    QVariant m_smallPhoto;
    QVariant m_largePhoto;
    QVariant m_phone;
    QVariant m_voiceraId;
    QVariant m_sipCallerId;
    QVariant m_sourceLanguage;
    QVariant m_targetLanguage;
    QVariant m_onlineStatus;
    QVariant m_systemUser;
    QVariant m_photofile;
    QVariant m_totalSize;
    QVariant m_availableSize;

    ContactPrivate();
    ContactPrivate(const ContactPrivate& rhs);
    explicit ContactPrivate(qlonglong id);
    explicit ContactPrivate(const QSqlQuery& query);
    explicit ContactPrivate(const QSqlRecord& record);
    ~ContactPrivate();
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_CONTACT_P_H
