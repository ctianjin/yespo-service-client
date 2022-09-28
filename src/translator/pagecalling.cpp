#include <QResizeEvent>
#include <QTime>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QDebug>
#include <QBitmap>
#include <QMovie>
#include <QPainter>
#include <QVideoWidget>
#include "pagecalling.h"
#include "ui_pagecalling.h"
#include "phoneapplication.h"
#include "userservice.h"
#include "sipservice.h"
#include "cameraservice.h"
#include "widgetphototranslation.h"
#include "yespoproto.h"
#include "remotevideodialog.h"
#include "callmode.h"

#define CAPTURE 5

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PageCalling:"
#endif // DEBUG_PREFIX

namespace ns {

const QString PageCalling::styleVoiceFull = QLatin1String("border-image: url(:/images/ui/bi_calling_voice_full.png);");
const QString PageCalling::styleVoiceHalf = QLatin1String("border-image: url(:/images/ui/bi_calling_voice_half.png);");
const QString PageCalling::styleVoiceQuarter = QLatin1String("border-image: url(:/images/ui/bi_calling_voice_quarter.png);");
const QString PageCalling::styleVoiceEmpty = QLatin1String("border-image: url(:/images/ui/bi_calling_voice_empty.png);");

PageCalling::PageCalling(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_PageCalling),
	m_callId(-1),
	m_sourceLang(),
	m_targetLang(),
	m_callMode(),
	m_serviceMode(),
	m_remoteContact(),
	m_localCountry(),
	m_localLanguage(),
	m_duration(0),
	m_movieNum(0),
	m_receiveText(),
	m_cameraStatus(Normal),
	m_state(-1),
	m_videoEnabled(false),
	m_pixmapCapture(),
	m_remoteVideoDialog(new RemoteVideoDialog)
{
    m_ui->setupUi(this);
	
	m_ui->labelCalleePhoto->setRadius(5.0,5.0);
	m_ui->labelMePhoto->setRadius(7.0,7.0);
	m_ui->labelCallerPhoto->setRadius(5.0,5.0);

	QRect labelRect = m_ui->labelMePhoto->rect();

	QPainterPath myPath;
	myPath.setFillRule(Qt::WindingFill);
	myPath.addEllipse(labelRect);
	QPixmap pixLabel(labelRect.size());

	QPainter painterLabel(&pixLabel);
	painterLabel.fillRect(pixLabel.rect(), Qt::white);
	painterLabel.setBrush(Qt::black);
	painterLabel.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
	painterLabel.fillPath(myPath, Qt::black);
	m_ui->labelMePhoto->setMask(pixLabel.createMaskFromColor(Qt::white));

// 	m_topPanel = new QLabel(m_ui->widgetCallTop);
// 	m_topPanel->setGeometry(0, 0, width(), 44);
// 	m_topPanel->setStyleSheet(QLatin1String("background: rgba(255,255,255,50);color: #005995;"));
// 	m_topPanel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

	m_labelValueIcon = new QLabel(m_ui->widgetVoice);
	m_labelValueIcon->setMinimumSize(QSize(24, 24));
	m_labelValueIcon->setMaximumSize(QSize(24, 24));
	m_labelValueIcon->setObjectName(QLatin1String("labelVoiceIcon"));
	m_labelValueIcon->setStyleSheet(QLatin1String("QLabel#labelVoiceIcon{border-image: url(:/images/ui/bi_calling_voice_value.png)}"));
	m_labelValueIcon->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

	m_ui->sliderVoice->setValue(m_ui->sliderVoice->maximum() / 2);
	m_labelValueIcon->hide();

 	m_movie = new QMovie(QLatin1String(":/images/ui/loading.gif"));

	m_durationTimer = new QTimer(this);
	m_durationTimer->setInterval(1000);
	connect(m_durationTimer, SIGNAL(timeout()), this, SLOT(slotDuration()));

	m_movieTimer = new QTimer(this);
	m_movieTimer->setInterval(1000);
	connect(m_movieTimer, SIGNAL(timeout()), this, SLOT(slotChangeImage()));

	m_ui->widgetPhoto->hide();
	m_ui->widgetLanguage->hide();
/*	m_ui->widgetCallBottom->adjustSize();*/

	m_remoteVideoDialog->hide();
}

PageCalling::~PageCalling()
{
	 
}

UserService* PageCalling::userService() const
{
	if (m_userService)
		return m_userService;
	return NULL;
}

void PageCalling::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;

	if (m_userService) {
	}
}

SipService* PageCalling::sipService() const
{
	if (m_sipService)
		return m_sipService;
	return NULL;
}

void PageCalling::setSipService(SipService* sipService)
{
	if (m_sipService) {
		//disconnect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)), this, SLOT(slotReceiveText(int, const QString&, const QString&)));
	}

	m_sipService = sipService;
	m_sipService->setCodecSize(m_remoteVideoDialog->widgetVideo()->width(), m_remoteVideoDialog->widgetVideo()->height());
	m_sipService->setVideoWindow((void *)m_ui->labelMePhoto->winId(), (void *)m_remoteVideoDialog->widgetVideo()->winId());

	if (m_sipService) {
		//connect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)), this, SLOT(slotReceiveText(int, const QString&, const QString&)));
	}
}

CameraService* PageCalling::cameraService() const
{
	if (m_cameraService)
		return m_cameraService;
	return NULL;
}

void PageCalling::setCameraService(CameraService* cameraService)
{
 	if (m_cameraService) {
 		disconnect(m_cameraService, SIGNAL(cameraLoaded()), this, SLOT(slotCameraError()));
 		disconnect(m_cameraService, SIGNAL(cameraActive()), this, SLOT(slotCameraActive()));
 	}
 
 	m_cameraService = cameraService;
 
 	if (m_cameraService) {
 		connect(m_cameraService, SIGNAL(cameraLoaded()), this, SLOT(slotCameraError()));
 		connect(m_cameraService, SIGNAL(cameraActive()), this, SLOT(slotCameraActive()));
 	}
}

WidgetPhotoTranslation* PageCalling::widgetPhotoTranslation() const 
{
	if (m_widgetPhotoTranslation)
		return m_widgetPhotoTranslation;
	return NULL;
}

void PageCalling::setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation)
{
	m_widgetPhotoTranslation = widgetPhotoTranslation;
	m_ui->widgetPhoto->show();
	m_widgetPhotoTranslation->setParent(m_ui->widgetPhoto);
	m_widgetPhotoTranslation->setGeometry(0, 0, m_ui->widgetPhoto->width(), m_ui->widgetPhoto->height());
	m_widgetPhotoTranslation->show();
	m_ui->widgetLanguage->show();
}

YespoProto* PageCalling::proto() const 
{
	if (m_proto)
		return m_proto;
	return NULL;
}

void PageCalling::setProto(YespoProto* proto)
{
	if (m_proto) {
		disconnect(m_proto, SIGNAL(receiveVideoStatus(int)), this, SLOT(slotReceiveVideoStatus(int)));
		disconnect(m_proto, SIGNAL(receiveCallStatus(int)), this, SLOT(slotReceiveStatus(int)));
	}

	m_proto = proto;

	if (m_proto) {
		connect(m_proto, SIGNAL(receiveVideoStatus(int)), this, SLOT(slotReceiveVideoStatus(int)));
		connect(m_proto, SIGNAL(receiveCallStatus(int)), this, SLOT(slotReceiveStatus(int)));
	}
}

CallMode* PageCalling::callMode() const 
{
	if (m_widgetCallMode)
		return m_widgetCallMode;
	return NULL;
}

void PageCalling::setCallMode(CallMode* callMode)
{
	if (m_widgetCallMode) {
	}

	m_widgetCallMode = callMode;

	if (m_widgetCallMode) {
	}
}

QWidget* PageCalling::widgetLocalVideo() const 
{
	return m_ui->labelMePhoto;
}

void PageCalling::setCallId(int callId)
{
	m_callId = callId;
}

void PageCalling::setLocalCountryLang(const QString& localCountry, const QString& localLanguage)
{

}

void PageCalling::setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile)
{
	if (m_remoteContact.contains(callerId)) {
		m_ui->labelCallerName->setText(displayName);
		m_ui->labelCallerDetail->setText(tr("%1").arg(mobile));
		m_callerPhoto = photo;

		m_callerDisplayName = displayName;
		m_callerMobile = mobile;

	} else {
		m_ui->labelCalleeName->setText(displayName);
		m_ui->labelCalleeDetail->setText(tr("%1").arg(mobile));
		m_calleePhoto = photo;

		m_calleeDisplayName = displayName;
		m_calleeMobile = mobile;
	}
}

void PageCalling::setPhotoPicture(const QString& photoLink, const QString& photoFile)
{
	QPixmap pic(photoFile);
	QPixmap mask = QLatin1String(":/images/ui/home_bg_mask.png");
	if (photoLink == m_callerPhoto) {
		m_ui->labelCallerPhoto->setMaskPixmap(pic.scaled(m_ui->labelCallerPhoto->size()),mask);
	} else if (photoLink == m_calleePhoto){
		m_ui->labelCalleePhoto->setMaskPixmap(pic.scaled(m_ui->labelCalleePhoto->size()),mask);
	} 
}

void PageCalling::cleanReceiveText()
{
	m_receiveText = QString();
	m_videoStatus = -1;
	m_callStatus = -1;
}

void PageCalling::setRemoteVideoWidget(RemoteVideoDialog* remote)
{
	if (m_remoteVideoDialog) {
		disconnect(m_remoteVideoDialog, SIGNAL(userCancelled()), this, SLOT(slotUserCancelled()));
	}

	m_remoteVideoDialog = remote;

	if (m_remoteVideoDialog) {
		connect(m_remoteVideoDialog, SIGNAL(userCancelled()), this, SLOT(slotUserCancelled()));
	}
}

bool PageCalling::videoEnabled()
{
	return m_videoEnabled;
}

bool PageCalling::capture(const QString& fileName)
{
	m_pixmapCapture = QPixmap();

	m_pixmapCapture = QPixmap::grabWindow(m_ui->labelMePhoto->winId());

	if (!m_pixmapCapture.isNull()) {
		if (!fileName.isEmpty()) {
			QString format = QLatin1String("jpg");
			if (m_pixmapCapture.save(fileName, format.toAscii().data())) 
				return true;
		}
	}

	return false;
}

void PageCalling::setLanguage(const QString& srcLang, const QString& targetLang)
{

}

void PageCalling::setCallMode(const QString& mode)
{
	m_callMode = mode;
	if (m_callMode == QLatin1String("a1")) {
		m_ui->widgetCallee->hide();
	} else {
		m_ui->widgetCallee->show();
	}
	
	if (m_widgetCallMode)
		m_widgetCallMode->show();
/*	m_topPanel->show();*/
}

void PageCalling::setServiceMode(const QString& mode)
{
	m_serviceMode = mode;
}

void PageCalling::setRemoteContact(const QString& contact)
{
	m_remoteContact = contact;
}

void PageCalling::initCall(bool local)
{
 	if(m_movie) {
 		m_ui->labelMePhoto->setMovie(m_movie);
 		m_movie->start();
 	}
	if (m_durationTimer) {
		m_durationTimer->start();
	}
	if (m_widgetCallMode) {
		m_widgetCallMode->setParent(this);
		m_widgetCallMode->setGeometry(0, 0, width(), 40);
		m_widgetCallMode->show();
	}

	if (m_callMode == QLatin1String("a1")) {
		/*m_topPanel->setText(tr("Conversation with the caller"));*/
		m_widgetCallMode->setCallingStatusCode(tr("Conversation with the caller"));
		QPixmap pic = QLatin1String(":/images/ui/calling_4.png");
		m_ui->labelCallerForward->setPixmap(pic);
	} else {
		/*m_topPanel->setText(tr("Connecting the third party, please waiting..."));*/
		m_widgetCallMode->setCallingStatusCode(tr("Connecting the third party, please waiting..."));
		if (m_movieTimer) {
			m_movieTimer->start();
		}
	}

	if (m_cameraService) {

 		QList<QByteArray> cameraDevices = m_cameraService->cameraDevices();
 		if (cameraDevices.size()>0) {
			m_state = m_cameraService->state();
 			if (m_state >= 1) {
 				m_movie->stop();
				m_cameraService->setState(++m_state);
				m_ui->labelMePhoto->setText(tr("Unable to open the camera , try closing other programs may use the camera"));
				return ;
			}

 			m_cameraService->setVideoWidget(m_ui->labelMePhoto);
 			QString deviceName = QString::fromLocal8Bit(cameraDevices[0].constData());
 			m_cameraService->start( deviceName );
 		} else {
 			m_movie->stop();
 			m_ui->labelMePhoto->setText(tr("Can not find camera!"));
			m_cameraStatus = Miss;
 		}
	}

	if (m_sipService && m_callId != -1) {
		m_sipService->setCallVolume(m_callId, 128);
		m_ui->sliderVoice->setValue(m_ui->sliderVoice->maximum() / 2);
		m_ui->labelVoiceSliderIcon->setStyleSheet(QLatin1String("#labelVoiceSliderIcon{border-image: url(:/images/ui/bi_calling_voice_half.png);}"));
		if (m_labelValueIcon)
			m_labelValueIcon->hide();
	}
}

void PageCalling::uninitCall()
{
	if (m_durationTimer) {
		m_durationTimer->stop();
		m_duration = 0;
	}

	if (m_movie) {
		m_movie->stop();
	}

	if(m_movieTimer) {
		m_movieTimer->stop();
		m_movieNum = 0;
	}

	if(m_cameraService) {
		int state = m_cameraService->state();

		if (state <= 1) {
			m_cameraService->stop();
			m_cameraService->setState(0);
		} else {
			m_cameraService->setState(--state);
		} 
	}

	m_ui->labelDuration->setText(QLatin1String("00:00:00"));

	if(!m_ui->pushButtonHangup->isEnabled())
		m_ui->pushButtonHangup->setEnabled(true);

	m_cameraStatus = Normal;

	m_ui->widgetPhoto->hide();
	m_ui->widgetLanguage->hide();

	if (m_remoteVideoDialog) {
		m_videoEnabled = false;
		m_remoteVideoDialog->resetCommonControls();
		m_remoteVideoDialog->stopDuration();
		m_remoteVideoDialog->stopNetworkDetect();
	}

	if (m_sipService) {
		m_sipService->setCallVolume(m_callId, 128);
		m_ui->labelVoiceSliderIcon->setStyleSheet(styleVoiceHalf);
	}

	if (m_widgetCallMode)
		m_widgetCallMode = 0;
}

void PageCalling::changeEvent(QEvent * event)
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

void PageCalling::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if (event->size() != event->oldSize()) {
		if (m_widgetCallMode)
			m_widgetCallMode->setGeometry(0, 0, width(),40);
	}
}

void PageCalling::on_pushButtonHangup_clicked()
{
	m_ui->pushButtonHangup->setEnabled(false);
	if (m_sipService) {
		m_sipService->hangupCall(m_callId);
	}
	if (m_proto) {
		m_proto->callCancel();
	}
}

void PageCalling::slotDuration()
{
	++m_duration;

 	const QTime currentTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60);
 	m_ui->labelDuration->setText(currentTime.toString(QLatin1String("hh:mm:ss")));
}

void PageCalling::slotCameraError()
{
	m_cameraStatus = Loaded;
	m_movie->stop();
	m_ui->labelMePhoto->setText(tr("Unable to open the camera , try closing other programs may use the camera"));
}

void PageCalling::slotCameraActive()
{
	m_cameraStatus = Active;
	m_movie->stop();
}

void PageCalling::slotReceiveText(int callId, const QString& remote, const QString& text)
{
	Q_UNUSED(remote);
	
	m_receiveText = text;
	if (/*m_topPanel*/ m_widgetCallMode && (text == QLatin1String("*#03#")) && m_callMode != QLatin1String("a1")) {
		/*m_topPanel->setText(tr("three_way calling..."));*/
		m_widgetCallMode->setCallingStatusCode(tr("three_way calling..."));
		m_movieTimer->stop();
		QPixmap pic = QLatin1String(":/images/ui/calling_4.png");
		m_ui->labelCallerForward->setPixmap(pic);
		m_ui->labelMeForward->setPixmap(pic);
	}

	/* Video interaction message */
	if (text == QLatin1String("uaVideoReqStart")) {
		m_videoEnabled = true;
		
		if (m_sipService) {
 			m_sipService->stopPsdVideoStream();
 			/*m_sipService->prepareVideoStream();*/
			m_sipService->setVideoIncoming(m_callId, true);
			m_sipService->prepareVideoStream();
			m_sipService->setVideoOpenWin();
		}

		/*m_topPanel->setText(tr("The caller has open the video"));*/
		m_widgetCallMode->setCallingStatusCode(tr("The caller has open the video"));

		Q_EMIT showVideo();
	}

	if (text == QLatin1String("uaVideoReqStop")) {
		m_videoEnabled = false;

		if (m_sipService)
			m_sipService->setVideoCloseWin();

		if (m_remoteVideoDialog)
			m_remoteVideoDialog->closeWindow();

		/*m_topPanel->setText(tr("The caller has close the video"));*/
		m_widgetCallMode->setCallingStatusCode(tr("The caller has close the video"));
	}

}

void PageCalling::slotChangeImage()
{
	++m_movieNum;
	if(m_movieNum > 3) {
		m_movieNum = 0;
		return;
	}
	QString srcImage = QLatin1String(":/images/ui/calling_") + QString::number(m_movieNum) + QLatin1String(".png");
	m_ui->labelCallerForward->setPixmap(srcImage);
	m_ui->labelMeForward->setPixmap(srcImage);
}

void PageCalling::slotReceiveStatus(int callStatus)
{
	qDebug() << "current status: " << callStatus;
	m_callStatus = callStatus;
	if (m_widgetCallMode) {
		if (callStatus == 3 ) {
			if (m_callMode == QLatin1String("a1")) {
				m_widgetCallMode->setCallingStatusCode(tr("Conversation with the caller"));
			} else if (m_callMode == QLatin1String("a2")) {
				m_widgetCallMode->setCallingStatusCode(tr("three_way calling..."));
				if (m_movieTimer && m_movieTimer->isActive())
					m_movieTimer->stop();	
				QPixmap pic = QLatin1String(":/images/ui/calling_4.png");
				m_ui->labelCallerForward->setPixmap(pic);
				m_ui->labelMeForward->setPixmap(pic);
			}
		} 
	}
}

void PageCalling::slotReceiveVideoStatus(int status)
{
	m_videoStatus = status;
	if (status == 1) {
		m_videoEnabled = true;

		if (m_sipService) {
			m_sipService->stopPsdVideoStream();
			m_sipService->prepareVideoStream();
			m_sipService->setVideoOpenWin();
		}

		if (m_widgetCallMode)
			m_widgetCallMode->setCallingStatusCode(tr("The caller has open the video"));

		Q_EMIT showVideo();
	}

	if (status == 2) {
		m_videoEnabled = false;

		if (m_remoteVideoDialog)
			m_remoteVideoDialog->closeWindow();

		if (m_widgetCallMode)
			m_widgetCallMode->setCallingStatusCode(tr("The caller has close the video"));
	}
}

void PageCalling::slotUserCancelled()
{
	if (m_sipService) {
		m_sipService->setVideoIncoming(m_callId, false);
	}
}

void PageCalling::on_sliderVoice_sliderPressed()
{
	if (m_labelValueIcon) {
		m_labelValueIcon->setText(QString::number(m_ui->sliderVoice->value()));
		m_labelValueIcon->setGeometry(m_ui->labelVoiceSliderIcon->width() + m_ui->widgetVoice->layout()->spacing() + m_ui->sliderVoice->value(), m_ui->sliderVoice->geometry().top() - 22,
			m_labelValueIcon->width(), m_labelValueIcon->height());
		m_labelValueIcon->show();
	}
}

void PageCalling::on_sliderVoice_sliderReleased()
{
	if (m_labelValueIcon->isVisible())
		m_labelValueIcon->hide();
}

void PageCalling::on_sliderVoice_sliderMoved(int value)
{
	if (m_labelValueIcon) {
		m_labelValueIcon->setText(QString::number(value));
		m_labelValueIcon->setGeometry(m_ui->labelVoiceSliderIcon->width() + m_ui->widgetVoice->layout()->spacing() + value, m_ui->sliderVoice->geometry().top() - 22,
			m_labelValueIcon->width(), m_labelValueIcon->height());
		m_labelValueIcon->show();
	}
}

void PageCalling::on_sliderVoice_valueChanged(int value)
{
	if (m_sipService && m_callId != -1) {
		if (value == m_ui->sliderVoice->minimum())
			m_ui->labelVoiceSliderIcon->setStyleSheet(styleVoiceEmpty);
		else if (value <= m_ui->sliderVoice->maximum() / 3 * 1)
			m_ui->labelVoiceSliderIcon->setStyleSheet(styleVoiceQuarter);
		else if (value <= m_ui->sliderVoice->maximum() / 3 * 2)
			m_ui->labelVoiceSliderIcon->setStyleSheet(styleVoiceHalf);
		else if (value <= m_ui->sliderVoice->maximum())
			m_ui->labelVoiceSliderIcon->setStyleSheet(styleVoiceFull);
		m_sipService->setCallVolume(m_callId, value * 2.5);
	}
}

void PageCalling::retranslateTr()
{
	switch(m_cameraStatus) {
		case Miss:
			m_ui->labelMePhoto->setText(tr("Can not find camera!"));
			break;
		case Loaded:
			m_ui->labelMePhoto->setText(tr("Unable to open the camera , try closing other programs may use the camera"));
			break;
		default:
			break;
	}
	m_ui->labelCallerName->setText(m_callerDisplayName);
	m_ui->labelCallerDetail->setText(tr("%1").arg(m_callerMobile));

	if (m_callMode != QLatin1String("a1")) {
		m_ui->labelCalleeName->setText(m_calleeDisplayName);
		m_ui->labelCalleeDetail->setText(tr("%1").arg(m_calleeMobile));
	} 

// 	m_ui->labelSource->setText(qApp->languageFull(m_sourceLang));
// 	m_ui->labelTarget->setText(qApp->languageFull(m_targetLang));
// 
// 	m_ui->labelCountryLangValue->setText(tr("%1/%2").arg(m_localCountry).arg(m_localLanguage));

	if (m_widgetCallMode) {
		if (m_callMode == QLatin1String("a1")) {
			m_widgetCallMode->setCallingStatusCode(tr("Conversation with the caller"));
		} else if (m_receiveText == QLatin1String("*#03#")) {
			m_widgetCallMode->setCallingStatusCode(tr("three_way calling..."));
		} else if (m_receiveText == QLatin1String("uaVideoReqStart")) {
			m_widgetCallMode->setCallingStatusCode(tr("The caller has open the video"));
		} else if (m_receiveText == QLatin1String("uaVideoReqStop")) {
			m_widgetCallMode->setCallingStatusCode(tr("The caller has close the video"));
		} else {
			m_widgetCallMode->setCallingStatusCode(tr("Connecting the third party, please waiting..."));
		}
	}

}

} // namespace ns
