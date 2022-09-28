#include "videolabel.h"
#include <QPainter>
#include <QTextLayout>
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QString>
#include <QDebug>
#include <QBitmap>

namespace ns {
 
VideoLabel::VideoLabel(QWidget *parent) 
	: QLabel(parent),
	m_fullScreen(false)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	resize(480, 640);
}

VideoLabel::~VideoLabel()
{

}

void VideoLabel::setFullScreen(bool fullScreen)
{
	m_fullScreen = fullScreen;
}

void VideoLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
	m_fullScreen = !m_fullScreen;
	Q_EMIT fullScreen(m_fullScreen);
}

}

