#ifndef QPID_AUDIONAVIGATE_H
#define QPID_AUDIONAVIGATE_H
#include <QPointer>
#include <QScopedPointer>
#include <QtMultimedia/QAudio>
#include "framelessdialog.h"

QT_BEGIN_NAMESPACE
class QFile;
class QMovie;
QT_END_NAMESPACE

namespace Qpid {

class AudioService;
class Ui_AudioNavigateDialog;
struct AudioNavigateDialogOptions;

class AudioNavigateDialog : public FramelessDialog
{
    Q_OBJECT
    Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)

public:
	enum RecordStep {
		RecordNotStart = 0,
		RecordStart = 1,
		RecordStop = 2,
		AuditionStart = 3
	};

    explicit AudioNavigateDialog(QWidget* parent = 0);
    virtual ~AudioNavigateDialog();

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

public Q_SLOTS:

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent* event);
	virtual void showEvent(QShowEvent* event);

private Q_SLOTS:
	void slotPlaybackChanged(QAudio::State state);
	void slotCaptureChanged(QAudio::State state);

	void on_pushButtonPlayback_clicked();
	void on_pushButtonPlaybackFix_clicked();
	void on_pushButtonPlaybackNext_clicked();
	void on_pushButtonPlaybackSkip_clicked();
	
	void on_pushButtonCapture_pressed();
	void on_pushButtonCapture_released();
	void on_pushButtonCaptureFix_clicked();
	void on_pushButtonCaptureFinish_clicked();

private:
	QFile* m_sampleAudio;
	QMovie* m_movie;
	RecordStep m_recordStep;

    QScopedPointer<Ui_AudioNavigateDialog> m_ui;
    QScopedPointer<AudioNavigateDialogOptions> m_opt;
	QPointer<AudioService> m_audioService;

    Q_DISABLE_COPY(AudioNavigateDialog)
};

} // namespace Qpid

#endif // QPID_AUDIONAVIGATE_H
