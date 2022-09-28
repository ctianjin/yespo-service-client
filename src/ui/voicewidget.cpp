#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include "voicewidget.h"

namespace ns {

RenderArea::RenderArea(QWidget* parent) :
    QWidget(parent)
{
	m_level = 0.0;
}

RenderArea::~RenderArea()
{
}

void RenderArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBackground(QBrush(QColor(229, 229, 229)));

	painter.setPen(Qt::black);
	painter.drawRect(QRect(painter.viewport().left(),
	painter.viewport().top(),
	painter.viewport().right(),
	painter.viewport().bottom()));
	if ( m_level - 0.0090000<= 0.0 )
		return;
	
	painter.setPen(QColor(31, 210, 86));

	int pos = ((painter.viewport().right()) - (painter.viewport().left())) * m_level * 30;
	
	for (int i = 1; i < (20 - 1); ++i) {
		int x1 = painter.viewport().left() + 1;
		int y1 = painter.viewport().top() + i;
		int x2 = painter.viewport().left() + pos;
		int y2 = painter.viewport().top() + i;
 		if (x2 >= painter.viewport().right() - painter.viewport().left())
 			x2 = painter.viewport().right() - painter.viewport().left() - 1;

		painter.drawLine(QPoint(x1, y1),QPoint(x2, y2));		
	}
}

void RenderArea::setLevel(qreal value)
{
	m_level = value;
	update();
}


} // namespace ns
