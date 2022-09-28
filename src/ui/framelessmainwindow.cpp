#include <QtCore/QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QBitmap>
#include "framelessmainwindow.h"
#include "framelesshelper.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "FramelessMainWindow:"
#endif // DEBUG_PREFIX

namespace ns {

FramelessMainWindow::FramelessMainWindow(QWidget* parent, Qt::WindowFlags flags) :
    MainWindow(parent, flags)
    //m_framelessHelper(new FramelessHelper)
{
    //m_framelessHelper->activate(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowMinimizeButtonHint );

	setAttribute(Qt::WA_TranslucentBackground, true);

	leftButtonPressed = false;
}

FramelessMainWindow::~FramelessMainWindow()
{
}

void FramelessMainWindow::mousePressEvent(QMouseEvent * event)
{
	if(event->button() == Qt::LeftButton) {   
		dragPos = event->globalPos();
		leftButtonPressed = true;
	}
}

void FramelessMainWindow::mouseReleaseEvent(QMouseEvent * event )
{
	leftButtonPressed = false;
}

void FramelessMainWindow::mouseMoveEvent(QMouseEvent * event )
{
	if( event->buttons().testFlag(Qt::LeftButton) && leftButtonPressed) {
		move(pos() + (event->globalPos() - dragPos));
		dragPos = event->globalPos();
		Q_EMIT notifyMove(pos() + (event->globalPos() - dragPos));
	}
}

// FramelessHelper* FramelessMainWindow::framelessHelper() const
// {
//     return m_framelessHelper.data();
// }

} // namespace ns
