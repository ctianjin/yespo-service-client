#include <QDebug>
#include <QDatetime>
#include <QTimer>
#include <QMovie>
#include <QFontMetrics>
#include "phonecallingview.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_phonecallingview.h"
#include "userservice.h"
#include "sipservice.h"
#include "callinfo.h"
#include "user/contact.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneCallingView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneCallingViewOptions
{

};

PhoneCallingView::PhoneCallingView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneCallingView),
    m_opt(new PhoneCallingViewOptions),
	m_durationTimer(NULL),
	m_duration(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->labelCallDuration->hide();
	m_ui->pushButtonMaximize->hide();

	connect(m_ui->callControlPanel, SIGNAL(mute(bool)), this, SLOT(slotMute(bool)));
	connect(m_ui->callControlPanel, SIGNAL(speaker(bool)), this, SLOT(slotSpeaker(bool)));
	connect(m_ui->callControlPanel, SIGNAL(hangup()), this, SLOT(slotHangup()));
	connect(m_ui->callControlPanel, SIGNAL(digit(const QChar&)), this, SLOT(slotDTMF(const QChar&)));
	connect(m_ui->callControlPanel, SIGNAL(answer()), this, SLOT(slotAnswer()));
	connect(m_ui->callControlPanel, SIGNAL(reject()), this, SLOT(slotReject()));
	connect(m_ui->callControlPanel, SIGNAL(volume(int)), this, SLOT(slotVolumeChanged(int)));
	connect(m_ui->pushButtonMaximize, SIGNAL(clicked()), this, SIGNAL(maximize()));
	
	m_durationTimer = new QTimer(this);
	m_durationTimer->setInterval(1000);
	connect(m_durationTimer, SIGNAL(timeout()), this, SLOT(slotDuration()));

	m_callConnecting = new QMovie(QLatin1String(":/images/ui/call_connecting.gif"));
}

PhoneCallingView::~PhoneCallingView()
{

}

UserService* PhoneCallingView::userService() const
{
	return m_userService;
}

void PhoneCallingView::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;

	if (m_userService) {
	}
}

SipService* PhoneCallingView::sipService() const
{
	return m_sipService;
}

void PhoneCallingView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

// Call* PhoneCallingView::currentCall() const
// {
// 	return m_currentCall;
// }
// 
// void PhoneCallingView::setCurrentCall(Call* call)
// {
// 	m_currentCall = call;
// }

void PhoneCallingView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneCallingView::signOut()
{
	m_ui->labelCallerPhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));

	if (m_sipService) {
		m_sipService->hangupCall(qApp->call()->callID());
	}
	AbstractPhoneView::signOut();
}

void PhoneCallingView::callOut()
{
	qApp->call()->setDirection(Call::DirectionOut);

	QString serviceMode = qApp->call()->serviceMode();
	if (isCenterCall(serviceMode)) {
		m_ui->frameCallerPanel->show();
		m_ui->labelTransConnect->show();
		m_ui->frameTransPanel->show();
		m_ui->labelCalleeConnect->hide();
		m_ui->frameCalleePanel->hide();

	} else if (isDirectCall(serviceMode)) {
		m_ui->frameCallerPanel->show();
		m_ui->labelTransConnect->hide();
		m_ui->frameTransPanel->hide();
		m_ui->labelCalleeConnect->show();
		m_ui->frameCalleePanel->show();
	} else {
		m_ui->frameCallerPanel->show();
		m_ui->labelTransConnect->show();
		m_ui->frameTransPanel->show();
		m_ui->labelCalleeConnect->show();
		m_ui->frameCalleePanel->show();
	}

	QString displayName;
	User::Contact contact = m_userService->findPhoneContactByNumber(qApp->call()->calleeNumber());
	if (contact.isValid()) {
		if (!contact.photofile().isEmpty())
			m_ui->labelCalleePhoto->setPixmap(QPixmap(contact.photofile()));
		else
			m_ui->labelCalleePhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
		m_ui->labelCalleeName->setText(contact.contactName().isEmpty()?contact.voiceraId():contact.contactName());
	} else {
		m_ui->labelCalleePhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
		displayName = qApp->call()->calleeName();
		if (displayName.isEmpty())
			m_ui->labelCalleeName->setText(qApp->call()->calleeNumber());
		else
			m_ui->labelCalleeName->setText(displayName);
	}

	QString text = tr("Calling...");
	m_ui->labelCallStatus->setText(text);
	m_ui->labelCallDuration->setText(text);

	m_ui->labelTransConnect->setMovie(m_callConnecting);
	m_ui->labelCalleeConnect->setMovie(m_callConnecting);
	m_callConnecting->start();
}

void PhoneCallingView::callIn()
{
	qApp->call()->setDirection(Call::DirectionIn);

	m_ui->callControlPanel->callIn();

	User::Contact contact = m_userService->findPhoneContactByNumber(qApp->call()->callerNumber());
	if (contact.isValid() && !contact.photofile().isEmpty()) {
		m_ui->labelCalleePhoto->setPixmap(QPixmap(contact.photofile()));
	} else {
		m_ui->labelCalleePhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
	}

	QString text = tr("%1 incoming call...").arg(qApp->call()->callerName());
	m_ui->labelCalleeName->setText(qApp->call()->callerName());
	m_ui->labelCallStatus->setText(text);
	m_ui->labelCallDuration->setText(text);

	m_ui->frameCallerPanel->hide();
	m_ui->labelTransConnect->hide();
	m_ui->frameTransPanel->hide();
	m_ui->labelCalleeConnect->hide();
	m_ui->frameCalleePanel->show();
}

void PhoneCallingView::callConnected(const QString& stausText)
{
	if (stausText==QLatin1String("*#01#")) {
		m_ui->labelCallStatus->setText(tr("Looking for an interpreter..."));
		m_ui->labelCallDuration->setText(tr("Looking for an interpreter..."));
	} else if(stausText==QLatin1String("*#02#")) {
		m_ui->labelTransConnect->setMovie(NULL);
		m_ui->labelTransConnect->setPixmap(QPixmap(QLatin1String(":/images/ui/call_connected.png")));
		m_ui->labelCallStatus->setText(tr("Calling destination party..."));
		m_ui->labelCallDuration->setText(tr("Calling destination party..."));
	} else {
		m_callConnecting->stop();
		m_ui->labelTransConnect->setMovie(NULL);
		m_ui->labelCalleeConnect->setMovie(NULL);
		m_ui->labelTransConnect->setPixmap(QPixmap(QLatin1String(":/images/ui/call_connected.png")));
		m_ui->labelCalleeConnect->setPixmap(QPixmap(QLatin1String(":/images/ui/call_connected.png")));
		m_ui->callControlPanel->callConnected();
		m_duration = 0;
		if (m_durationTimer)
			m_durationTimer->start();
	}
}

void PhoneCallingView::callClosed()
{
	m_callConnecting->stop();

	if (m_durationTimer)
		m_durationTimer->stop();

	m_duration = 0;
	m_ui->labelCallStatus->setText(QString());
	m_ui->labelCallDuration->setText(QString());

	m_ui->callControlPanel->callClosed();
	m_ui->labelCallDuration->setText(QString());

	qApp->call()->setCallID(-1);
	qApp->call()->setCalleeName(QString());
}

void PhoneCallingView::callConfirmed()
{
	if (m_sipService) {
		int volume = m_sipService->callVolume(qApp->call()->callID());
		m_ui->callControlPanel->setCallVolume(volume);
	}

	if (qApp->call()->direction() == Call::DirectionIn) {
		m_ui->frameCallerPanel->show();
		m_ui->labelCalleeConnect->show();
		m_ui->labelCalleeConnect->setMovie(NULL);
		m_ui->labelCalleeConnect->setPixmap(QPixmap(QLatin1String(":/images/ui/call_connected.png")));
		m_ui->callControlPanel->callConnected();
		m_duration = 0;
		if (m_durationTimer)
			m_durationTimer->start();
	}
}

void PhoneCallingView::showMinimized(bool customerized)
{
	m_ui->frameCallPhotoContainer->hide();
	m_ui->labelCallStatus->hide();

	m_ui->labelCallDuration->show();
	m_ui->pushButtonMaximize->show();

	show();
}

void PhoneCallingView::setUserPhoto(const QString& photo)
{
	m_ui->labelCallerPhoto->setPixmap(QPixmap(photo));
}

void PhoneCallingView::changeEvent(QEvent* event)
{
	AbstractPhoneView::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

void PhoneCallingView::slotMute(bool mute)
{
	if (m_sipService && qApp->call()->callID()!=-1) {
		m_sipService->muteCall(qApp->call()->callID(), mute);
	}
}

void PhoneCallingView::slotSpeaker(bool open)
{

}

void PhoneCallingView::slotHangup()
{
	if (m_sipService) {
		m_sipService->hangupCall(qApp->call()->callID());
	}
}

void PhoneCallingView::slotDTMF(const QChar& c)
{
	if (m_sipService && qApp->call()->callID()!=-1) {
		m_sipService->sendDtmf(qApp->call()->callID(), c);
	}
}

void PhoneCallingView::slotAnswer()
{
	if (m_sipService) {
		m_sipService->answerCall(qApp->call()->callID());
	}
}

void PhoneCallingView::slotReject()
{
	if (m_sipService) {
		m_sipService->rejectCall(qApp->call()->callID());
	}
}

void PhoneCallingView::slotVolumeChanged(int value)
{
	if (m_sipService && qApp->call()->callID()!=-1) {
		m_sipService->setCallVolume(qApp->call()->callID(), (unsigned)value);
	}
}

void PhoneCallingView::slotDuration()
{
	++m_duration;

	const QTime currentTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60);
	QString format;
	if (m_duration > 3600) {
		format = QLatin1String("hh:mm:ss");
	} else {
		format = QLatin1String("mm:ss");
	}
	m_ui->labelCallStatus->setText(tr("%1").arg(currentTime.toString(format)));
	m_ui->labelCallDuration->setText(tr("%1").arg(currentTime.toString(format)));


	if (m_sipService) {
		float rxLossPercent, txLossPercent;
		m_sipService->callQuality(qApp->call()->callID(), &rxLossPercent, &txLossPercent);
		m_ui->callControlPanel->setCallQuality(rxLossPercent);
	}
}

void PhoneCallingView::retranslateTr()
{

}

} // namespace Qpid
