#include <QDebug>
#include "customtreewidget.h"
#include <QMouseEvent>

namespace ns {

CustomTreeWidget::CustomTreeWidget(QWidget* parent) :
    QTreeWidget(parent)
{
}

CustomTreeWidget::~CustomTreeWidget()
{
}

void CustomTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
	QTreeWidget::mouseMoveEvent(event);

	if (itemAt(event->pos()) == NULL) {

		Q_EMIT itemLeaved(NULL, -1);

		setCursor(Qt::ArrowCursor);
	}	
}

void CustomTreeWidget::leaveEvent(QEvent *event)
{
	QTreeWidget::leaveEvent(event);

	Q_EMIT itemLeaved(NULL, -1);

	setCursor(Qt::ArrowCursor);
}

} // namespace ns
