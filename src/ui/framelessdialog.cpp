#include <QtCore/QDebug>
#include <QtGui/QPixmap>
#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QApplication>
#include "framelessdialog.h"
#include "boxshadow.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "FramelessDialog:"
#endif // DEBUG_PREFIX

namespace ns {

FramelessDialog::FramelessDialog(QWidget* parent, Qt::WindowFlags flags) :
    QDialog(parent, flags)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground);

// 	BoxShadow *boxShadow = new BoxShadow();
// 	boxShadow->setBlurRadius(35.0);
// 	boxShadow->setDistance(12.0);
// 	boxShadow->setColor(QColor(0, 0, 0, 70));
// 	setGraphicsEffect(boxShadow);

	leftButtonPressed = false;
}

FramelessDialog::~FramelessDialog()
{
}

void FramelessDialog::resizeEvent(QResizeEvent* event)
{
// 	QDialog::resizeEvent(event);
// 
// 	const QSize size = event->size();
// 	QPixmap pixmap(size);
// 	QPainter painter(&pixmap);
// 	painter.fillRect(pixmap.rect(), Qt::white);
// 	painter.setBrush(Qt::black);
// 	painter.drawRoundedRect(0, 0, size.width() - 1, size.height() - 1, 3, 3);
// 	setMask(pixmap.createMaskFromColor(Qt::white));
}

void FramelessDialog::mousePressEvent(QMouseEvent * event)
{
	if(event->button() == Qt::LeftButton) {   
		dragPos = event->globalPos();
		leftButtonPressed = true;
	}
}

void FramelessDialog::mouseReleaseEvent (QMouseEvent * event )
{
	leftButtonPressed = false;
}

void FramelessDialog::mouseMoveEvent (QMouseEvent * event )
{
	if( event->buttons().testFlag(Qt::LeftButton) && leftButtonPressed) {
		move(pos() + (event->globalPos() - dragPos));
		dragPos = event->globalPos();
	}
}

} // namespace ns
