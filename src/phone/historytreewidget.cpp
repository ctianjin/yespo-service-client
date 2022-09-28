#include "historytreewidget.h"
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include "recentlistitems_p.h"

namespace Qpid {
 
HistoryTreeWidget::HistoryTreeWidget(QWidget *parent) 
	: QTreeWidget(parent),
	m_itemTriggered(0)
{
	m_actionRemove = new QAction(tr("Remove"), this);
	m_menu = new QMenu(this);
	m_menu->addAction(m_actionRemove);

	connect(m_actionRemove, SIGNAL(triggered()), this, SLOT(slotRemoveTriggered()));
}

HistoryTreeWidget::~HistoryTreeWidget()
{

}

void HistoryTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button()== Qt::RightButton)
		return;
	else
		QTreeWidget::mousePressEvent(event);
}

void HistoryTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
	m_itemTriggered = itemAt(event->pos());

	if (m_itemTriggered && m_itemTriggered->type() == ItemBase::PhoneHistoryItemType) {
		PhoneHistoryItem* historyItem = static_cast<PhoneHistoryItem*>(m_itemTriggered);
		if (historyItem) {
			m_menu->exec(event->globalPos());
			update(visualItemRect(m_itemTriggered));
		}
	}
}

void HistoryTreeWidget::slotRemoveTriggered()
{
	Q_EMIT remove(m_itemTriggered);
}

}

