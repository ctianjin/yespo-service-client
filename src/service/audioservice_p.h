#ifndef QPID_AUDIOSERVICE_P_H
#define QPID_AUDIOSERVICE_P_H

#include <QtCore/QPointer>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include "audioservice.h"
#include "abstractservice_p.h"

QT_BEGIN_NAMESPACE
class QTemporaryFile;
class QAudioInput;
class QAudioOutput;
class QIODevice;
class QTimer;
QT_END_NAMESPACE

namespace ns {

struct AudioServiceOptions
{
    static const QString audio_input_;
	static const QString audio_output_;

    QAudioDeviceInfo audioInput;
	QAudioDeviceInfo audioOutput;

    AudioServiceOptions();
};

class AudioServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(AudioService)

public:
    static const QString tempFileTemplate;

    AudioServiceOptions m_opt;
    QPointer<UserService> m_userService;
	QAudioFormat m_format;
	QAudioOutput* m_audioOutput;
    QAudioInput* m_audioInput;
	QTemporaryFile* m_audioFile;
    QTemporaryFile* m_audioInputFile;
	QTimer* m_audioTimer;
	bool m_isRinging;
	int m_duration;

    AudioServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~AudioServicePrivate();
	
	void startPlay(QIODevice* audio);
	void stopPlay();
	void startRecord();
	void startRecord(QIODevice* info);
	void stopRecord();
	void audition();
	void stopAudition();

	void _q_timeout();

protected:
    virtual void cleanup();

private:
    void init();

};

} // namespace ns

#endif // QPID_AUDIOSERVICE_P_H
