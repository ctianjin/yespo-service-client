#include <QDebug>
#include <QFile>
#include <QMovie>
#include <QTimer>
#include <QResizeEvent>
#include <QPainter>
#include <QBitmap>
#include "viewincomingcall.h"
#include "ui_viewincomingcall.h"
#include "phoneapplication.h"
#include "sipservice.h"
#include "audioservice.h"
#include "widgetphototranslation.h"
#include "yespoproto.h"
#include "callmode.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "ViewIncomingCall:"
#endif // DEBUG_PREFIX

namespace ns {

struct ViewIncomingCallOptions
{
	static const QString close_hint_;
	static const QString call_hint_;
};
const QString ViewIncomingCallOptions::close_hint_ = QLatin1String("close_hint");
const QString ViewIncomingCallOptions::call_hint_ = QLatin1String("call_hint");

ViewIncomingCall::ViewIncomingCall(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ViewIncomingCall),
	m_accountId(-1),
	m_callId(-1),
	m_sourceLang(),
	m_targetLang(),
	m_callMode(),
	m_serviceMode(),
	m_remoteContact(),
	m_localCountry(),
	m_localLanguage(),
	m_ringingTone(0),
	m_movieNum(0)
{
    m_ui->setupUi(this);

	m_widgetCallMode = new CallMode(m_ui->widgetCallPanel);

	m_ringingTone = new QFile(QLatin1String(":/sounds/ringing.pcm"), this);
	if (!m_ringingTone->open(QIODevice::ReadOnly)) {
		qWarning() << DEBUG_PREFIX << "Open ringing tone file failed.";
	}

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()),	this, SLOT(slotNextAnimationFrame()));

	m_ui->widgetCallee->hide();
	m_ui->labelMovie->hide();
	m_ui->widgetTranslationLanguage->hide();
}

ViewIncomingCall::~ViewIncomingCall()
{
	 
}

SipService* ViewIncomingCall::sipService() const
{
	return m_sipService;
}

void ViewIncomingCall::setSipService(SipService* sipService)
{
	if (m_sipService) {
	}

	m_sipService = sipService;

	if (m_sipService) {
	}
}

AudioService* ViewIncomingCall::audioService() const
{
	return m_audioService;
}

void ViewIncomingCall::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
	}

	m_audioService = audioService;

	if (m_audioService) {
	}
}

WidgetPhotoTranslation* ViewIncomingCall::widgetPhotoTranslation() const 
{
	return m_widgetPhotoTranslation;
}

void ViewIncomingCall::setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation)
{	
	m_widgetPhotoTranslation = widgetPhotoTranslation;

	m_ui->widgetTranslationLanguage->setStyleSheet(QLatin1String("#widgetTranslationLanguage { background: rgb(0, 0, 0, 30%); border-bottom: 1px solid #33ADE7;}"));
	m_ui->widgetTranslationLanguage->show();
	m_widgetPhotoTranslation->setParent(m_ui->widgetTranslationLanguage);
	m_widgetPhotoTranslation->setGeometry((m_ui->widgetTranslationLanguage->width() - m_widgetPhotoTranslation->width()) / 2,
		0, m_widgetPhotoTranslation->width(), m_widgetPhotoTranslation->height());
	m_widgetPhotoTranslation->show();
}

YespoProto* ViewIncomingCall::proto() const 
{
	if (m_proto)
		return m_proto;
	return NULL;
}

void ViewIncomingCall::setProto(YespoProto* proto)
{
	if (m_proto) {

	}

	m_proto = proto;

	if (m_proto) {

	}
}

CallMode* ViewIncomingCall::callMode() const 
{
	if (m_widgetCallMode)
		return m_widgetCallMode;
	return NULL;
}

void ViewIncomingCall::setCallMode(CallMode* callMode)
{
	if (m_widgetCallMode) {

	}

	m_widgetCallMode = callMode;
	m_widgetCallMode->setParent(this);

	if (m_widgetCallMode) {

	}
}

void ViewIncomingCall::setCallId(int callId)
{
	m_callId = callId;
}

int ViewIncomingCall::callId()
{
	return m_callId;
}

void ViewIncomingCall::setCallState(const QString& state)
{
	m_callState = state;
}

void ViewIncomingCall::setLocalCountryLang(const QString& localCountry, const QString& localLanguage)
{
//	m_ui->widgetLocal->show();
	m_localLanguage = localLanguage;
	m_localCountry = localCountry;
	
//	m_ui->labelLocalLang->setText(tr("Country and language:"));
//	m_ui->labelLocalLangValue->setText(tr("%1/%2").arg(localCountry).arg(localLanguage));

	m_ui->widgetTranslationLanguage->hide();

	m_ui->widgetBottom->setStyleSheet(QLatin1String("#widgetBottom { border-image: url(:/images/ui/bg_local.png); }"));
//	m_ui->widgetLocal->setStyleSheet(QLatin1String("#widgetLocal { background: rgb(0, 0, 0, 30%); border-bottom: 1px solid #339999; border-bottom: 1px solid #339999;}"));
	m_ui->widgetCall->setStyleSheet(QLatin1String("#widgetCall { background: rgb(0, 0, 0, 20%); }"));
	m_ui->labelCallHint->setStyleSheet(QLatin1String("#labelCallHint { background: rgb(0, 0, 0, 20%); }"));
}

void ViewIncomingCall::setLanguage(const QString& srcLang, const QString& targetLang)
{
#if 0
	m_ui->labelCountryLang->hide();
	m_ui->labelCountryLangValue->hide();
	m_sourceLang = srcLang;
	m_targetLang = targetLang;

	QString srcImage = QLatin1String(":/images/ui/") + srcLang + QLatin1String(".png");
	QString targetImage = QLatin1String(":/images/ui/") + targetLang + QLatin1String(".png");

	m_ui->labelSourceFlag->setStyleSheet(QString(QLatin1String("border-image: url(%1)")).arg(srcImage));
	m_ui->labelTargetFlag->setStyleSheet(QString(QLatin1String("border-image: url(%1)")).arg(targetImage));

	m_ui->labelSource->setText(qApp->languageFull(srcLang));
	m_ui->labelTarget->setText(qApp->languageFull(targetLang));

	m_ui->widgetSource->show();
	m_ui->widgetTarget->show();

	m_ui->widgetSource->adjustSize();
	m_ui->widgetTarget->adjustSize();	
#endif

//	m_ui->widgetLocal->hide();

 	m_sourceLang = srcLang;
 	m_targetLang = targetLang;

	if (m_widgetCallMode) {
		m_widgetCallMode->setCallingStatusCode(QString());
		m_widgetCallMode->setLanguage(srcLang, targetLang);
		//m_widgetCallMode->setStyleSheet(QLatin1String("widgetBottom{background: rgba(255,255,255,50);}"));
	}

	//m_ui->widgetBottom->setStyleSheet(QLatin1String("#widgetBottom { background: url(:/images/ui/bg_processing.png); }"));
	//m_ui->widgetTranslationLanguage->setStyleSheet(QLatin1String("#widgetTranslationLanguage { background: rgb(0, 0, 0, 30%); border-bottom: 1px solid #33ADE7;}"));
	m_ui->widgetCall->setStyleSheet(QLatin1String("#widgetCall { background: rgb(0, 0, 0, 20%); }"));
	m_ui->labelCallHint->setStyleSheet(QLatin1String("#labelCallHint { background: rgb(0, 0, 0, 20%); }"));
}

void ViewIncomingCall::setCallMode(const QString& mode)
{
	m_callMode = mode;
	if (m_callMode == QLatin1String("a1")) {
		m_ui->widgetCallee->hide();
		m_ui->labelMovie->hide();
	} else {
		m_ui->widgetCallee->show();
		m_ui->labelMovie->show();
	}
	if (m_widgetCallMode)
		m_widgetCallMode->setCallMode(m_callMode);
}

void ViewIncomingCall::setServiceMode(const QString& mode)
{
}

void ViewIncomingCall::setRemoteContact(const QString& contact)
{
	m_remoteContact = contact;
}

void ViewIncomingCall::setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile)
{
	if (m_remoteContact.contains(callerId)) {	
		m_callerDisplayName = displayName;
		m_callerMobile = mobile;
		m_callerPhoto = photo;
		m_ui->labelCallerName->setText(displayName);
		m_ui->labelCallerMobile->setText(mobile);
	} else {
		m_ui->labelCalleeName->setText(displayName);
		m_ui->labelCalleeMobile->setText(mobile);
		m_calleePhoto = photo;

		m_calleeDisplayName = displayName;
		m_calleeMobile = mobile;
	}

// 	if (m_Local) {
// 		//m_topPanel->setText(tr("Call incoming... %1 would like to talk to local people").arg(displayName));
// 		m_topPanelLeft->setText(tr("Service channel : communicate with the locals"));
// 		m_topPanelRight->setText(tr("Language : %1").arg(qApp->languageFull(m_localLanguage)));
// 
// 	} else {
// 		//m_topPanel->setText(tr("Call incoming... %1 wants a translater").arg(displayName));
// 		m_topPanelLeft->setText(tr("Service channel : i need a translator"));
// 		m_topPanelRight->setText(tr("Language : %1").arg(qApp->languageFull(m_targetLang)));
// 	}
}

void ViewIncomingCall::setPhotoPicture(const QString& photoLink, const QString& photoFile)
{
	QPixmap pic(photoFile);
	QPixmap mask = QLatin1String(":/images/ui/home_bg_mask.png");

	if (photoLink == m_callerPhoto) {
		m_ui->labelCallerPhoto->setMaskPixmap(pic.scaled(m_ui->labelCallerPhoto->size()),mask);
	} else if(photoLink == m_calleePhoto){
		m_ui->labelCalleePhoto->setMaskPixmap(pic.scaled(m_ui->labelCalleePhoto->size()),mask);
	} else {
		return;
	}
}

void ViewIncomingCall::callIncoming(int callId)
{
	m_callId = callId;

	if (m_audioService) {
		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));
		connect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));

		typedef ViewIncomingCallOptions Opt;
		Preference* pref = preference();
		if (pref) {
			bool closeHint = pref->getBool(Opt::close_hint_, false);
			bool callHint = pref->getBool(Opt::call_hint_, false);
			if (!closeHint && callHint) {
				m_audioService->startPlay(m_ringingTone);
			} 
		} else {
			m_audioService->startPlay(m_ringingTone);
		}
	}

// 	if(m_timer) {
// 		startTimer();
// 	}

// 	if(m_movieTimer) {
// 		m_movieTimer->start();
// 	}

	if (m_widgetCallMode) {
		m_widgetCallMode->setParent(this);
		m_widgetCallMode->setGeometry(0, 0, width(), 40);
		m_widgetCallMode->show();
	}
}

void ViewIncomingCall::callAnswered()
{
	if (m_audioService) {
		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));

		m_audioService->stopPlay();
	}

	if(m_timer->isActive()) {
		m_timer->stop();
	}

// 	if(m_movieTimer->isActive()) {
// 		m_movieTimer->stop();
// 	}

	if (!m_ui->pushButtonAnswer->isEnabled())
		m_ui->pushButtonAnswer->setEnabled(true);

	if (!m_ui->pushButtonReject->isEnabled())
		m_ui->pushButtonReject->setEnabled(true);

}

void ViewIncomingCall::callDisconnected()
{
	if (m_audioService) {

		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));

		m_audioService->stopPlay();
		m_audioService->setRingStatus(false);
	}

	if(m_timer->isActive()) {
		m_timer->stop();
	}

// 	if(m_movieTimer->isActive()) {
// 		m_movieTimer->stop();
// 	}

	if (!m_ui->pushButtonAnswer->isEnabled())
		m_ui->pushButtonAnswer->setEnabled(true);

	if (!m_ui->pushButtonReject->isEnabled())
		m_ui->pushButtonReject->setEnabled(true);

	if (m_widgetCallMode)
		m_widgetCallMode->hide();

	m_ui->widgetTranslationLanguage->hide();
}

void ViewIncomingCall::startRinging()
{
	if (m_audioService) {
// 		if (m_audioService->isRinging())
// 			return;
// 
// 		if (m_ringed)
// 			return;

		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));
		connect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), 
			this, SLOT(slotRingingChanged(QAudio::State)));

		typedef ViewIncomingCallOptions Opt;
		Preference* pref = preference();
		if (pref) {
			bool closeHint = pref->getBool(Opt::close_hint_, false);
			bool callHint = pref->getBool(Opt::call_hint_, false);
			if (!closeHint && callHint) {
				m_audioService->startPlay(m_ringingTone);
/*				m_ringed = true;*/
			} 
		} else {
			m_audioService->startPlay(m_ringingTone);
/*			m_ringed = true;*/
		}
	}
}

int ViewIncomingCall::callId() const
{
	return m_callId;
}

void ViewIncomingCall::setPreference(Preference* pref)
{
	PreferenceSupport::setPreference(pref);
}

void ViewIncomingCall::startTimer()
{
	m_timer->start(500);
}

void ViewIncomingCall::stopTimer()
{
	m_timer->stop();
}

void ViewIncomingCall::changeEvent(QEvent * event)
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

void ViewIncomingCall::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if (event->size() != event->oldSize()) {
		if (m_widgetCallMode) {
			m_widgetCallMode->setGeometry(0, 0, width() , 40);
		}
	}
}

void ViewIncomingCall::applyPreference(const QSet<QString>& paths, bool force)
{
	typedef ViewIncomingCallOptions Opt;
	Preference* pref = preference();
	if (pref) {

	}
}

void ViewIncomingCall::on_pushButtonAnswer_clicked()
{
	m_ui->pushButtonAnswer->setEnabled(false);
	m_ui->pushButtonReject->setEnabled(false);
	if (m_sipService) {
		m_sipService->answerCall(m_callId);
	} else {
		qDebug() << DEBUG_PREFIX << "The SIP service must initialize first.";
	}
}

void ViewIncomingCall::on_pushButtonReject_clicked()
{
	m_ui->pushButtonAnswer->setEnabled(false);
	m_ui->pushButtonReject->setEnabled(false);
	if (m_sipService) {
		m_sipService->rejectCall(m_callId);
	} else {
		qDebug() << DEBUG_PREFIX << "The SIP service must initialize first.";
	}
	if (m_proto) {
		m_proto->callCancel();
	}
}

void ViewIncomingCall::slotRingingChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:
		if (m_audioService) {
			m_audioService->startPlay(m_ringingTone);
		}
		break;
	}
}

void ViewIncomingCall::slotNextAnimationFrame()
{
	m_ui->widgetMovie->nextAnimationFrame( 30, m_ui->widgetMovie->width());
}

void ViewIncomingCall::slotChangeImage()
{
	++m_movieNum;
	if(m_movieNum > 3) {
		m_movieNum = 0;
		return;
	}
	QString srcImage = QLatin1String(":/images/ui/calling_") + QString::number(m_movieNum) + QLatin1String(".png");
	m_ui->labelMovie->setPixmap(srcImage);
}

void ViewIncomingCall::retranslateTr()
{
	m_ui->labelCallerName->setText(m_callerDisplayName);
	m_ui->labelCallerMobile->setText(m_callerMobile);
	if (m_callMode != QLatin1String("a1")) {
		m_ui->labelCalleeName->setText(m_calleeDisplayName);
		m_ui->labelCalleeMobile->setText(tr("%1").arg(m_calleeMobile));
	} 

// 	if (m_Local) {
// 		m_topPanelLeft->setText(tr("Service channel : communicate with the locals"));
// 		m_topPanelRight->setText(tr("Language : %1").arg(qApp->languageFull(m_localLanguage)));
// 
// 	} else {
// 		m_topPanelLeft->setText(tr("Service channel : i need a translator"));
// 		m_topPanelRight->setText(tr("Language : %1").arg(qApp->languageFull(m_targetLang)));
// 	}
}

} // namespace ns
