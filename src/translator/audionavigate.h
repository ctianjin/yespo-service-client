#ifndef AUDIONAVIGATE_H
#define AUDIONAVIGATE_H
#ifdef Q_OS_WIN 
#include <Windows.h> 
#include <Psapi.h>
#endif
#include <QPointer>
#include <QScopedPointer>
#include "framelessdialog.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace ns {

class AudioService;
class CameraService;
class Ui_AudioNavigateDialog;

class AudioNavigateDialog : public FramelessDialog
{
    Q_OBJECT
    Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(CameraService* cameraService READ cameraService WRITE setCameraService)

public:
    explicit AudioNavigateDialog(QWidget* parent = 0);
    virtual ~AudioNavigateDialog();

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	CameraService* cameraService() const;
	void setCameraService(CameraService* cameraService);

	void setNetworkAddress(const QString& address);

	void getSystemVersion(QString& systemVersion);
	void getMemeryLoad(QString& memeryLoad);
	void getProcessorLoad(QString& processorLoad);

public Q_SLOTS:
	void initTest();
	void uninitTest();

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent* event);
	virtual void hideEvent(QHideEvent *event);

private Q_SLOTS:
	void on_pushButtonTestStart_clicked();
	void on_pushButtonStopTest_clicked();
	void on_pushButtonNext_clicked();
	void on_pushButtonTestFinish_clicked();
	void on_pushButtonReTest_clicked();

	void slotNetTestStarted();
	void slotNetTestResult(int level);
	void slotNetTestFinished(int level);
	void slotVoiceTestFinished(int level);
	void slotCameraTestFinished(int level);

private:
	void retranslateTr();
	void resetDisplayUi();
	void winProcessInit();

	QPointer<AudioService> m_audioService;
	QPointer<CameraService> m_cameraService;
    QScopedPointer<Ui_AudioNavigateDialog> m_ui;

	QString m_networkAddress;
	QString m_operatingSystem;
	QString m_memeryLoad;
	QString m_processorLoad;

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
	
    Q_DISABLE_COPY(AudioNavigateDialog)
};

} // namespace ns

#endif // AUDIONAVIGATE_H
