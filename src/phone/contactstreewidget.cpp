#include "contactstreewidget.h"
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include "contactsitems_p.h"

namespace Qpid {
 
ContactsTreeWidget::ContactsTreeWidget(QWidget *parent) 
	: QTreeWidget(parent),
	m_itemTriggered(0)
{
	m_actionProfile = new QAction(tr("View Profile"), this);
	m_actionRemove = new QAction(tr("Remove"), this);
	m_menu = new QMenu(this);
	m_menu->addAction(m_actionProfile);
	m_menu->addSeparator();
	m_menu->addAction(m_actionRemove);

	connect(m_actionProfile, SIGNAL(triggered()), this, SLOT(slotProfileTriggered()));
	connect(m_actionRemove, SIGNAL(triggered()), this, SLOT(slotRemoveTriggered()));
}

ContactsTreeWidget::~ContactsTreeWidget()
{

}

void ContactsTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button()== Qt::RightButton)
		return;
	else
		QTreeWidget::mousePressEvent(event);
}

void ContactsTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
	m_itemTriggered = itemAt(event->pos());

	if (m_itemTriggered && m_itemTriggered->type() == ContactItemBase::ContactItemType) {
		ContactItem* contactItem = static_cast<ContactItem*>(m_itemTriggered);
		if (contactItem && contactItem->contact().addBookId()!=QLatin1String("-1")) {
			m_menu->exec(event->globalPos());
			update(visualItemRect(m_itemTriggered));
		}
	}
}

void ContactsTreeWidget::slotProfileTriggered()
{
	Q_EMIT viewProfile(m_itemTriggered);
}

void ContactsTreeWidget::slotRemoveTriggered()
{
	Q_EMIT remove(m_itemTriggered);
}

}

