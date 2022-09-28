#include "pagewelcome.h"
#include "ui_pagewelcome.h"
#include "phoneapplication.h"
#include "pagelogin.h"
#include "messagepanel.h"
#include "userservice.h"

#include <QMovie>
#include <QDebug>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PageWelcome:"
#endif // DEBUG_PREFIX

namespace ns {

PageWelcome::PageWelcome(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_PageWelcome)
{
	m_ui->setupUi(this);

	m_widgetLogin.reset(new PageLogin(this));
	m_widgetLogin->hide();

	connect(m_widgetLogin.data(), SIGNAL(back()), this, SLOT(slotHideLoginWidget()));

	QString version = qApp->applicationVersion();
#ifdef BUILDSYS_OFFICAL 
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);
#endif
	m_ui->labelVersion->setText(tr("V : ") + version);
}

PageWelcome::~PageWelcome()
{
	 
}

UserService* PageWelcome::userService() const
{
	return m_userService;
}

void PageWelcome::setUserService(UserService* userService)
{
	if(m_userService) {
	}
	
	m_userService = userService;
	m_widgetLogin->setUserService(userService);

	if(m_userService) {
	}
}

void PageWelcome::setVersion(QString version)
{	
	m_widgetLogin->setVersion(version);
}

void PageWelcome::setError(const QString& error)
{
	m_ui->labelVersion->setText(error);
}

void PageWelcome::setPreference(Preference* pref)
{
	PreferenceSupport::setPreference(pref);

	if (m_widgetLogin) {
		m_widgetLogin->setPreference(pref);
	}
}

void PageWelcome::logout()
{
	m_widgetLogin->logout();
}

PageLogin* PageWelcome::loginPage() const
{
	return m_widgetLogin.data();
}

void PageWelcome::autoLogin(QString userName, QString password)
{
	m_widgetLogin->show();
	m_widgetLogin->setGeometry(rect());
	m_widgetLogin->autoLogin(userName, password);
}

void PageWelcome::changeEvent(QEvent * event)
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

void PageWelcome::on_pushButtonLogin_clicked()
{
	m_widgetLogin->setGeometry(QRect(rect().bottomLeft(), size()));
	QPropertyAnimation *animation = new QPropertyAnimation(m_widgetLogin.data(),"geometry");
	animation->setDuration(1000);
	animation->setStartValue(QRect(rect().bottomLeft(), QSize(m_widgetLogin->width(), m_widgetLogin->height())));
	animation->setEndValue(geometry());
	//animationControl->setEasingCurve(QEasingCurve::InOutCirc);
	animation->setEasingCurve(QEasingCurve::OutExpo);
	animation->start(QPropertyAnimation::DeleteWhenStopped);
	m_widgetLogin->show();
// 	if(m_animationControl) {
// 		m_animationControl->setDuration(1000);
// 		m_animationControl->setStartValue(QRect(rect().bottomLeft(), QSize(m_widgetLogin->width(), m_widgetLogin->height())));
// 		m_animationControl->setEndValue(geometry());
// 		//animationControl->setEasingCurve(QEasingCurve::InOutCirc);
// 		m_animationControl->setEasingCurve(QEasingCurve::OutExpo);
// 		m_animationControl->start();
//  		m_widgetLogin->show();
// 	}
	//m_ui->pushButtonLogin->setEnabled(false);
}

void PageWelcome::slotHideLoginWidget()
{
	QPropertyAnimation* animation = new QPropertyAnimation(m_widgetLogin.data(),"geometry");

	animation->setDuration(1000);
	animation->setStartValue(QRect(m_widgetLogin->pos(), QSize(m_widgetLogin->width(), m_widgetLogin->height())));
	animation->setEndValue(QRect(rect().bottomLeft()  ,QSize(m_widgetLogin->width(), m_widgetLogin->height())));
	animation->start(QPropertyAnimation::DeleteWhenStopped);

// 	if(m_animationControl) {
// 		m_animationControl->setDuration(1000);
// 		m_animationControl->setStartValue(QRect(m_widgetLogin->pos(), QSize(m_widgetLogin->width(), m_widgetLogin->height())));
// 		m_animationControl->setEndValue(QRect(rect().bottomLeft()  ,QSize(m_widgetLogin->width(), m_widgetLogin->height())));
// 		m_animationControl->start();
// 	}
}

void PageWelcome::retranslateTr()
{
	QString version = qApp->applicationVersion();
#ifdef BUILDSYS_OFFICAL
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);
#endif
	m_ui->labelVersion->setText(tr("V : ") + version);
}

} // namespace ns
