#ifndef PHONEMAINWINDOW_H
#define PHONEMAINWINDOW_H

#include <QHash>
#include <QtNetwork/QNetworkReply>
#include <QScopedPointer>
#include <QAbstractAnimation>
#include <QSystemTrayIcon>
#include <QtMultimedia/QAudio>
#include "framelessmainwindow.h"
#include "preferencesupport.h"
#include "command.h"
#include "udp.h"
#include "stun.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QHostInfo;
class QPropertyAnimation;
QT_END_NAMESPACE

namespace ns {

class UserService;
class YespoProto;
class SipService;
class AudioService;
class CommandService;
class PhotoService;
class CameraService;
class Ui_PhoneMainWindow;
struct PhoneMainWindowOptions;
class CallInfo;
class SipPreferenceDialog;
class AudioNavigateDialog;
class SettingsDialog;
class UpdateDialog;
class RemoteVideoDialog; 
class CloseMethodDialog;
class WidgetPhotoTranslation;
class SipNotify;
class ReportNotify;
class CallMode;
class Browser;

class PhoneMainWindow : public FramelessMainWindow, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(PhotoService* photoService READ photoService WRITE setPhotoService)
public:
	static const QString token;
	static const QString deviceId;
	static const QString language;
	static const QString callRecordId;
	static const QString source;
	static const QString stunServer1;
	static const QString stunServer2;
	static const QString stunServer3;

    explicit PhoneMainWindow(QWidget* parent = 0);
    virtual ~PhoneMainWindow();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	PhotoService* photoService() const;
	void setPhotoService(PhotoService* photoService);

	CameraService* cameraService() const;
	void setCameraService(CameraService* cameraService);

	WidgetPhotoTranslation* widgetPhotoTranslation() const;

    virtual void setPreference(Preference* pref);

	void loadConfig();
	void autoLogin(const QString& userName, const QString& password);

	void notifySipStatus(const QString& sipServiceHost);

	QString getNetworkAddress() const;

	bool parseServer(QStringList serverList, StunAddress4& address, QString& server); 
	void getNetworkAddressFormStun(QString& networkAddr);
	
public Q_SLOTS:
    void showMainWindow();
	void showMinimized();
	void logout();
	void showVideo();
	void slotAssignIP(QNetworkReply* reply);
	void slotRequestReportList();
	void slotRequestReport(const QString& cdrId);
	void slotRequestRefreshBrowser();
protected:
    virtual void changeEvent(QEvent* event);
    virtual void closeEvent(QCloseEvent* event);
	virtual void showEvent(QShowEvent *event);
    virtual void applyPreference(const QSet<QString>& paths, bool force);

private Q_SLOTS:
    void slotUserServiceActive();
    void slotUserServiceInactive();
#ifdef QPID_DTMF_ENABLE
	void slotDtmfOutputStateChanged(QAudio::State state);
#endif
    void slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void slotMinimizeTray();
	void slotQuitApplication();
	void slotServiceError(ns::Service::Error error, const QString& token, const QString& reason);
	void slotLoadconfigFinished(const Command::LoadConfigParam& param, const Command::LoadConfigResult& result);
	void slotSetClientInfoFinished(const Command::SetClientInfoParam& param, const Command::SetClientInfoResult& result);

	void on_actionAbout_triggered();
	void on_actionUpdate_triggered();
	void on_actionQuit_triggered();
	void on_actionSettings_triggered();
	void on_actionDevice_triggered();
	void on_actionLogout_triggered();
	void on_actionHome_triggered();
	void on_actionReport_triggered();
	void on_actionHistory_triggered();
	void on_actionProfile_triggered();
	void on_actionPrivacy_triggered();
	void on_actionPassword_triggered();	

	void on_pushButtonSipSetting_clicked();

	/* New Prototype */
	void slotLoginFinished(bool succeed);
	void slotMainCallIn();
	void slotMainCallConfirmed(bool callLocal);
	void slotMainCallClosed();
	void slotMainCallSucceed(const QString& cdrId);
	void slotAlwaysOnTop(bool on);
	void slotShowTrayIcon(bool show);
	void slotTransportChanged(int transport);

	void slotAnimationStateChanged(QAbstractAnimation::State newState,
		QAbstractAnimation::State oldState);

	void slotSocketError(const QString& errorMsg);
	void slotConnected();
	void slotDisconnected();
	void slotLoginFinished(int rspCode);
	void slotLogoutFinished();
	void slotReLogin();

private:
	static void addCommonRequestHeaders(QNetworkRequest& request); 
	void initVideoWidget();
	void stopPhoneView();
	void importLanguages();
	void retranslateTr();

    QScopedPointer<Ui_PhoneMainWindow> m_ui;
    QScopedPointer<PhoneMainWindowOptions> m_opt;
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayIconMenu;
	QMenu* m_accountMenu;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<AudioService> m_audioService;
#ifdef QPID_DTMF_ENABLE
	QPointer<AudioService> m_audioDtmfService;
#endif
	QPointer<AudioService> m_audioNavService;
	QPointer<CommandService> m_commandService;
	QPointer<PhotoService> m_photoService;
	QPointer<CameraService> m_cameraService;
	QPointer<YespoProto> m_proto;
	QHash<QString, QString> m_languages;

	QScopedPointer<AudioNavigateDialog> m_audioNavDlg;
	QScopedPointer<SettingsDialog> m_settings;
	QScopedPointer<UpdateDialog> m_updateDialog;
	QScopedPointer<SipPreferenceDialog> m_sipPreferenceDialog;
	QPointer<WidgetPhotoTranslation> m_widgetPhotoTranslation;
	QPointer<SipNotify> m_sipNotify;
	QPointer<ReportNotify> m_reportNotify;
	QPointer<CallMode> m_callMode;
	QPointer<RemoteVideoDialog> m_remoteVideoDialog;
	QPointer<CloseMethodDialog> m_closeMethodDialog;
	QPointer<Browser> m_browser;
	QPointer<Browser> m_browserReportList;
// 	QPointer<Browser> m_browserReport;
// 	QPointer<Browser> m_browserNotice;
// 	QPointer<Browser> m_browserRewrite;

	QString m_homeLink;
	QString m_reportLink;
	QString m_noReportListLink;
	QString m_addReportLink;
	QString m_cdrLink;
	QString m_profileLink;
	QString m_privacyLink;
	QString m_passwordLink;
	QString m_forgetPasswordLink;
	QString m_networkAddress;

	QString m_clientVersion;
	QString m_clientLink;
	QString m_clientForce;

	QString m_notice;
	QString m_domainStatus;
	QString m_domainCall;
	QString m_domainIm;

	QString m_callReportHtml;
	QString m_callReportNoticeHtml;
	QString m_callReportRewrite;

	int m_countReload;
	QPixmap m_pixMask;
	QPropertyAnimation* m_animation;

    Q_DISABLE_COPY(PhoneMainWindow)
};

} // namespace ns

#endif // PHONEMAINWINDOW_H
