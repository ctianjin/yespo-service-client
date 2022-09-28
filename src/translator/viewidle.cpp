#include "viewidle.h"
#include "ui_viewidle.h"
#include "phoneapplication.h"
#include <QTimer>
#include <QTime>
#include <QGraphicsEffect>
#include <QDebug>

#include "reportnotify.h"

namespace ns {

ViewIdle::ViewIdle(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ViewIdle),
	m_duration(0),
	m_timeInterval(MorningEve)
{
    m_ui->setupUi(this);

// 	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect( );
// 	effect->setBlurRadius( 7 );
// 	m_ui->labelDuration->setGraphicsEffect( effect );

//	m_ui->labelViewPhoto->setRadius(m_ui->labelViewPhoto->width()/2.0, m_ui->labelViewPhoto->height()/2.0);

	m_ui->labelCode->setPointSize(16);
	m_ui->labelCode->setFontWeight(0);
	m_ui->labelCode->setFontAlpha(150);
	m_ui->labelCode->setFontBorder(2);
	m_ui->labelCode->setFontSpacing(0.5);

	m_ui->labelIntroduce->setPointSize(25);
	m_ui->labelIntroduce->setFontWeight(75);
	m_ui->labelIntroduce->setFontAlpha(150);
	m_ui->labelIntroduce->setFontBorder(3);
	m_ui->labelIntroduce->setFontFamily(QLatin1String("Arial"));

	m_ui->labelWaiting->setPointSize(20);
	m_ui->labelWaiting->setFontWeight(75);
	m_ui->labelWaiting->setFontAlpha(190);
	m_ui->labelWaiting->setFontBorder(3);
	m_ui->labelWaiting->setFontFamily(QLatin1String("Arial"));

	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()));
}

ViewIdle::~ViewIdle()
{
}

void ViewIdle::setDisplayInfo(const QString& displayName)
{
	m_displayName = displayName;
	int hour = QTime::currentTime().hour();
	m_timeInterval = timeToNameSpace(hour);
	nameToWelcomeCode(m_timeInterval);
	QPixmap pic(QString(QLatin1String(":/images/ui/%1.jpg")).arg(nameToString(m_timeInterval)));
	m_ui->widgetTop->setFadePixmap(pic);
	m_ui->widgetTop->setTimeString(nameToString(m_timeInterval));
	m_ui->labelIntroduce->setText(tr("%1, %2").arg(m_welcomeCode).arg(m_displayName));
	m_ui->labelCode->setText(m_timeCode);
	m_timer->start();
}

void ViewIdle::setPhoto(const QString& photoFile)
{
// 	QPixmap pic(photoFile);
// 	QPixmap mask = QLatin1String(":/images/ui/view_id_mask.png");
// 	//m_ui->labelViewPhoto->setPixmap(pic.scaled(m_ui->labelViewPhoto->size()));
// 	
// 	m_ui->labelViewPhoto->setMaskPixmap(pic.scaled(m_ui->labelViewPhoto->size()), mask);
}

void ViewIdle::showNotify(int height) 
{
	if (m_reportNotify) {
		m_reportNotify->setGeometry(0, height, width(), 32);
		m_reportNotify->show();
	}
}

ReportNotify* ViewIdle::reportNotify() const 
{
	if (m_reportNotify)
		return m_reportNotify;
	return NULL;
}

void ViewIdle::setReportNotify(ReportNotify* reportNotify)
{
	if (m_reportNotify) {

	}

	m_reportNotify = reportNotify;
	m_reportNotify->setParent(this);

	if (m_reportNotify) {

	}

}

void ViewIdle::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	} 
}

void ViewIdle::slotUpdateTime()
{
	QTime time = QTime::currentTime();
	int hour = time.hour();
	QString text = time.toString(QLatin1String("hh:mm"));
// 	if (currentTime.second() % 2 == 0)
// 		text[2] = QLatin1Char(' ');
	m_ui->labelDuration->setText(text);

  	if (0 <= hour && hour < 4) {
  		m_timeInterval = MorningEve;
  	} else if (4 <= hour && hour < 8) {
  		m_timeInterval = Morning;
  	} else if (8 <= hour && hour < 12) {
  		m_timeInterval = Inmorning;
  	} else if (12 <= hour && hour < 16) {
  		m_timeInterval = EveningEve;
  	} else if (16 <= hour && hour < 20) {
  		m_timeInterval = Evening;
  	} else if (20 <= hour && hour < 24) {
  		m_timeInterval = Inevening;
  	} 
#if 0
 	if (0 <= sec && sec < 10) {
 		m_timeInterval = MorningEve;
 	} else if (10 <= sec && sec < 20) {
 		m_timeInterval = Morning;
 	} else if (20 <= sec && sec < 30) {
 		m_timeInterval = Inmorning;
  	} else if (30 <= sec && sec < 40) {
  		m_timeInterval = EveningEve;
  	} else if (40 <= sec && sec < 50) {
  		m_timeInterval = Evening;
  	} else if (50 <= sec && sec < 60) {
  		m_timeInterval = Inevening;
  	}
#endif

	if (m_ui->widgetTop->timeString() != nameToString(m_timeInterval)) {
		QPixmap pic(QString(QLatin1String(":/images/ui/%1.jpg")).arg(nameToString(m_timeInterval)));
		m_ui->widgetTop->setUserPixmap(pic);
		m_ui->widgetTop->startTimer(nameToString(m_timeInterval));
		nameToWelcomeCode(m_timeInterval);
		m_ui->labelIntroduce->setText(tr("%1, %2").arg(m_welcomeCode).arg(m_displayName));
		m_ui->labelCode->setText(m_timeCode);
	}

}

ViewIdle::TimeInterval ViewIdle::timeToNameSpace(int& hour)
{
	if (0 <= hour && hour < 4) {
		return MorningEve;
	} else if (4 <= hour && hour < 8) {
		return Morning;
	} else if (8 <= hour && hour < 12) {
		return Inmorning;
	} else if (12 <= hour && hour < 16) {
		return EveningEve;
	} else if (16 <= hour && hour < 20) {
		return Evening;
	} else if (20 <= hour && hour < 24) {
		return Inevening;
	} else
		return Unknown;
}

QString ViewIdle::nameToString(ViewIdle::TimeInterval tInterval)
{
	if (tInterval == MorningEve) {
		return QLatin1String("MorningEve");
	} else if(tInterval == Morning) {
		return QLatin1String("Morning");
	} else if (tInterval == Inmorning) {
		return QLatin1String("Inmorning");
	} else if (tInterval == EveningEve) {
		return QLatin1String("EveningEve");
	} else if (tInterval == Evening) {
		return QLatin1String("Evening");
	} else if (tInterval == Inevening) {
		return QLatin1String("Inevening");
	} else {
		return QString();
	}
}

void ViewIdle::nameToWelcomeCode(ViewIdle::TimeInterval tInterval)
{
	switch(tInterval)
	{
	case MorningEve:
		m_timeCode = tr("It is time to sleep,remembered must earlier rest oh.");
		m_welcomeCode = tr("Late at night");
		break;
	case Morning:
		m_timeCode = tr("Keep smiling,only do what your heart tells you.");
		m_welcomeCode = tr("Good morning");
		break;
	case Inmorning:
		m_timeCode = tr("Every day is the best day in the year.");
		m_welcomeCode = tr("Hi");
		break;
	case EveningEve:
		m_timeCode = tr("If we dream, everything is possible.");
		m_welcomeCode = tr("Good afternoon");
		break;	
	case Evening:
		m_timeCode = tr("Happiness is about having each tiny wish come true.");
		m_welcomeCode = tr("Twilight, again");
		break;
	case Inevening:
		m_timeCode = tr("Learn from yesterday, live for today, hope for tomorrow.");
		m_welcomeCode = tr("Good evening");
		break;
	}
}

void ViewIdle::retranslateTr()
{
	nameToWelcomeCode(m_timeInterval);
	m_ui->labelIntroduce->setText(tr("%1, %2").arg(m_welcomeCode).arg(m_displayName));
	m_ui->labelCode->setText(m_timeCode);
}


} // namespace ns
