// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "contact.h"
#include "contact_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

ContactPrivate::ContactPrivate() :
    QSharedData()
{
}

ContactPrivate::ContactPrivate(const ContactPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_userName(rhs.m_userName),
    m_cid(rhs.m_cid),
    m_addBookId(rhs.m_addBookId),
    m_contactName(rhs.m_contactName),
    m_smallPhoto(rhs.m_smallPhoto),
    m_largePhoto(rhs.m_largePhoto),
    m_phone(rhs.m_phone),
    m_voiceraId(rhs.m_voiceraId),
    m_sipCallerId(rhs.m_sipCallerId),
    m_sourceLanguage(rhs.m_sourceLanguage),
    m_targetLanguage(rhs.m_targetLanguage),
    m_onlineStatus(rhs.m_onlineStatus),
    m_systemUser(rhs.m_systemUser),
    m_photofile(rhs.m_photofile),
    m_totalSize(rhs.m_totalSize),
    m_availableSize(rhs.m_availableSize)
{
}

ContactPrivate::ContactPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

ContactPrivate::ContactPrivate(const QSqlQuery& query) :
    QSharedData(),
    m_id(query.value(0)),
    m_userName(query.value(1)),
    m_cid(query.value(2)),
    m_addBookId(query.value(3)),
    m_contactName(query.value(4)),
    m_smallPhoto(query.value(5)),
    m_largePhoto(query.value(6)),
    m_phone(query.value(7)),
    m_voiceraId(query.value(8)),
    m_sipCallerId(query.value(9)),
    m_sourceLanguage(query.value(10)),
    m_targetLanguage(query.value(11)),
    m_onlineStatus(query.value(12)),
    m_systemUser(query.value(13)),
    m_photofile(query.value(14)),
    m_totalSize(query.value(15)),
    m_availableSize(query.value(16))
{
}

ContactPrivate::ContactPrivate(const QSqlRecord& record) :
    QSharedData(),
    m_id(record.value(QLatin1String("id"))),
    m_userName(record.value(QLatin1String("user_name"))),
    m_cid(record.value(QLatin1String("cid"))),
    m_addBookId(record.value(QLatin1String("add_book_id"))),
    m_contactName(record.value(QLatin1String("contact_name"))),
    m_smallPhoto(record.value(QLatin1String("small_photo"))),
    m_largePhoto(record.value(QLatin1String("large_photo"))),
    m_phone(record.value(QLatin1String("phone"))),
    m_voiceraId(record.value(QLatin1String("voicera_id"))),
    m_sipCallerId(record.value(QLatin1String("sip_caller_id"))),
    m_sourceLanguage(record.value(QLatin1String("source_language"))),
    m_targetLanguage(record.value(QLatin1String("target_language"))),
    m_onlineStatus(record.value(QLatin1String("online_status"))),
    m_systemUser(record.value(QLatin1String("system_user"))),
    m_photofile(record.value(QLatin1String("photoFile"))),
    m_totalSize(record.value(QLatin1String("total_size"))),
    m_availableSize(record.value(QLatin1String("available_size")))
{
}

ContactPrivate::~ContactPrivate()
{
}

Contact::Contact() :
    d(new ContactPrivate)
{
}

Contact::Contact(const Contact& rhs) :
    d(rhs.d)
{
}

Contact::Contact(qlonglong id) :
    d(new ContactPrivate(id))
{
}

Contact::Contact(const QSqlQuery& query) :
    d(new ContactPrivate(query))
{
}

Contact::Contact(const QSqlRecord& record) :
    d(new ContactPrivate(record))
{
}

Contact::~Contact()
{
}

Contact& Contact::operator=(const Contact& rhs)
{
    d = rhs.d;
    return *this;
}

bool Contact::isValid() const
{
    return d->m_id.isValid();
}

Contact Contact::identity() const
{
    Contact entity;
    entity.d->m_id = d->m_id;
    return entity;
}

bool Contact::identityEqual(const Contact& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool Contact::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong Contact::id() const
{
    return d->m_id.toLongLong();
}

void Contact::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void Contact::setId(qlonglong id)
{
    d->m_id = id;
}

bool Contact::isUserNameNull() const
{
    return d->m_userName.isNull();
}

QString Contact::userName() const
{
    return d->m_userName.toString();
}

void Contact::setUserNameNull()
{
    d->m_userName = QVariant(QVariant::String);
}

void Contact::setUserName(const QString& userName)
{
    d->m_userName = userName;
}

bool Contact::isCidNull() const
{
    return d->m_cid.isNull();
}

QString Contact::cid() const
{
    return d->m_cid.toString();
}

void Contact::setCidNull()
{
    d->m_cid = QVariant(QVariant::String);
}

void Contact::setCid(const QString& cid)
{
    d->m_cid = cid;
}

bool Contact::isAddBookIdNull() const
{
    return d->m_addBookId.isNull();
}

QString Contact::addBookId() const
{
    return d->m_addBookId.toString();
}

void Contact::setAddBookIdNull()
{
    d->m_addBookId = QVariant(QVariant::String);
}

void Contact::setAddBookId(const QString& addBookId)
{
    d->m_addBookId = addBookId;
}

bool Contact::isContactNameNull() const
{
    return d->m_contactName.isNull();
}

QString Contact::contactName() const
{
    return d->m_contactName.toString();
}

void Contact::setContactNameNull()
{
    d->m_contactName = QVariant(QVariant::String);
}

void Contact::setContactName(const QString& contactName)
{
    d->m_contactName = contactName;
}

bool Contact::isSmallPhotoNull() const
{
    return d->m_smallPhoto.isNull();
}

QString Contact::smallPhoto() const
{
    return d->m_smallPhoto.toString();
}

void Contact::setSmallPhotoNull()
{
    d->m_smallPhoto = QVariant(QVariant::String);
}

void Contact::setSmallPhoto(const QString& smallPhoto)
{
    d->m_smallPhoto = smallPhoto;
}

bool Contact::isLargePhotoNull() const
{
    return d->m_largePhoto.isNull();
}

QString Contact::largePhoto() const
{
    return d->m_largePhoto.toString();
}

void Contact::setLargePhotoNull()
{
    d->m_largePhoto = QVariant(QVariant::String);
}

void Contact::setLargePhoto(const QString& largePhoto)
{
    d->m_largePhoto = largePhoto;
}

bool Contact::isPhoneNull() const
{
    return d->m_phone.isNull();
}

QString Contact::phone() const
{
    return d->m_phone.toString();
}

void Contact::setPhoneNull()
{
    d->m_phone = QVariant(QVariant::String);
}

void Contact::setPhone(const QString& phone)
{
    d->m_phone = phone;
}

bool Contact::isVoiceraIdNull() const
{
    return d->m_voiceraId.isNull();
}

QString Contact::voiceraId() const
{
    return d->m_voiceraId.toString();
}

void Contact::setVoiceraIdNull()
{
    d->m_voiceraId = QVariant(QVariant::String);
}

void Contact::setVoiceraId(const QString& voiceraId)
{
    d->m_voiceraId = voiceraId;
}

bool Contact::isSipCallerIdNull() const
{
    return d->m_sipCallerId.isNull();
}

QString Contact::sipCallerId() const
{
    return d->m_sipCallerId.toString();
}

void Contact::setSipCallerIdNull()
{
    d->m_sipCallerId = QVariant(QVariant::String);
}

void Contact::setSipCallerId(const QString& sipCallerId)
{
    d->m_sipCallerId = sipCallerId;
}

bool Contact::isSourceLanguageNull() const
{
    return d->m_sourceLanguage.isNull();
}

QString Contact::sourceLanguage() const
{
    return d->m_sourceLanguage.toString();
}

void Contact::setSourceLanguageNull()
{
    d->m_sourceLanguage = QVariant(QVariant::String);
}

void Contact::setSourceLanguage(const QString& sourceLanguage)
{
    d->m_sourceLanguage = sourceLanguage;
}

bool Contact::isTargetLanguageNull() const
{
    return d->m_targetLanguage.isNull();
}

QString Contact::targetLanguage() const
{
    return d->m_targetLanguage.toString();
}

void Contact::setTargetLanguageNull()
{
    d->m_targetLanguage = QVariant(QVariant::String);
}

void Contact::setTargetLanguage(const QString& targetLanguage)
{
    d->m_targetLanguage = targetLanguage;
}

bool Contact::isOnlineStatusNull() const
{
    return d->m_onlineStatus.isNull();
}

QString Contact::onlineStatus() const
{
    return d->m_onlineStatus.toString();
}

void Contact::setOnlineStatusNull()
{
    d->m_onlineStatus = QVariant(QVariant::String);
}

void Contact::setOnlineStatus(const QString& onlineStatus)
{
    d->m_onlineStatus = onlineStatus;
}

bool Contact::isSystemUserNull() const
{
    return d->m_systemUser.isNull();
}

QString Contact::systemUser() const
{
    return d->m_systemUser.toString();
}

void Contact::setSystemUserNull()
{
    d->m_systemUser = QVariant(QVariant::String);
}

void Contact::setSystemUser(const QString& systemUser)
{
    d->m_systemUser = systemUser;
}

bool Contact::isPhotofileNull() const
{
    return d->m_photofile.isNull();
}

QString Contact::photofile() const
{
    return d->m_photofile.toString();
}

void Contact::setPhotofileNull()
{
    d->m_photofile = QVariant(QVariant::String);
}

void Contact::setPhotofile(const QString& photofile)
{
    d->m_photofile = photofile;
}

bool Contact::isTotalSizeNull() const
{
    return d->m_totalSize.isNull();
}

QString Contact::totalSize() const
{
    return d->m_totalSize.toString();
}

void Contact::setTotalSizeNull()
{
    d->m_totalSize = QVariant(QVariant::String);
}

void Contact::setTotalSize(const QString& totalSize)
{
    d->m_totalSize = totalSize;
}

bool Contact::isAvailableSizeNull() const
{
    return d->m_availableSize.isNull();
}

QString Contact::availableSize() const
{
    return d->m_availableSize.toString();
}

void Contact::setAvailableSizeNull()
{
    d->m_availableSize = QVariant(QVariant::String);
}

void Contact::setAvailableSize(const QString& availableSize)
{
    d->m_availableSize = availableSize;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
