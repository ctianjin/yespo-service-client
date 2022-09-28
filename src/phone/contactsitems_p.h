#ifndef QPID_CONTACTSLISTITEMS_P_H
#define QPID_CONTACTSLISTITEMS_P_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtCore/QObject>
#include "user/contact.h"

namespace Qpid {

class ContactItemBase : public QTreeWidgetItem, public QObject
{
public:
    enum ItemType
    {
        DefaultItemType = QTreeWidgetItem::UserType + 1,
        ContactItemType,
    };

	enum Column
	{
		MinColumn = 0,
		PhotoColumn = MinColumn,
		StatusColumn,
		LabelColumn,

		MaxColumn
	};

	static const int statusColumnWidth;

    explicit ContactItemBase(QTreeWidget* view, ItemType type = DefaultItemType);
    ContactItemBase(const ContactItemBase& rhs);
    virtual ~ContactItemBase();

    ContactItemBase& operator=(const ContactItemBase& rhs);

    virtual bool operator<(const QTreeWidgetItem& rhs) const;

	void updateItem();

protected:
    // Compare returns negative, 0, positive, rhs is always the same type of this
    // Returns 0 by default
    virtual int compare(const QTreeWidgetItem& rhs) const;

private:

};

class ContactItem : public ContactItemBase
{
public:
	ContactItem(QTreeWidget* view, const User::Contact& contact);
    virtual ~ContactItem();

	User::Contact contact() const;
	void setContact(const User::Contact& contact);

    virtual QVariant data(int column, int role) const;

	void setPhoto(const QString& fileName);
	void updateContactPhone(const QString& oldPhone, const QString& phoneCountry, const QString& phone);
	void deleteContactPhone(const QString& delPhone);
	void updateInterpretationLanguage(const QString& source, const QString& target);

	int buddyID() const;
	void setBuddyID(int buddyID);
	
	void setOnlineStatus(const QString& status);

protected:
    virtual int compare(const QTreeWidgetItem& rhs) const;

	QVariant photoData(int role) const;
	QVariant statusData(int role) const;
    QVariant labelData(int role) const;

private:
	bool isOnline() const;
	bool isOffline() const;
	bool isPhone() const;

	User::Contact m_contact;
	int m_buddyID;

    Q_DISABLE_COPY(ContactItem)
};


} // namespace Qpid

#endif // QPID_CONTACTSLISTITEMS_P_H
