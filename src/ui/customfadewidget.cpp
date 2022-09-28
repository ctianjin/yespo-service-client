#include "customfadewidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QTimer>

namespace ns {

CustomFadeWidget::CustomFadeWidget(QWidget* parent) :
    QWidget(parent),
	m_userFade(false),
	m_background(),
	m_userBackground(),
	m_timeString()
{
	m_alpha = 0;
	m_alphaShow = 255;
	m_timer = new QTimer(this);
	m_timer->setInterval(100);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

CustomFadeWidget::~CustomFadeWidget()
{
}

int CustomFadeWidget::alpha()
{
	return m_alpha;
}

void CustomFadeWidget::startTimer(const QString& timeString)
{
	m_timeString = timeString;
	m_alpha = 0;
	m_timer->start();
	m_userFade = true;
}

bool CustomFadeWidget::isTimerActive()
{
	return m_timer->isActive();
}

void CustomFadeWidget::setFadePixmap(const QPixmap& pic)
{
	m_background = pic;
}

void CustomFadeWidget::setUserPixmap(const QPixmap& pic)
{
	m_userBackground = pic;
}

void CustomFadeWidget::setTimeString(const QString& timeString)
{
	m_timeString = timeString;
}

QString CustomFadeWidget::timeString() const
{
	return m_timeString;
}

void CustomFadeWidget::slotTimer()
{
	update();
}

void CustomFadeWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (!m_background.isNull()) {
		painter.fillRect(rect(), m_background);
	}

	if (!m_userFade)
		return;

	painter.fillRect(rect(), QColor(0, 0, 0, m_alpha));
	m_alpha += 255 * m_timer->interval() / 1000;  
	if (m_alpha >= 255) {  
		m_timer->stop(); 
		m_alpha = 255;
		m_userFade = false;
		painter.fillRect(rect(), m_userBackground);
		m_background = m_userBackground;
	}  
}



} // namespace ns
