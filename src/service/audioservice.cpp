#include <QDebug>
#include <QFile>
#include <QDir>
#include <qendian.h>
#include <QTemporaryFile>
#include <QTimer>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include "audioservice.h"
#include "audioservice_p.h"
#include "service_p.h"
#include "userservice.h"
#include "servicemanager.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AudioService:"
#endif // DEBUG_PREFIX

#define EXCEED 60

namespace ns {

const QString AudioServiceOptions::audio_input_ = QLatin1String("audio_input");
const QString AudioServiceOptions::audio_output_ = QLatin1String("audio_output");

AudioServiceOptions::AudioServiceOptions() 
{
}

const QString AudioServicePrivate::tempFileTemplate = QLatin1String("audioservice.XXXXXX");

AudioServicePrivate::AudioServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
	m_audioOutput(0),
    m_audioInput(0),
    m_audioInputFile(0),
	m_audioFile(0),
	m_duration(0)
{
	m_format.setSampleRate(8000);
	m_format.setChannelCount(1);
	m_format.setSampleSize(16);
	m_format.setCodec(QLatin1String("audio/pcm"));
	m_format.setByteOrder(QAudioFormat::LittleEndian);
	m_format.setSampleType(QAudioFormat::SignedInt);
}

AudioServicePrivate::~AudioServicePrivate()
{
    cleanup();
}

void AudioServicePrivate::init()
{
    Q_Q(AudioService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

	m_audioTimer = new QTimer();
	m_audioTimer->setInterval(1000);
	QObject::connect(m_audioTimer, SIGNAL(timeout()), q, SLOT(_q_timeout()));

	m_isRinging = false;
}

void AudioServicePrivate::cleanup()
{
	if (m_audioInput) {
		m_audioInput->stop();
		delete m_audioInput;
		m_audioInput = 0;
	}

	if (m_audioInputFile) {
		delete m_audioInputFile;
		m_audioInputFile = 0;
	}

	if (m_audioOutput) {
		m_audioOutput->stop();
		delete m_audioOutput;
		m_audioOutput = 0;
	}

    AbstractServicePrivate::cleanup();
}

void AudioServicePrivate::startRecord()
{
	Q_Q(AudioService);

	Q_ASSERT(isActive());

	setError(Service::NoError);

	if (m_audioInput) {
		m_audioInput->stop();
		QObject::disconnect(m_audioInput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioInputStateChanged(QAudio::State)));
		delete m_audioInput;
		m_audioInput = 0;
	}
	if (m_audioInputFile) {
		delete m_audioInputFile;
		m_audioInputFile = 0;
	}

	if (!m_opt.audioInput.isFormatSupported(m_format)) {
		m_format = m_opt.audioInput.nearestFormat(m_format);
	}

	m_audioInput = new QAudioInput(m_opt.audioInput, m_format, q); 
	if (m_audioInput) {

		QObject::connect(m_audioInput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioInputStateChanged(QAudio::State)));
		QDir dir(QDir::tempPath());
		m_audioInputFile = new QTemporaryFile(dir.absoluteFilePath(tempFileTemplate), q);
		m_audioInputFile->setAutoRemove(true);

		if (!m_audioInputFile->open()) {
			setError(Service::AudioError, m_audioInputFile->errorString());
			return;
		}
		m_audioInput->start(m_audioInputFile);
	}
}

void AudioServicePrivate::startRecord(QIODevice* info)
{
	Q_Q(AudioService);

	Q_ASSERT(isActive());

	setError(Service::NoError);

	if (m_audioInput) {
		m_audioInput->stop();
		QObject::disconnect(m_audioInput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioInputStateChanged(QAudio::State)));
		delete m_audioInput;
		m_audioInput = 0;
	}

	if (!m_opt.audioInput.isFormatSupported(m_format)) {
		m_format = m_opt.audioInput.nearestFormat(m_format);
	}

	m_audioInput = new QAudioInput(m_opt.audioInput, m_format, q); 
	if (m_audioInput) {

		QObject::connect(m_audioInput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioInputStateChanged(QAudio::State)));

		m_audioInput->start(info);
	}
}

void AudioServicePrivate::stopRecord()
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	if (m_audioInput)
		m_audioInput->stop();

	do {
		if (m_audioInputFile && !m_audioInputFile->seek(0)) {
			setError(Service::AudioError, m_audioInputFile->errorString());
			break;
		}
	} while (false);
}

void AudioServicePrivate::audition()
{
	Q_Q(AudioService);

	Q_ASSERT(isActive());

	setError(Service::NoError);

	if (!m_audioInputFile) {
		qDebug() << DEBUG_PREFIX << "No audio has been recorded";
		return;
	}

	do {
		if (!m_audioInputFile->seek(0)) {
			setError(Service::AudioError, m_audioInputFile->errorString());
			break;
		}

		if (m_audioOutput) {
			m_audioOutput->stop();
			QObject::disconnect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioOutputStateChanged(QAudio::State)));
			delete m_audioOutput;
			m_audioOutput = 0;
		}
		m_audioOutput = new QAudioOutput(m_opt.audioOutput, m_format, q);
		if (m_audioOutput) {
			QObject::connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioOutputStateChanged(QAudio::State)));
		}

		m_audioOutput->start(m_audioInputFile);

	} while (false);
}

void AudioServicePrivate::stopAudition()
{
	Q_Q(AudioService);

	Q_ASSERT(isActive());

	setError(Service::NoError);

	stopPlay();
}

void AudioServicePrivate::_q_timeout()
{
	++m_duration;
	if (m_duration >=  EXCEED) {
		stopPlay();
	}
}

void AudioServicePrivate::startPlay(QIODevice* audio)
{
	Q_Q(AudioService);

	Q_ASSERT(isActive());
	Q_ASSERT(audio);

	setError(Service::NoError);

	do {
		if (!audio->seek(0)) {
			setError(Service::AudioError, audio->errorString());
			break;
		}

		if (m_audioOutput) {
			m_audioOutput->stop();
			QObject::disconnect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioOutputStateChanged(QAudio::State)));
			delete m_audioOutput;
			m_audioOutput = 0;
		}
		m_audioOutput = new QAudioOutput(m_opt.audioOutput, m_format, q);
		if (m_audioOutput) {
			QObject::connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), q, SIGNAL(audioOutputStateChanged(QAudio::State)));
		}

		m_audioOutput->start(audio);

		if (m_audioTimer)
			m_audioTimer->start();

		m_isRinging = true;

	} while (false);
}

void AudioServicePrivate::stopPlay()
{
	if (m_audioOutput) {
		m_audioOutput->stop();
	}

	if (m_audioTimer && m_audioTimer->isActive()) {
		m_audioTimer->stop();
		m_duration = 0;
	}

/*	m_isRinging = false;*/
}

AudioService::AudioService(ServiceManager* manager, QObject* parent) :
    AbstractService(*new AudioServicePrivate(manager, QString()), parent)
{
    Q_D(AudioService);
    d->init();
}

AudioService::AudioService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractService(*new AudioServicePrivate(manager, name), parent)
{
    Q_D(AudioService);
    d->init();
}

AudioService::AudioService(AudioServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
    Q_D(AudioService);
    d->init();
}

AudioService::~AudioService()
{
}

void AudioService::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(AudioService);

    typedef AudioServiceOptions Opt;
    Opt& opt = d->m_opt;
    Preference* pref = preference();

    if (force || paths.contains(Opt::audio_input_)) {
        opt.audioInput = QAudioDeviceInfo::defaultInputDevice();

        const QString deviceName = pref->getString(Opt::audio_input_);
        const QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
        foreach (const QAudioDeviceInfo& device, devices) {
            if (deviceName == device.deviceName()) {
                opt.audioInput = device;
            }
        }
    }

	if (force || paths.contains(Opt::audio_output_)) {
		opt.audioOutput = QAudioDeviceInfo::defaultOutputDevice();

		const QString deviceName = pref->getString(Opt::audio_output_);
		const QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
		foreach (const QAudioDeviceInfo& device, devices) {
			if (deviceName == device.deviceName()) {
				opt.audioOutput = device;
			}
		}
	}
}

void AudioService::savePreference()
{
    Q_D(AudioService);

    typedef AudioServiceOptions Opt;
    Preference* pref = preference();

    PreferenceTransaction trans(pref);

    {
        QString deviceName = d->m_opt.audioInput.deviceName();
		qDebug() << deviceName;
        QAudioDeviceInfo defaultAudioInput = QAudioDeviceInfo::defaultInputDevice();
        if (deviceName == defaultAudioInput.deviceName()) {
            deviceName = QString();
        }
        pref->putString(Opt::audio_input_, deviceName);
    }

	{
		QString deviceName = d->m_opt.audioOutput.deviceName();

		QAudioDeviceInfo defaultAudioOutput = QAudioDeviceInfo::defaultOutputDevice();
		if (deviceName == defaultAudioOutput.deviceName()) {
			deviceName = QString();
		}
		pref->putString(Opt::audio_output_, deviceName);
	}

    trans.commit();
}

UserService* AudioService::userService() const
{
    Q_D(const AudioService);
    return d->m_userService;
}

void AudioService::setUserService(UserService* userService)
{
    Q_D(AudioService);

    if (d->m_userService == userService)
        return;

    if (!isStopped()) {
        stop();
    }

    if (d->m_userService) {
        disconnect(d->m_userService, SIGNAL(active()), this, SLOT(start()));
        disconnect(d->m_userService, SIGNAL(stopped()), this, SLOT(stop()));
    }

    d->m_userService = userService;

    if (d->m_userService) {
        connect(d->m_userService, SIGNAL(active()), this, SLOT(start()));
        connect(d->m_userService, SIGNAL(stopped()), this, SLOT(stop()));
    }

    if (d->m_userService && d->m_userService->isActive()) {
        start();
    }
}

QAudioDeviceInfo AudioService::audioInput() const
{
    Q_D(const AudioService);
    return d->m_opt.audioInput;
}

void AudioService::setAudioInput(const QAudioDeviceInfo& audioInput)
{
    Q_D(AudioService);

//    if (d->m_opt.audioInput.deviceName() != audioInput.deviceName()) {
        d->m_opt.audioInput = audioInput;
        scheduleSavePreference();
  //  }
}

QAudioDeviceInfo AudioService::audioOutput() const
{
	Q_D(const AudioService);
	return d->m_opt.audioOutput;
}

void AudioService::setAudioOutput(const QAudioDeviceInfo& audioOutput)
{
	Q_D(AudioService);

//	if (d->m_opt.audioOutput.deviceName() != audioOutput.deviceName()) {
		d->m_opt.audioOutput = audioOutput;
		scheduleSavePreference();
//	}
}

void AudioService::setRingStatus(bool ring)
{
	Q_D(AudioService);

	d->m_isRinging = ring;
}

bool AudioService::isRinging()
{
	Q_D(AudioService);

	return d->m_isRinging;
}

void AudioService::startRecord()
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "startRecord rejected because of invalid state";
		return;
	}

	d->startRecord();
}

void AudioService::startRecord(QIODevice* info)
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "startRecord rejected because of invalid state";
		return;
	}

	d->startRecord(info);
}

void AudioService::stopRecord()
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "stopRecord rejected because of invalid state";
		return;
	}

	d->stopRecord();
}

void AudioService::audition()
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "audition rejected because of invalid state";
		return;
	}

	d->audition();
}

void AudioService::stopAudition()
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "stopAudition rejected because of invalid state";
		return;
	}

	d->stopAudition();
}

void AudioService::startPlay(QIODevice* audio)
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "startPlay rejected because of invalid state";
		return;
	}

	return d->startPlay(audio);
}

void AudioService::stopPlay()
{
	Q_D(AudioService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "stopPlay rejected because of invalid state";
		return;
	}

	return d->stopPlay();
}

} // namespace ns

#include "moc_audioservice.cpp"
