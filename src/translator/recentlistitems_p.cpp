#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QRegExp>
#include <QIcon>
#include <QDateTime>
#include <QFontMetricsF>
#include <QDebug>
#include <QPen>
#include <QPainter>
#include "recentlistitems_p.h"
#include "photoservice.h"
#include "tonegoglobal.h"
#include "customtreewidget.h"

namespace ns {

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

//BlacklistItem
BlacklistItem::BlacklistItem(QTreeWidget* view, const User::Blacklist& blist) :
	ItemBase(view, BlacklistItemType),
	m_blacklist(blist),
	m_isHovered(false),
	m_isHoverColumn(false),
	m_isClicked(false)
{
	CustomTreeWidget* parentView = static_cast<CustomTreeWidget*>(view);
	if (parentView) {
		connect(parentView, SIGNAL(itemEntered(QTreeWidgetItem*, int)), this, SLOT(itemEntered(QTreeWidgetItem*, int)));
		connect(parentView, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(itemPressed(QTreeWidgetItem*, int)));
		connect(parentView, SIGNAL(itemLeaved(QTreeWidgetItem*, int)), this, SLOT(itemLeaved(QTreeWidgetItem*, int)));
	}
}

BlacklistItem::~BlacklistItem()
{
	if (m_photoService) {
		disconnect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}

	CustomTreeWidget* parentView = static_cast<CustomTreeWidget*>(treeWidget());
	if (parentView) {
		disconnect(parentView, SIGNAL(itemEntered(QTreeWidgetItem*, int)), this, SLOT(itemEntered(QTreeWidgetItem*, int)));
		disconnect(parentView, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(itemPressed(QTreeWidgetItem*, int)));
		disconnect(parentView, SIGNAL(itemLeaved(QTreeWidgetItem*, int)), this, SLOT(itemEntered(QTreeWidgetItem*, int)));
	}
}

void BlacklistItem::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
		disconnect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}

	m_photoService = photoService;

	if (m_photoService) {
		connect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}
}

User::Blacklist BlacklistItem::blacklist() const
{
	return m_blacklist;
}

void BlacklistItem::setBlacklist(const User::Blacklist& blist)
{
	m_blacklist = blist;
}

int BlacklistItem::compare(const QTreeWidgetItem& rhs) const
{
	return 0;
}

QVariant BlacklistItem::data(int column, int role) const
{
	QVariant ret;
	switch (column) {
		 case BlacklistLabelColumn:
			 ret = labelData(role);
			 break;
		 case BlacklistDelColumn:
			 ret = delData(role);
			 break;
		 default:
			 break;
	}

	return ret;
}

void BlacklistItem::itemEntered(QTreeWidgetItem* item, int column)
{
	if (static_cast<BlacklistItem*>(item) == this) {
		m_isHovered = true;
		if (column == BlacklistDelColumn) {
			treeWidget()->setCursor(Qt::PointingHandCursor);
			m_isHoverColumn = true;
		} else {
			treeWidget()->setCursor(Qt::ArrowCursor);
			m_isHoverColumn = false;
		}
	} else {
		m_isHovered = false;
		m_isHoverColumn = false;
	}

	emitDataChanged();
}

void BlacklistItem::itemPressed(QTreeWidgetItem* item, int column)
{
	if(static_cast<BlacklistItem*>(item) == this) {
		m_isClicked = true;
	} else {
		m_isClicked = false;
	}

	emitDataChanged();
}

void BlacklistItem::itemLeaved(QTreeWidgetItem* item, int column)
{
	m_isHovered = false;
	m_isHoverColumn = false;
	m_isClicked = false;
}

QVariant BlacklistItem::labelData(int role) const
{
	if (role == Qt::DisplayRole) {

		return m_blacklist.displayName();

	}else if(role == Qt::DecorationRole) {
		QPixmap pix;
		QString photoFile;
		if (m_photoService) 
			photoFile = m_photoService->photoFile(m_blacklist.smallPhoto());
		if (photoFile.isEmpty())
			photoFile = QLatin1String(":/images/ui/cdr_default.png");
		pix.load(photoFile);
		QPen pen(QColor(3,72,103));
		QPainter paint(&pix);
		paint.setPen(pen);
		paint.drawRect(0, 0, pix.width()-1, pix.height()-1);
		return QIcon(pix);
	} 

	return QVariant();
}

QVariant BlacklistItem::delData(int role) const
{
	if(role == Qt::DecorationRole) {
		QPixmap pix;
		QString photoFile;
		if (m_isHovered) {
			photoFile = QLatin1String(":/images/ui/delete_blacklist.png");
			if (m_isHoverColumn) {
				photoFile = QLatin1String(":/images/ui/delete_blacklist_hover.png");
				if(m_isClicked) {
					photoFile = QLatin1String(":/images/ui/delete_blacklist_pressed.png");
				}
			}
		} else {
			photoFile = QLatin1String(":/images/ui/blacklist_delete_transparent.png");
		}
		pix.load(photoFile);
		return QIcon(pix);
	}
	return QVariant();
}

void BlacklistItem::slotPhotoUpdate(const QString& url, const QString& fileName)
{
	if(url == m_blacklist.smallPhoto())
		emitDataChanged();
}

// CallRecordItem
CallRecordItem::CallRecordItem(QTreeWidget* view, const User::CallRecord& cdr) :
    ItemBase(view, CallRecordItemType),
	m_cdr(cdr)
{
	
}

CallRecordItem::~CallRecordItem()
{
	if (m_photoService) {
		disconnect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}
}

void CallRecordItem::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
		disconnect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}

	m_photoService = photoService;

	if (m_photoService) {
		connect(m_photoService, SIGNAL(photoDownloaded(const QString&, const QString&)), 
			this, SLOT(slotPhotoUpdate(const QString&, const QString&)));
	}
}

User::CallRecord CallRecordItem::cdr() const
{
	return m_cdr;
}

void CallRecordItem::setCDR(const User::CallRecord& cdr)
{
	m_cdr = cdr;
}

int CallRecordItem::compare(const QTreeWidgetItem& rhs) const
{
    Q_ASSERT(type() == rhs.type());

	const CallRecordItem& other = static_cast<const CallRecordItem&>(rhs);

	 QString callTime = m_cdr.startCallTime();
	 QString otherCallTime = other.cdr().startCallTime();

	 int ret = (callTime==otherCallTime ? 0 : (callTime > otherCallTime ? 1 : -1));

	 return ret;
}

QVariant CallRecordItem::data(int column, int role) const
{
    QVariant ret;
     switch (column) {
		 case PhotoColumn:
			 ret = photoData(role);
			 break;
         case LabelColumn:
             ret = labelData(role);
             break;
		 case ModeColumn:
			 ret = modeData(role);
			 break;
		 case TimeColumn:
			 ret = timeData(role);
			 break;
         default:
             break;
     }

    return ret;
}

bool CallRecordItem::isAnswered()
{
	if (m_cdr.isAnswered() == QLatin1String("1"))
		return true;
	return false;
}

QVariant CallRecordItem::photoData(int role) const
{
	if(role == Qt::DecorationRole) {

		QPixmap pix;
		QString photoFile;
		if (m_photoService) 
			photoFile = m_photoService->photoFile(m_cdr.smallPhoto());
		if (photoFile.isEmpty())
			photoFile = QLatin1String(":/images/ui/cdr_default.png");
		pix.load(photoFile);
		QPen pen(QColor(3,72,103));
		QPainter paint(&pix);
		paint.setPen(pen);
		paint.drawRect(0, 0, pix.width()-1, pix.height()-1);
		return QIcon(pix);

	} 

	return QVariant();
}

QVariant CallRecordItem::labelData(int role) const
{
    if (role == Qt::DisplayRole) {

		QString displayText = m_cdr.displayName();
		QString callDuration;

 		QFontMetricsF ff(font(LabelColumn));
 		QString elidedText = ff.elidedText(displayText, Qt::ElideRight, treeWidget()->geometry().width()-210);
		elidedText.append(QLatin1String("\n"));   //show the duration at a new line

		if (m_cdr.isAnswered().toInt() == 2) {
			callDuration.append(tr("Missed"));
		} else {
			int duration = m_cdr.callTime().toInt();
			const QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60);
			callDuration += QString::number(currentTime.hour()*60+currentTime.minute());
			callDuration += QLatin1Char('\'');
			callDuration += tr("%1").arg(currentTime.second(), 2, 10, QLatin1Char('0'));
			callDuration += QLatin1Char('\"');
		}		

		return (elidedText+callDuration);

	} /*else if(role == Qt::DecorationRole) {

		QPixmap pix;
		QString photoFile;
		if (m_photoService) 
			photoFile = m_photoService->photoFile(m_cdr.smallPhoto());
		if (photoFile.isEmpty())
			photoFile = QLatin1String(":/images/ui/cdr_default.png");
		pix.load(photoFile);
		QPen pen(QColor(3,72,103));
		QPainter paint(&pix);
		paint.setPen(pen);
		paint.drawRect(0, 0, pix.width()-1, pix.height()-1);
		return QIcon(pix);

	} */else if (role == Qt::TextColorRole) {

		if (m_cdr.isAnswered().toInt() == 2) 
			return QColor(200,0,0);
		else
			return QColor(0,0,0);

	}

    return QVariant();
}

QVariant CallRecordItem::modeData(int role) const
{
	if (role == Qt::DecorationRole) {
		QIcon modeIcon;
		if (YesPo::callMode(m_cdr.callMode()) == YesPo::cmTwoWay)
			modeIcon.addFile(QLatin1String(":/images/ui/call_mode_two.png"));
		else
			modeIcon.addFile(QLatin1String(":/images/ui/call_mode_three.png"));
		return modeIcon;
	} 

	return QVariant();
}

QVariant CallRecordItem::timeData(int role) const
{
	if (role == Qt::DisplayRole)  {
		QDateTime dateTime = QDateTime::fromTime_t(m_cdr.startCallTime().toInt());
		int hour = dateTime.time().hour();
		QString format  = QLatin1String("\nhh:mm");
		QString prefix;
		if (0 <= hour && hour <= 12) {
			prefix = tr("AM");
		} else {
			prefix = tr("PM");
		}
		return (prefix+dateTime.toString(format));
	} else if (role == Qt::TextColorRole) {
			return QColor(152,152,152);
	}

	return QVariant();
}

void CallRecordItem::slotPhotoUpdate(const QString& url, const QString& fileName)
{
	if (url == m_cdr.smallPhoto()) 
		emitDataChanged();
}

} // namespace ns
