#ifndef TESTVOICE_H
#define TESTVOICE_H

#include <QtCore/QScopedPointer>
#include <QPointer>
#include <QWidget>
#include <QAudio>
#include <QAudioFormat>

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

namespace ns {

class AudioInfo;
class AudioService;
class Ui_TestVoice;
//class RenderArea;

class TestVoice : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)

public:
	enum VoiceLevel {
		vlNormal = 0,
		vlNoSpeaker = 1,
		vlNoMicrophone = 2,
	};

    TestVoice(QWidget* parent = 0);
    ~TestVoice();

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	void startRecord();
	int level() const;

public Q_SLOTS: 
	void startTest();
	void finishTest();

Q_SIGNALS:
	void testFinished(int level);

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:
	void slotPlaybackChanged(QAudio::State state);
	void slotCaptureChanged(QAudio::State state);
	void on_pushButtonSpeakTest_clicked();
	void deviceChanged(int index);
	//void slotVoiceValue(qreal value);
	void slotVoiceValue();

private:
	int m_level;
	QFile* m_sampleAudio;
	AudioInfo *m_audioInfo;

	QPointer<AudioService> m_audioService;
	QScopedPointer<Ui_TestVoice> m_ui;
};

class AudioInfo : public QIODevice
{
	Q_OBJECT

public:
	AudioInfo(const QAudioFormat &format, QObject *parent);
	~AudioInfo();

	void start();
	void stop();

	qreal level() const { return m_level; }

	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len);

private:
	const QAudioFormat m_format;
	quint16 m_maxAmplitude;
	qreal m_level; // 0.0 <= m_level <= 1.0

Q_SIGNALS:
	void update();
};

} // namespace ns

#endif // TESTVOICE_H
