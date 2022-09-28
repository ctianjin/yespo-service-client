#ifndef VIEWINCOMINGCALL_H
#define VIEWINCOMINGCALL_H

#include <QtCore/QScopedPointer>
#include <QPointer>
#include <QWidget>
#include <QAudio>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
class QFile;
class QLabel;
class QMovie;
class QTimer;
QT_END_NAMESPACE

namespace ns {

class SipService;
class AudioService;
class CallMode;
class WidgetPhotoTranslation;
class YespoProto;
class Ui_ViewIncomingCall;

class ViewIncomingCall : public QWidget, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(WidgetPhotoTranslation* widgetPhotoTranslation READ widgetPhotoTranslation WRITE setWidgetPhotoTranslation)
	Q_PROPERTY(YespoProto* proto READ proto WRITE setProto)

public:
    ViewIncomingCall(QWidget* parent = 0);
    ~ViewIncomingCall();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	WidgetPhotoTranslation* widgetPhotoTranslation() const;
	void setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation);

	YespoProto* proto() const;
	void setProto(YespoProto* proto);

	CallMode* callMode() const;
	void setCallMode(CallMode* callMode);

// 	CameraService* cameraService() const;
// 	void setCameraService(CameraService* cameraService);

	void setCallId(int callId);
	int callId();
	void setCallState(const QString& state);
	void setLocalCountryLang(const QString& localCountry, const QString& localLanguage);
	void setLanguage(const QString& srcLang, const QString& targetLang);
	void setCallMode(const QString& mode);
	void setServiceMode(const QString& mode);
	void setRemoteContact(const QString& contact);
	void setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile);
	void setPhotoPicture(const QString& photoLink, const QString& photoFile);

	void callIncoming(int callId);
	void callAnswered();
	void callDisconnected();
	void startRinging();
	int callId() const;

	virtual void setPreference(Preference* pref);

	void startTimer();
	void stopTimer();

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

private	Q_SLOTS:
	void on_pushButtonAnswer_clicked();
	void on_pushButtonReject_clicked();

	void slotRingingChanged(QAudio::State state);
// 	void slotCameraError();
// 	void slotCameraActive();

    void slotNextAnimationFrame();
	void slotChangeImage();
private:
	void retranslateTr();

	int m_accountId;
	int m_callId;
	QString m_callState;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_callMode;
	QString m_serviceMode;
	QString m_remoteContact;
	QString m_callerPhoto;
	QString m_calleePhoto;
	bool m_Local;
	QString m_localCountry;
	QString m_localLanguage;
	int m_movieNum;

	QFile* m_ringingTone;

// 	QLabel* m_topPanel;
// 	QLabel* m_topPanelLeft;
// 	QLabel* m_topPanelRight;

//	QMovie* m_movie;

	QTimer* m_timer;
	QTimer* m_movieTimer;

	QString m_callerDisplayName;
	QString m_callerMobile;
	QString m_calleeDisplayName;
	QString m_calleeMobile;

	QPointer<SipService> m_sipService;
	QPointer<AudioService> m_audioService;
	QPointer<WidgetPhotoTranslation> m_widgetPhotoTranslation;
	QPointer<YespoProto> m_proto;
	QPointer<CallMode> m_widgetCallMode;
	QScopedPointer<Ui_ViewIncomingCall> m_ui;
};

} // namespace ns

#endif // VIEWINCOMINGCALL_H
