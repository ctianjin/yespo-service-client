// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CONTACT_H
#define NS_USER_CONTACT_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
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

class Contact;
class ContactPrivate;
typedef QList<Contact> ContactList;

class BUILDSYS_SERVICE_DECL Contact
{
public:
    Contact();
    Contact(const Contact& rhs);
    explicit Contact(qlonglong id);
    explicit Contact(const QSqlQuery& query);
    explicit Contact(const QSqlRecord& record);
    ~Contact();

    Contact& operator=(const Contact& rhs);

    bool isValid() const;
    Contact identity() const;
    bool identityEqual(const Contact& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    bool isUserNameNull() const;
    QString userName() const;
    void setUserNameNull();
    void setUserName(const QString& userName);

    bool isCidNull() const;
    QString cid() const;
    void setCidNull();
    void setCid(const QString& cid);

    bool isAddBookIdNull() const;
    QString addBookId() const;
    void setAddBookIdNull();
    void setAddBookId(const QString& addBookId);

    bool isContactNameNull() const;
    QString contactName() const;
    void setContactNameNull();
    void setContactName(const QString& contactName);

    bool isSmallPhotoNull() const;
    QString smallPhoto() const;
    void setSmallPhotoNull();
    void setSmallPhoto(const QString& smallPhoto);

    bool isLargePhotoNull() const;
    QString largePhoto() const;
    void setLargePhotoNull();
    void setLargePhoto(const QString& largePhoto);

    bool isPhoneNull() const;
    QString phone() const;
    void setPhoneNull();
    void setPhone(const QString& phone);

    bool isVoiceraIdNull() const;
    QString voiceraId() const;
    void setVoiceraIdNull();
    void setVoiceraId(const QString& voiceraId);

    bool isSipCallerIdNull() const;
    QString sipCallerId() const;
    void setSipCallerIdNull();
    void setSipCallerId(const QString& sipCallerId);

    bool isSourceLanguageNull() const;
    QString sourceLanguage() const;
    void setSourceLanguageNull();
    void setSourceLanguage(const QString& sourceLanguage);

    bool isTargetLanguageNull() const;
    QString targetLanguage() const;
    void setTargetLanguageNull();
    void setTargetLanguage(const QString& targetLanguage);

    bool isOnlineStatusNull() const;
    QString onlineStatus() const;
    void setOnlineStatusNull();
    void setOnlineStatus(const QString& onlineStatus);

    bool isSystemUserNull() const;
    QString systemUser() const;
    void setSystemUserNull();
    void setSystemUser(const QString& systemUser);

    bool isPhotofileNull() const;
    QString photofile() const;
    void setPhotofileNull();
    void setPhotofile(const QString& photofile);

    bool isTotalSizeNull() const;
    QString totalSize() const;
    void setTotalSizeNull();
    void setTotalSize(const QString& totalSize);

    bool isAvailableSizeNull() const;
    QString availableSize() const;
    void setAvailableSizeNull();
    void setAvailableSize(const QString& availableSize);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class ContactDaoBase;
    friend class ContactDao;

    QSharedDataPointer<ContactPrivate> d;
};

} // namespace ns::User
} // namespace ns

Q_DECLARE_METATYPE(ns::User::Contact)
Q_DECLARE_METATYPE(ns::User::ContactList)

#endif // NS_USER_CONTACT_H
