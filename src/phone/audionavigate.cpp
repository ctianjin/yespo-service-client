#include <Qdebug>
#include <QFile>
#include <QMovie>
#include <QtMultimedia/QAudioDeviceInfo>
#include "phoneapplication.h"
#include "audionavigate.h"
#include "audioservice.h"
#include "ui_audionavigate.h"

namespace Qpid {

struct AudioNavigateDialogOptions
{
    static const QString playback_;
	static const QString capture_;
};

const QString AudioNavigateDialogOptions::playback_ = QLatin1String("playback");
const QString AudioNavigateDialogOptions::capture_ = QLatin1String("capture");

AudioNavigateDialog::AudioNavigateDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_AudioNavigateDialog),
    m_opt(new AudioNavigateDialogOptions),
	m_sampleAudio(0),
	m_movie(0),
	m_recordStep(RecordNotStart)
{
    m_ui->setupUi(this);

	m_ui->playbackNext->hide();
	m_ui->captureNext->hide();
	m_ui->labelMovie->hide();
   	
	adjustSize();

	/* Bug: This function crops full name of multimedia device on Win32 */
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		m_ui->comboPlayback->addItem(deviceInfo.deviceName(), QVariant::fromValue(deviceInfo));

	if (m_ui->comboPlayback->count() == 0)
		m_ui->pushButtonPlayback->setEnabled(false);

	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
		m_ui->comboCapture->addItem(deviceInfo.deviceName(), QVariant::fromValue(deviceInfo));

	if (m_ui->comboCapture->count() == 0)
		m_ui->pushButtonCapture->setEnabled(false);

}

AudioNavigateDialog::~AudioNavigateDialog()
{
	if (m_audioService) {
		m_audioService->stopPlay();
		m_audioService->stopRecord();
	}
}

AudioService* AudioNavigateDialog::audioService() const
{
	return m_audioService;
}

void AudioNavigateDialog::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotPlaybackChanged(QAudio::State)));
		disconnect(m_audioService, SIGNAL(audioInputStateChanged(QAudio::State)), this, SLOT(slotCaptureChanged(QAudio::State)));
	}

	m_audioService = audioService;

	if (m_audioService) {
		connect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotPlaybackChanged(QAudio::State)));
		connect(m_audioService, SIGNAL(audioInputStateChanged(QAudio::State)), this, SLOT(slotCaptureChanged(QAudio::State)));
	}
}

void AudioNavigateDialog::changeEvent(QEvent* event)
{
    FramelessDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void AudioNavigateDialog::showEvent(QShowEvent* event)
{
	FramelessDialog::showEvent(event);

	m_ui->labelHeadphone->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_left_selected.png)"));
	m_ui->labelMiddle->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_play_selected.png)"));
	m_ui->labelMicrophone->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_right_unselect.png)"));

	m_ui->captureDesp->setText(tr("Hold left-click on the button to speak"));
	m_ui->pushButtonPlayback->setEnabled(true);
	m_ui->playbackDesp->show();
	m_ui->playbackNext->hide();
	m_ui->captureDesp->show();
	m_ui->captureNext->hide();
	m_ui->deviceContainer->setCurrentWidget(m_ui->pagePlayback);
//	m_recordStep = RecordNotStart;
}

void AudioNavigateDialog::slotPlaybackChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::ActiveState:
		if (m_recordStep == RecordStop) {
			m_recordStep = AuditionStart;
			if (m_movie == NULL) {
				m_movie = new QMovie(QLatin1String(":/images/ui/voice_detected.gif"));
				m_ui->labelMovie->setMovie(m_movie);
			}
			m_movie->start();
			m_ui->labelMovie->show();
			m_ui->captureDesp->setText(tr("Speak now..."));
		} else {
			m_ui->pushButtonPlayback->setEnabled(false);
		}
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:			
		if (m_ui->deviceContainer->currentWidget() == m_ui->pagePlayback) {
			m_audioService->stopPlay();
			m_ui->playbackDesp->hide();
			m_ui->playbackNext->show();
		} else {
			m_audioService->stopAudition();
			m_ui->captureDesp->hide();
			m_ui->captureNext->show();

			m_movie->stop();
			m_ui->labelMovie->hide();
		}
		break;
	}
}

void AudioNavigateDialog::slotCaptureChanged(QAudio::State state)
{

	switch(state)
	{
	case QAudio::ActiveState:
		m_recordStep = RecordStart;
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		m_recordStep = RecordStop;
		m_audioService->audition();
		break;
	case QAudio::IdleState:
		break;
	}
}

void AudioNavigateDialog::on_pushButtonPlayback_clicked()
{
	if (m_audioService == NULL)
		return;
	int currentIndex = m_ui->comboPlayback->currentIndex();
	if (currentIndex != -1) {
		QAudioDeviceInfo deviceInfo = m_ui->comboPlayback->itemData(currentIndex).value<QAudioDeviceInfo>();

		if (m_sampleAudio == NULL) {
			m_sampleAudio = new QFile(QLatin1String(":/sounds/ringing.pcm"), this);
			m_sampleAudio->open(QIODevice::ReadOnly);
		}

		m_audioService->setAudioOutput(deviceInfo);
		m_audioService->startPlay(m_sampleAudio);
	}
}

void AudioNavigateDialog::on_pushButtonPlaybackFix_clicked()
{

}

void AudioNavigateDialog::on_pushButtonPlaybackNext_clicked()
{
	on_pushButtonPlaybackSkip_clicked();
}

void AudioNavigateDialog::on_pushButtonPlaybackSkip_clicked()
{
	m_ui->labelHeadphone->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_left_unselect.png)"));
	m_ui->labelMiddle->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_capture_selected.png)"));
	m_ui->labelMicrophone->setStyleSheet(QLatin1String("border-image: url(:/images/ui/step_right_selected.png)"));

	m_ui->deviceContainer->setCurrentWidget(m_ui->pageCapture);
}

void AudioNavigateDialog::on_pushButtonCapture_pressed()
{
	if (m_audioService == NULL)
		return;
	int currentIndex = m_ui->comboCapture->currentIndex();
	if (currentIndex != -1) {
		QAudioDeviceInfo deviceInfo = m_ui->comboCapture->itemData(currentIndex).value<QAudioDeviceInfo>();

		m_audioService->setAudioInput(deviceInfo);
		m_audioService->startRecord();
	}
}

void AudioNavigateDialog::on_pushButtonCapture_released()
{
	if (m_audioService == NULL)
		return;

	m_recordStep = RecordStop;
	m_audioService->stopRecord();
}

void AudioNavigateDialog::on_pushButtonCaptureFix_clicked()
{

}

void AudioNavigateDialog::on_pushButtonCaptureFinish_clicked()
{
	m_audioService->stopPlay();
	accept();
}

} // namespace Qpid
