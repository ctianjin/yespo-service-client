#ifndef PAGECALLING_H
#define PAGECALLING_H

#include <QtCore/QScopedPointer>
#include <QPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QTimer;
class QMovie;
class QPixmap;
QT_END_NAMESPACE

namespace ns {

class UserService;
class SipService;
class CameraService;
class WidgetPhotoTranslation;
class YespoProto;
class Ui_PageCalling;
class RemoteVideoDialog;
class CallMode;

class PageCalling : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(CameraService* cameraService READ cameraService WRITE setCameraService)
	Q_PROPERTY(WidgetPhotoTranslation* widgetPhotoTranslation READ widgetPhotoTranslation WRITE setWidgetPhotoTranslation)
	Q_PROPERTY(YespoProto* proto READ proto WRITE setProto)

	static const QString styleVoiceFull;
	static const QString styleVoiceHalf;
	static const QString styleVoiceQuarter;
	static const QString styleVoiceEmpty;
public:
    PageCalling(QWidget* parent = 0);
    ~PageCalling();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	CameraService* cameraService() const;
	void setCameraService(CameraService* cameraService);

	WidgetPhotoTranslation* widgetPhotoTranslation() const;
	void setWidgetPhotoTranslation(WidgetPhotoTranslation* widgetPhotoTranslation);

	YespoProto* proto() const;
	void setProto(YespoProto* proto);

	CallMode* callMode() const;
	void setCallMode(CallMode* callMode);

	QWidget* widgetLocalVideo() const;

	void setCallId(int callId);
	void setLocalCountryLang(const QString& localCountry, const QString& localLanguage);
	void setLanguage(const QString& srcLang, const QString& targetLang);
	void setCallMode(const QString& mode);
	void setServiceMode(const QString& mode);
	void setRemoteContact(const QString& contact);
	void initCall(bool local);
	void uninitCall();
	void setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile);
	void setPhotoPicture(const QString& photoLink, const QString& photoFile);
	void cleanReceiveText();
	void setRemoteVideoWidget(RemoteVideoDialog* remote);
	bool videoEnabled();
	bool capture(const QString& fileName);
Q_SIGNALS:
	void showVideo();
	void captureFinished(const QString& fileName);

protected:
//	virtual void paintEvent(QPaintEvent *event);
	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent *event);

private	Q_SLOTS:
	void on_pushButtonHangup_clicked();

	void slotDuration();
	void slotCameraError();
	void slotCameraActive();
	void slotReceiveText(int callId, const QString& remote, const QString& text);
	void slotChangeImage();
	void slotReceiveVideoStatus(int status);
	void slotReceiveStatus(int callStatus);
//	void slotNextAnimation();

	void slotUserCancelled();
	void on_sliderVoice_sliderPressed();
	void on_sliderVoice_sliderReleased();
	void on_sliderVoice_sliderMoved(int value);
	void on_sliderVoice_valueChanged(int value);

private:
	enum CameraStatus {
		Normal = 0,
		Loaded,
		Active,
		Miss
	};
	CameraStatus m_cameraStatus;
	void retranslateTr();

	int m_callId;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_callMode;
	QString m_serviceMode;
	QString m_remoteContact;
	QString m_callerPhoto;
	QString m_calleePhoto;
	QString m_localCountry;
	QString m_localLanguage;
	QString m_receiveText;
	int m_videoStatus;
	int m_callStatus;
	bool m_videoEnabled;
	int m_duration;
	int m_movieNum;
	int m_state;

	QLabel* m_labelValueIcon;
	QLabel* m_topPanel;
	QTimer* m_durationTimer;
	QMovie* m_movie;
	QTimer* m_movieTimer;

	QString m_callerDisplayName;
	QString m_callerMobile;
	QString m_calleeDisplayName;
	QString m_calleeMobile;

	QPixmap m_pixmapCapture;

	QPointer<UserService> m_userService; 
	QPointer<SipService> m_sipService;
	QPointer<CameraService> m_cameraService;
	QPointer<WidgetPhotoTranslation> m_widgetPhotoTranslation;
	QPointer<YespoProto> m_proto;
	QPointer<RemoteVideoDialog> m_remoteVideoDialog;
	QPointer<CallMode> m_widgetCallMode;
	QScopedPointer<Ui_PageCalling> m_ui;
};

} // namespace ns

#endif // PAGECALLING_H
