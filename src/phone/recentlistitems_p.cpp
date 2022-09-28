#include <QtCore/QDebug>
#include <QDateTime>
#include "recentlistitems_p.h"
#include "phoneglobal.h"
#include "user/contact.h"
#include "userservice.h"

namespace Qpid {

// ItemBase
ItemBase::ItemBase(QTreeWidget* view, ItemType type) :
    QTreeWidgetItem(view, type)
{
}

ItemBase::ItemBase(const ItemBase& rhs) :
    QTreeWidgetItem(rhs)
{
}

ItemBase::~ItemBase()
{
}

ItemBase& ItemBase::operator=(const ItemBase& rhs)
{
    QTreeWidgetItem::operator=(rhs);

    return *this;
}

bool ItemBase::operator<(const QTreeWidgetItem& rhs) const
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

int ItemBase::compare(const QTreeWidgetItem& rhs) const
{
    return 0;
}

// PhoneHistoryItem
PhoneHistoryItem::PhoneHistoryItem(QTreeWidget* view, const User::PhoneHistory& phoneHistory) :
    ItemBase(view, PhoneHistoryItemType),
	m_phoneHistory(phoneHistory),
	m_userService(0)
{
	
}

PhoneHistoryItem::~PhoneHistoryItem()
{
}

void PhoneHistoryItem::setUserService(UserService* userService)
{
	m_userService = userService;
}

User::PhoneHistory PhoneHistoryItem::phoneHistory() const
{
	return m_phoneHistory;
}

void PhoneHistoryItem::setPhoneHistory(const User::PhoneHistory& phoneHistory)
{
	m_phoneHistory = phoneHistory;
}

int PhoneHistoryItem::compare(const QTreeWidgetItem& rhs) const
{
    Q_ASSERT(type() == rhs.type());

	const PhoneHistoryItem& other = static_cast<const PhoneHistoryItem&>(rhs);

	 QString callTime = m_phoneHistory.callTime();
	 QString otherCallTime = other.phoneHistory().callTime();

	 int ret = (callTime==otherCallTime ? 0 : (callTime > otherCallTime ? 1 : -1));

	 return ret;
}

QVariant PhoneHistoryItem::data(int column, int role) const
{
    QVariant ret;
     switch (column) {
         case LabelColumn:
             ret = labelData(role);
             break;
		 case DirectionColumn:
			 ret = directionData(role);
			 break;
         default:
             break;
     }

    return ret;
}

QVariant PhoneHistoryItem::labelData(int role) const
{
	if(role == Qt::DecorationRole) {
		QImage photo;
		if (isCenterCall(m_phoneHistory.callMode())) {
			photo.load(QLatin1String(":/images/ui/call_live_interpreter.jpg"));
		} else {
			QString photoFile;
			User::Contact contact = m_userService->findPhoneContactByNumber(m_phoneHistory.callerId());
			if (contact.isValid()) 
				photoFile = contact.photofile();
			if (photoFile.isEmpty())
				photo.load(QLatin1String(":/images/ui/default_photo.jpg"));
			else
				photo.load(photoFile);
		}
		return photo.scaled(QSize(32,32));
	} else  if (role == Qt::DisplayRole) {

		QString callerName;
		QString timeDisplay;
		if (isCenterCall(m_phoneHistory.callMode())) {
			callerName = tr("Live interpreter");
		} else {
			callerName = m_phoneHistory.contactName();
			if (callerName.isEmpty())
				callerName = m_phoneHistory.callerId();
		}
		QFontMetricsF ff(font(LabelColumn));
		QString elidedText = ff.elidedText(callerName, Qt::ElideRight, treeWidget()->geometry().width()-120);
		elidedText.append(QLatin1String("\n"));   //show the duration at a new line

		QDateTime today = QDateTime::currentDateTime();
		QDateTime callTime = QDateTime::fromTime_t(m_phoneHistory.callTime().toInt());
		if (today.date() == callTime.date()) {
			timeDisplay = callTime.toString(QLatin1String("hh:mm"));
		} else {
			timeDisplay = callTime.toString(QLatin1String("MM-dd"));
		}

		return (elidedText+timeDisplay);
	}

    return QVariant();
}

QVariant PhoneHistoryItem::directionData(int role) const
{
	if (role == Qt::DecorationRole) {
		QIcon statusIcon;
		if (isCallMissed()) {
			statusIcon.addFile(QLatin1String(":/images/ui/callmissed.png"));
		} else if (isCallIn()) {
			statusIcon.addFile(QLatin1String(":/images/ui/callin.png"));
		} else/* if (isCallOut()) */{
			statusIcon.addFile(QLatin1String(":/images/ui/callout.png"));
		}
		return statusIcon;
	} 

	return QVariant();
}

bool PhoneHistoryItem::isCallOut() const
{
	return (m_phoneHistory.callStatus()==QLatin1String("1") || m_phoneHistory.callStatus()==QLatin1String("2"));
}

bool PhoneHistoryItem::isCallIn() const
{
	return (m_phoneHistory.callStatus()==QLatin1String("4"));
}

bool PhoneHistoryItem::isCallMissed() const
{
	return (m_phoneHistory.callStatus()==QLatin1String("3"));
}

} // namespace Qpid
