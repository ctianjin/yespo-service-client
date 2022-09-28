#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioFormat>
#include "abstractservice.h"
#include "serviceglobal.h"

QT_BEGIN_NAMESPACE
class QAudioDeviceInfo;
class QIODevice;
class QAudioFormat;
QT_END_NAMESPACE

namespace ns {

class UserService;
class AudioServicePrivate;

class BUILDSYS_SERVICE_DECL AudioService : public AbstractService
{
    Q_OBJECT
    Q_PROPERTY(UserService* userService READ userService WRITE setUserService)

public:
    explicit AudioService(ServiceManager* manager, QObject* parent = 0);
    AudioService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~AudioService();

    UserService* userService() const;
    void setUserService(UserService* userService);

    QAudioDeviceInfo audioInput() const;
    void setAudioInput(const QAudioDeviceInfo& audioInput);

	QAudioDeviceInfo audioOutput() const;
	void setAudioOutput(const QAudioDeviceInfo& audioOutput);

	void setRingStatus(bool ring);
	bool isRinging();

public Q_SLOTS:
	void startRecord();
	void startRecord(QIODevice* info);
	void stopRecord();
	void audition();
	void stopAudition();
	void startPlay(QIODevice* audio);
	void stopPlay();
private:
	Q_PRIVATE_SLOT(d_func(), void _q_timeout());

Q_SIGNALS:
	void audioOutputStateChanged(QAudio::State state);
	void audioInputStateChanged(QAudio::State state);

protected:
    AudioService(AudioServicePrivate& dd, QObject* parent = 0);

    virtual void applyPreference(const QSet<QString>& paths, bool force);
    virtual void savePreference();

private:

    Q_DECLARE_PRIVATE(AudioService)
    Q_DISABLE_COPY(AudioService)
};



} // namespace ns

#endif // AUDIOSERVICE_H
