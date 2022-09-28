#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QRegExp>
#include <QIcon>
#include <QDateTime>
#include <QFontMetricsF>
#include "contactsitems_p.h"

namespace Qpid {

// ContactItemBase

const int ContactItemBase::statusColumnWidth = 21;

ContactItemBase::ContactItemBase(QTreeWidget* view, ItemType type) :
    QTreeWidgetItem(view, type)
{
}

ContactItemBase::ContactItemBase(const ContactItemBase& rhs) :
    QTreeWidgetItem(rhs)
{
}

ContactItemBase::~ContactItemBase()
{
}

ContactItemBase& ContactItemBase::operator=(const ContactItemBase& rhs)
{
    QTreeWidgetItem::operator=(rhs);

    return *this;
}

bool ContactItemBase::operator<(const QTreeWidgetItem& rhs) const
{
    // Compare the item type first
    if (type() != rhs.type())
        return type() < rhs.type();

    // Now for the same type
    int n = compare(rhs);
    if (n == 0) {
        return QTreeWidgetItem::operator<(rhs);
    } else {
        return n < 0;
    }
}

void ContactItemBase::updateItem()
{
//	emitDataChanged();
}

int ContactItemBase::compare(const QTreeWidgetItem& rhs) const
{
    return 0;
}

// ContactItem
ContactItem::ContactItem(QTreeWidget* view, const User::Contact& contact) :
    ContactItemBase(view, ContactItemType),
	m_contact(contact),
	m_buddyID(-1)
{
}

ContactItem::~ContactItem()
{
}

int ContactItem::compare(const QTreeWidgetItem& rhs) const
{
    Q_ASSERT(type() == rhs.type());

	const ContactItem& other = static_cast<const ContactItem&>(rhs);
	if (m_contact.addBookId() == QLatin1String("-1"))
		return 1;

	QString labelText = m_contact.contactName();
	if (labelText.isEmpty())
		labelText = m_contact.voiceraId();
	QString otherLabelText = other.contact().contactName();
	if (otherLabelText.isEmpty())
		otherLabelText = other.contact().voiceraId();

	int ret = (labelText==otherLabelText?0:(labelText>otherLabelText?-1:1));
	return ret;
}

User::Contact ContactItem::contact() const
{
	return m_contact;
}

void ContactItem::setContact(const User::Contact& contact)
{
	m_contact = contact;
}

QVariant ContactItem::data(int column, int role) const
{
    QVariant ret;
     switch (column) {
		 case PhotoColumn:
			 ret = photoData(role);
			 break;
		 case StatusColumn:
			 ret = statusData(role);
			 break;
         case LabelColumn:
             ret = labelData(role);
             break;
         default:
             break;
     }

    return ret;
}

void ContactItem::setPhoto(const QString& fileName)
{
	m_contact.setPhotofile(fileName);
	emitDataChanged();
}

void ContactItem::updateContactPhone(const QString& oldPhone, const QString& phoneCountry, const QString& phone)
{
	if (oldPhone.isEmpty()) {  //add 

		QString newPhone = m_contact.phone();
		if (!newPhone.isEmpty())
			newPhone.append(QLatin1Char(','));
		newPhone.append(phoneCountry+QLatin1Char('|')+phone);
		m_contact.setPhone(newPhone);

		return;
	}

	//update
	QStringList phoneList = m_contact.phone().split(QLatin1Char(','));
	QStringList newPhoneList;
	foreach (const QString& ph, phoneList) {
		if (ph.contains(oldPhone))
			newPhoneList << (phoneCountry+QLatin1Char('|')+phone);
		else
			newPhoneList << ph;
	}
	m_contact.setPhone(newPhoneList.join(QLatin1String(",")));
}
	

void ContactItem::deleteContactPhone(const QString& delPhone)
{
	QStringList phoneList = m_contact.phone().split(QLatin1Char(','));
	QStringList newPhoneList;
	foreach (const QString& ph, phoneList) {
		if (ph.contains(delPhone))
			continue;
		else
			newPhoneList << ph;
	}
	m_contact.setPhone(newPhoneList.join(QLatin1String(",")));
}

void ContactItem::updateInterpretationLanguage(const QString& source, const QString& target)
{
	m_contact.setSourceLanguage(source);
	m_contact.setTargetLanguage(target);
}

int ContactItem::buddyID() const
{
	return m_buddyID;
}

void ContactItem::setBuddyID(int buddyID)
{
	m_buddyID = buddyID;
}

void ContactItem::setOnlineStatus(const QString& status)
{
	m_contact.setOnlineStatus(status);
	emitDataChanged();
}

QVariant ContactItem::photoData(int role) const
{
	if (role == Qt::DecorationRole) {
		QImage statusIcon;
		if (m_contact.addBookId()==QLatin1String("-1")) {
			statusIcon.load(QLatin1String(":/images/ui/call_live_interpreter.jpg"));
		} else {
			if (m_contact.photofile().isEmpty())
				statusIcon.load(QLatin1String(":/images/ui/default_photo.jpg"));
			else
				statusIcon.load(m_contact.photofile());
		}
		return statusIcon.scaled(QSize(32,32));
	} 

	return QVariant();
}

QVariant ContactItem::statusData(int role) const
{
	if (role == Qt::DecorationRole) {
		QIcon statusIcon;
		if (isPhone()) {
			statusIcon.addFile(QLatin1String(":/images/ui/online_status_phone.png"));
		} else {
			if (isOnline())
				statusIcon.addFile(QLatin1String(":/images/ui/online_status_online.png"));
			else
				statusIcon.addFile(QLatin1String(":/images/ui/online_status_offline.png"));
		}
		return statusIcon;
	}  else if (role == Qt::SizeHintRole) {
		return QSize(13,13);
	} 

	return QVariant();
}

QVariant ContactItem::labelData(int role) const
{
     if (role == Qt::DisplayRole) {
		 if (!m_contact.contactName().isEmpty())
			 return m_contact.contactName();
		return m_contact.voiceraId();
	 } 

    return QVariant();
}

bool ContactItem::isOnline() const
{
	return (m_contact.onlineStatus()==QLatin1String("1"));
}

bool ContactItem::isOffline() const
{
	return (m_contact.onlineStatus()==QLatin1String("2"));
}

bool ContactItem::isPhone() const
{
	return (m_contact.voiceraId().isEmpty());
}

} // namespace Qpid
