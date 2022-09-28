#ifndef QPID_RECENTLISTITEMS_P_H
#define QPID_RECENTLISTITEMS_P_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtCore/QObject>
#include <QPointer>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QPainter>
#include "user/callrecord.h"
#include "user/blacklist.h"

namespace ns {

class PhotoService;

class TextColorDelegate: public QStyledItemDelegate
{
public:
	explicit TextColorDelegate(QObject* parent = 0) : QStyledItemDelegate(parent)
	{ }

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const  {
		QStyleOptionViewItemV4 viewOption(option);
		painter->save();
		QColor itemForegroundColor = index.data(Qt::ForegroundRole).value<QColor>();
		if (itemForegroundColor.isValid()) {
			if (itemForegroundColor != option.palette.color(QPalette::WindowText))
				viewOption.palette.setColor(QPalette::HighlightedText, itemForegroundColor);
		}
		painter->restore();
		QStyledItemDelegate::paint(painter, viewOption, index);
	}
};

class ItemBase : public QTreeWidgetItem, public QObject
{
public:
    enum ItemType
    {
        DefaultItemType = QTreeWidgetItem::UserType + 1,
        CallRecordItemType,
		BlacklistItemType
    };

	enum Column
	{
		MinColumn = 0,
		PhotoColumn = MinColumn,
		LabelColumn,
		ModeColumn,
		TimeColumn,

		MaxColumn
	};

	enum ColumnBlacklist
	{
		BlacklistMinColumn = 0,
		BlacklistLabelColumn = BlacklistMinColumn,
		BlacklistDelColumn,

		BlacklistMaxColumn
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

class BlacklistItem : public ItemBase
{
	Q_OBJECT

public:
	BlacklistItem(QTreeWidget* view, const User::Blacklist& blist);
	virtual ~BlacklistItem();

	void setPhotoService(PhotoService* photoService);

	User::Blacklist blacklist() const;
	void setBlacklist(const User::Blacklist& blist);

	virtual QVariant data(int column, int role) const;

public Q_SLOTS:
	void itemEntered(QTreeWidgetItem* item, int column);
	void itemPressed(QTreeWidgetItem* item, int column);
	void itemLeaved(QTreeWidgetItem* item, int column);

protected:
	virtual int compare(const QTreeWidgetItem& rhs) const;

	QVariant labelData(int role) const;
	QVariant delData(int role) const;

private Q_SLOTS:
	void slotPhotoUpdate(const QString& url, const QString& fileName);

private:
	User::Blacklist m_blacklist;
	bool m_isHovered;
	bool m_isHoverColumn;
	bool m_isClicked;

	QPointer<PhotoService> m_photoService;

	 Q_DISABLE_COPY(BlacklistItem)

};

class CallRecordItem : public ItemBase
{
	Q_OBJECT

public:
	CallRecordItem(QTreeWidget* view, const User::CallRecord& cdr);
    virtual ~CallRecordItem();

	void setPhotoService(PhotoService* photoService);

	User::CallRecord cdr() const;
	void setCDR(const User::CallRecord& cdr);

    virtual QVariant data(int column, int role) const;

	bool isAnswered();

protected:
    virtual int compare(const QTreeWidgetItem& rhs) const;

	QVariant photoData(int role) const;
    QVariant labelData(int role) const;
	QVariant modeData(int role) const;
	QVariant timeData(int role) const;

private Q_SLOTS:
	void slotPhotoUpdate(const QString& url, const QString& fileName);

private:
	User::CallRecord m_cdr;

	QPointer<PhotoService> m_photoService;

    Q_DISABLE_COPY(CallRecordItem)
};


} // namespace ns

#endif // QPID_RECENTLISTITEMS_P_H
