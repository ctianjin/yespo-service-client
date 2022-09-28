#ifndef QPID_RECENTLISTITEMS_P_H
#define QPID_RECENTLISTITEMS_P_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtCore/QObject>
#include "user/phonehistory.h"

namespace Qpid {

class UserService;

class ItemBase : public QTreeWidgetItem, public QObject
{
public:
    enum ItemType
    {
        DefaultItemType = QTreeWidgetItem::UserType + 1,
        PhoneHistoryItemType
    };

	enum Column
	{
		MinColumn = 0,
		LabelColumn = MinColumn,
		DirectionColumn,

		MaxColumn
	};

    explicit ItemBase(QTreeWidget* view, ItemType type = DefaultItemType);
    ItemBase(const ItemBase& rhs);
    virtual ~ItemBase();

    ItemBase& operator=(const ItemBase& rhs);

    virtual bool operator<(const QTreeWidgetItem& rhs) const;

protected:
    // Compare returns negative, 0, positive, rhs is always the same type of this
    // Returns 0 by default
    virtual int compare(const QTreeWidgetItem& rhs) const;

private:

};

class PhoneHistoryItem : public ItemBase
{
public:
	PhoneHistoryItem(QTreeWidget* view, const User::PhoneHistory& phoneHistory);
    virtual ~PhoneHistoryItem();

	void setUserService(UserService* userService);

	User::PhoneHistory phoneHistory() const;
	void setPhoneHistory(const User::PhoneHistory& phoneHistory);

    virtual QVariant data(int column, int role) const;

	bool isCallOut() const;
	bool isCallIn() const;
	bool isCallMissed() const;

protected:
    virtual int compare(const QTreeWidgetItem& rhs) const;

    QVariant labelData(int role) const;
	QVariant directionData(int role) const;

private:

	User::PhoneHistory m_phoneHistory;
	UserService* m_userService;

    Q_DISABLE_COPY(PhoneHistoryItem)
};


} // namespace Qpid

#endif // QPID_RECENTLISTITEMS_P_H
