#include <QDebug>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include <QtEndian>
#include "testvoice.h"
#include "ui_testvoice.h"
#include "phoneapplication.h"
#include "audioservice.h"
#include "voicewidget.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "TestVoice:"
#endif // DEBUG_PREFIX

namespace ns {

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent) : 
	QIODevice(parent),
	m_format(format),
	m_maxAmplitude(0),
	m_level(0.0)
{
	switch (m_format.sampleSize()) 
	{
	case 8:
		switch (m_format.sampleType()) 
		{
		case QAudioFormat::UnSignedInt:
			m_maxAmplitude = 255;
			break;
		case QAudioFormat::SignedInt:
			m_maxAmplitude = 127;
			break;
		default:
			break;
		}
		break;
	
	case 16:
		switch (m_format.sampleType()) 
		{
		case QAudioFormat::UnSignedInt:
			m_maxAmplitude = 65535;
			break;
		case QAudioFormat::SignedInt:
			m_maxAmplitude = 32767;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{
	open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
	close();
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
	Q_UNUSED(data)
	Q_UNUSED(maxlen)

	return 0;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
	if (m_maxAmplitude) {
		Q_ASSERT(m_format.sampleSize() % 8 == 0);
		const int channelBytes = m_format.sampleSize() / 8;
		const int sampleBytes = m_format.channels() * channelBytes;
//		Q_ASSERT(len % sampleBytes == 0);
		const int numSamples = len / sampleBytes;

		quint16 maxValue = 0;
		const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

		for (int i = 0; i < numSamples; ++i) {
			for(int j = 0; j < m_format.channels(); ++j) {
				quint16 value = 0;

				if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
					value = *reinterpret_cast<const quint8*>(ptr);
				} else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
					value = qAbs(*reinterpret_cast<const qint8*>(ptr));
				} else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
					if (m_format.byteOrder() == QAudioFormat::LittleEndian)
						value = qFromLittleEndian<quint16>(ptr);
					else
						value = qFromBigEndian<quint16>(ptr);
				} else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
					if (m_format.byteOrder() == QAudioFormat::LittleEndian)
						value = qAbs(qFromLittleEndian<qint16>(ptr));
					else
						value = qAbs(qFromBigEndian<qint16>(ptr));
				}

				maxValue = qMax(value, maxValue);
				ptr += channelBytes;
			}
		}

		maxValue = qMin(maxValue, m_maxAmplitude);
		m_level = qreal(maxValue) / m_maxAmplitude;
	}
	
	//Q_EMIT update(m_level);
	Q_EMIT update();

	return len;
}

TestVoice::TestVoice(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TestVoice),
	m_level(vlNormal),
	m_sampleAudio(0),
	m_audioInfo(0)
{
    m_ui->setupUi(this);
}

TestVoice::~TestVoice()
{
	 
}

AudioService* TestVoice::audioService() const
{
	return m_audioService;
}

void TestVoice::setAudioService(AudioService* audioService)
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

void TestVoice::startRecord()
{
	if (m_audioService==NULL)
		return;

	if(m_audioInfo) {
		disconnect(m_audioInfo, SIGNAL(update()), this, SLOT(slotVoiceValue()));
		delete m_audioInfo;
		m_audioInfo = 0;
	}

	QAudioFormat fmt;
	fmt.setFrequency(8000);
	fmt.setChannels(1);
	fmt.setSampleSize(16);
	fmt.setSampleType(QAudioFormat::SignedInt);
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setCodec(QLatin1String("audio/pcm"));

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
	if (!info.isFormatSupported(fmt)) {
		qWarning() << DEBUG_PREFIX << "Default format not supported - trying to use nearest";
		fmt = info.nearestFormat(fmt);
	}

	m_audioInfo  = new AudioInfo(fmt, this);
	connect(m_audioInfo, SIGNAL(update()), this, SLOT(slotVoiceValue()));

	m_audioInfo->start();
	m_audioService->startRecord(m_audioInfo);
}

int TestVoice::level() const
{
	return m_level;
}

void TestVoice::startTest()
{
	m_ui->comboBoxSpeaker->clear();
	m_ui->comboBoxMic->clear();

	/* Bug: This function crops full name of multimedia device on Win32 */
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		m_ui->comboBoxSpeaker->addItem(deviceInfo.deviceName(), QVariant::fromValue(deviceInfo));

	if (m_ui->comboBoxSpeaker->count() == 0) {
		m_ui->pushButtonSpeakTest->setEnabled(false);
		m_level |= vlNoSpeaker;
	} else {
		m_ui->pushButtonSpeakTest->setEnabled(true);
	}

	QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
	for(int i = 0; i < devices.size(); ++i) {
		m_ui->comboBoxMic->addItem(devices.at(i).deviceName(), QVariant::fromValue(devices.at(i)));
	}
	connect(m_ui->comboBoxMic, SIGNAL(activated(int)), SLOT(deviceChanged(int)));

	if (m_ui->comboBoxMic->count() == 0) {
		m_level |= vlNoMicrophone;
	}

	if (devices.size() > 0)
		startRecord();
}

void TestVoice::finishTest()
{
 	if (m_audioService) {
 		m_audioService->stopPlay();
 		if(m_audioInfo) {
 			m_audioInfo->stop();
 		}
 		m_audioService->stopRecord();
 	}

	if (m_sampleAudio) {
		m_sampleAudio->deleteLater();
		m_sampleAudio = NULL;
	}
}

void TestVoice::slotPlaybackChanged(QAudio::State state)
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
		if (m_audioService)
			m_audioService->stopPlay();
		m_ui->pushButtonSpeakTest->setEnabled(true);
		break;
	}
}

void TestVoice::slotCaptureChanged(QAudio::State state)
{
	qDebug() << DEBUG_PREFIX << "slotCaptureChanged" << state;
	switch(state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:
		break;
	}
}

void TestVoice::changeEvent(QEvent* event)
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

void TestVoice::on_pushButtonSpeakTest_clicked()
{
	if (m_audioService==NULL)
		return;

	int currentIndex = m_ui->comboBoxSpeaker->currentIndex();
	if (currentIndex != -1) {
		if (m_sampleAudio == NULL) {
			m_sampleAudio = new QFile(QLatin1String(":/sounds/ringing_short.pcm"), this);
			m_sampleAudio->open(QIODevice::ReadOnly);
		}

		QAudioDeviceInfo deviceInfo = m_ui->comboBoxSpeaker->itemData(currentIndex).value<QAudioDeviceInfo>();
		m_audioService->setAudioOutput(deviceInfo);
		m_audioService->startPlay(m_sampleAudio);
		m_ui->pushButtonSpeakTest->setEnabled(false);
	}
}

void TestVoice::deviceChanged(int index)
{
	if (m_audioService == NULL){
		return;
	}

	m_audioInfo->stop();

	m_audioService->stopRecord();

	QAudioDeviceInfo device = m_ui->comboBoxMic->itemData(index).value<QAudioDeviceInfo>();

	m_audioService->setAudioInput(device);

	startRecord();

}

void TestVoice::slotVoiceValue()
{
	//m_ui->progressBarVoiceDetect->setValue(value*4000);
	/*m_ui->voiceDetectWidget->*/
	m_ui->voiceDetectWidget->setLevel(m_audioInfo->level());
	m_ui->voiceDetectWidget->update();
}

} // namespace ns
