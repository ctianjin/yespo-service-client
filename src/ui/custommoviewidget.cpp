#include "custommoviewidget.h"
#include <QPaintEvent>
#include <QPainter>

namespace ns {

CustomMovieWidget::CustomMovieWidget(QWidget* parent) :
    QWidget(parent)
{
	m_frameNo = 0;
}

CustomMovieWidget::~CustomMovieWidget()
{
}

void CustomMovieWidget::nextAnimationFrame(int diameter, int width)
{
	m_frameNo += 20;
	m_diameter = diameter;
	m_widthToPaint = width;
	update();
}

void CustomMovieWidget::paintEvent(QPaintEvent *e)
{
// 	QPainter painter(this);
// 	QPen pen;
// 
// 	painter.setRenderHint(QPainter::Antialiasing, true);
// 	painter.translate(width() / 2, height() / 2);
// 
// 	for (int diameter = 100; diameter < m_widthToPaint; diameter += m_diameter) {
// 		int delta = abs((m_frameNo % 128) - diameter / 2);
// 		int alpha = 255 - (delta * delta) / 4 - diameter;
// 
// 		if (alpha > 0) {
// 			pen.setWidth(10);
// 			//painter.setPen(QPen(QColor(0, diameter / 2, 127, alpha), 3));
// 			pen.setColor(QColor(255, 255, 255, alpha));
// 			painter.setPen(pen);
// 			painter.drawEllipse(QRect(-diameter /2 , -diameter / 2,
// 					diameter , diameter ));
// 		}
// 	}
}



} // namespace ns
