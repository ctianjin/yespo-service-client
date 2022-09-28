#include "callcontrolpanel.h"
#include "phoneapplication.h"
#include "ui_callcontrolpanel.h"
#include "popupdtmf.h"
#include "popupvolume.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CallControlPanel:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct CallControlPanelOptions
{

};

CallControlPanel::CallControlPanel(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_CallControlPanel),
    m_opt(new CallControlPanelOptions),
	m_currentQualityImage()
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->frameCallinPanel->hide();

	connect(m_ui->pushButtonMute, SIGNAL(clicked(bool)), this, SIGNAL(mute(bool)));
	connect(m_ui->pushButtonSpeaker, SIGNAL(clicked(bool)), this, SIGNAL(speaker(bool)));
	connect(m_ui->pushButtonHangup, SIGNAL(clicked(bool)), this, SIGNAL(hangup()));
	connect(m_ui->pushButtonAnswer, SIGNAL(clicked(bool)), this, SIGNAL(answer()));
	connect(m_ui->pushButtonReject, SIGNAL(clicked(bool)), this, SIGNAL(reject()));

	m_DTMFPanel = new PopupDTMF(this);
	m_ui->pushButtonDialpad->setPopupWindow(m_DTMFPanel, PopupButton::AlignTop);
	connect(m_DTMFPanel, SIGNAL(digit(const QChar&)), this, SIGNAL(digit(const QChar&)));

	m_volumePanel = new PopupVolume(this);
	m_ui->pushButtonSpeaker->setPopupWindow(m_volumePanel, PopupButton::AlignTop);
	connect(m_volumePanel, SIGNAL(volumeChanged(int)), this, SIGNAL(volume(int)));

	m_currentQualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality.png)");
}

CallControlPanel::~CallControlPanel()
{
	
}

void CallControlPanel::callIn()
{
	m_ui->frameCallinPanel->show();
	m_ui->frameContainer->hide();
	m_ui->pushButtonMute->setChecked(false);
}

void CallControlPanel::callOut()
{
	m_ui->frameCallinPanel->hide();
	m_ui->frameContainer->show();
	m_ui->pushButtonMute->setChecked(false);
}

void CallControlPanel::callConnected()
{
	m_ui->frameCallinPanel->hide();
	m_ui->frameContainer->show();
}

void CallControlPanel::callClosed()
{
	m_DTMFPanel->clear();
	m_DTMFPanel->hide();
	m_volumePanel->hide();
	m_ui->pushButtonMute->setChecked(false);
}

void CallControlPanel::setCallVolume(int volume)
{
	m_volumePanel->setVolume(volume);
}

void CallControlPanel::setCallQuality(float rxLossPercent)
{
	QString qualityImage; 
	if (rxLossPercent-30 > 0.0) { // >30%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_30.png)");
	} else if (rxLossPercent-20 > 0.0) { // <30% and >20%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_20.png)");
	}else if (rxLossPercent-15 > 0.0) { // <20% and >15%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_15.png)");
	}else if (rxLossPercent-10 > 0.0) { // <15% and >10%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_10.png)");
	}else if (rxLossPercent-5 > 0.0) { // <10% and >5%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_5.png)");
	}else if (rxLossPercent-2 > 0.0) { // <5% and >2%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality_2.png)");
	} else { // <2%
		qualityImage = QLatin1String("border-image: url(:/images/ui/call_menu_quality.png)");
	}

	if (m_currentQualityImage != qualityImage) {
		m_ui->pushButtonSignal->setStyleSheet(qualityImage);
		m_currentQualityImage = qualityImage;
	}
}

void CallControlPanel::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void CallControlPanel::slotDTMFClicked()
{
	Q_EMIT digit(0);
}

} // namespace Qpid
