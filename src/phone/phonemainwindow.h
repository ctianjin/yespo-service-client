#ifndef QPID_PHONEMAINWINDOW_H
#define QPID_PHONEMAINWINDOW_H

#include <QHash>
#include <QScopedPointer>
#include <QSystemTrayIcon>
#include <QtMultimedia/QAudio>
#include "framelessmainwindow.h"
#include "preferencesupport.h"
#include "command.h"
#include "contactsitems_p.h"
#include "recentlistitems_p.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QTimer;
class QMovie;
class QAction;
class QActionGroup;
QT_END_NAMESPACE

namespace Qpid {

class UserService;
class SipService;
class AudioService;
class CommandService;
class MediaService;
class Ui_PhoneMainWindow;
struct PhoneMainWindowOptions;
class LoginDialog;
class CallSplash;
class AudioNavigateDialog;
class MessagePanel;

class PhoneMainWindow : public MainWindow, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)

public:
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

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

    virtual void setPreference(Preference* pref);

	void setLoginDialog(LoginDialog *l);

	QString userName() const;
	void setUserName(const QString& userName);

	QString password() const;
	void setPassword(const QString& password);

	void serverSetting();

	void phoneLoadConfig();
	
public Q_SLOTS:
    void showMainWindow();
	void slotUnLogin();

protected:
	virtual bool event(QEvent* event);
    virtual void changeEvent(QEvent* event);
    virtual void closeEvent(QCloseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void applyPreference(const QSet<QString>& paths, bool force);

private Q_SLOTS:
	void slotUserServiceStopped();
    void slotUserServiceActive();
    void slotUserServiceInactive();	
	void slotLoginFailed(const QString& error, const QString& errorMsg, bool command);

	void slotSipLog(const QString& msg);
	void slotRegState(int account, int code, const QString& reason);
	void slotSipIncommingCall(int account, int callId, const QString& srcLang, const QString& dstLang, 
		const QString& mode, const QString& serviceMode, const QString& remote);
	void slotSipCallState(int callId, int state);
	void slotReceiveText(int callId, const QString& remote, const QString& text);
	void slotReceiveDtmf(int callId, int dtmf);
	void slotBuddyStateChanged(int buddyID, int state);
	void slotAudioOutputStateChanged(QAudio::State state);
#ifdef QPID_DTMF_ENABLE
	void slotDtmfOutputStateChanged(QAudio::State state);
#endif
    void slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void slotMinimizeTray();
	void slotQuitApplication();

	void slotPhoneLoadconfigFinished(const Command::PhoneLoadConfigParam& param, const Command::PhoneLoadConfigResult& result);
	void slotPhoneRegisterFinished(const Command::PhoneRegisterParam& param, const Command::PhoneRegisterResult& result);
	void slotPhoneForgetPasswordFinished(const Command::PhoneForgetPasswordParam& param, const Command::PhoneForgetPasswordResult& result);
	void slotPhoneContactListFinished(const Command::PhoneContactListParam& param, const Command::PhoneContactListResult& result);
	void slotPhoneAddContactFinished(const Command::PhoneAddContactParam& param, const Command::PhoneAddContactResult& result);
	void slotPhoneUpdateContactFinished(const Command::PhoneUpdateContactParam& param, const Command::PhoneUpdateContactResult& result);
	void slotPhoneDeleteContactFinished(const Command::PhoneDeleteContactParam& param, const Command::PhoneDeleteContactResult& result);
	void slotPhoneDetailContactFinished(const Command::PhoneDetailContactParam& param, const Command::PhoneDetailContactResult& result);
	void slotPhoneGetHistoryCDRFinished(const Command::PhoneGetHistoryCDRParam& param, const Command::PhoneGetHistoryCDRResult& result);
	void slotPhoneGetUserInfoFinished(const Command::PhoneGetUserInfoParam& param, const Command::PhoneGetUserInfoResult& result);
	void slotPhoneEditUserInfoFinished(const Command::PhoneEditUserInfoParam& param, const Command::PhoneEditUserInfoResult& result);
	void slotPhoneChangePasswordFinished(const Command::PhoneChangePasswordParam& param, const Command::PhoneChangePasswordResult& result);
	void slotPhoneNewPasswordFinished(const Command::PhoneNewPasswordParam& param, const Command::PhoneNewPasswordResult& result);
	void slotPhoneGetCDRIDFinished(const Command::PhoneGetCDRIDParam& param, const Command::PhoneGetCDRIDResult& result);
	void slotPhoneDeleteCDRFinished(const Command::PhoneDeleteCDRParam& param, const Command::PhoneDeleteCDRResult& result);
	void slotPhotoDownloaded(const QString& addBookID, const QString& fileName);
	void slotUserPhotoDownloadFailed(const QString& link, const QString& reason);
	void slotUserPhotoDownloaded(const QString& photoLink, const QString& fileName);
	void slotDelayCDR();
	void slotCallingMinimize(bool minimize);
	void slotCallingMaximize();
	void slotSplashAnswer();
	void slotSplashDecline();
	void slotLanguageChanged(QAction* language);
	void slotViewProfile(QTreeWidgetItem * item);
	void slotRemoveContact(QTreeWidgetItem * item);
	void slotRemoveHistory(QTreeWidgetItem * item);

	void on_actionSign_Out_triggered();
	void on_actionClose_triggered();
	void on_actionAll_triggered();
	void on_actionVoice_Easy_Users_triggered();
	void on_actionPhone_Numbers_triggered();
	void on_actionRecentAll_triggered();
	void on_actionRecentMissed_triggered();
	void on_actionUpdate_triggered();
	void on_actionAbout_Voice_Easy_triggered();
	void on_actionQuit_triggered();
	void on_pushButtonSignin_clicked();
	void on_pushButtonCreateAccount_clicked();
	void on_pushButtonForgetPassword_clicked();
	void on_pushButtonAddCredit_clicked();
	void on_pushButtonContacts_clicked();
	void on_pushButtonRecent_clicked();
	void on_treeWidgetContacts_itemClicked(QTreeWidgetItem * item, int column);
	void on_treeWidgetRecent_itemClicked(QTreeWidgetItem * item, int column);
	void on_lineEditSearch_textChanged(const QString& text);

private:
	void retranslateTr();
	void finishLogin(bool succeeded);
	void stopPhoneView();
	void doContactFilter(const QString& text);
	void doPhoneHistoryFilter(const QString& text);
	void updateContactTypeAction();
	void updateRecentTypeAction();
	void disconnectPhotoSignal();

    QScopedPointer<Ui_PhoneMainWindow> m_ui;
    QScopedPointer<PhoneMainWindowOptions> m_opt;
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayIconMenu;
	QMenu* m_accountMenu;
	QMenu* m_statusMenu;
	LoginDialog* m_l;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<AudioService> m_audioService;
#ifdef QPID_DTMF_ENABLE
	QPointer<AudioService> m_audioDtmfService;
#endif
	QPointer<AudioService> m_audioNavService;
	QPointer<CommandService> m_commandService;
	QPointer<MediaService> m_mediaService;
	CallSplash* m_callSplash;
	QHash<QString, ContactItem*> m_contactItems;
	QHash<QString, PhoneHistoryItem*> m_phoneHistoryItems;
	AudioNavigateDialog* m_audioNavDlg;
	QMovie* m_movie;
	MessagePanel* m_messagePanel;
	int m_sipAccountID;

	QMenu* m_contactsFilterMenu;
	enum ContactType
	{
		ContactTypeAll = 0,
		ContactTypeVoiceEasy = 1,
		ContactTypeNumber = 2
	};
	ContactType m_contactType;
	QMenu* m_recentFilterMenu;
	enum RecentType
	{
		RecentTypeAll = 0,
		RecentTypeMissed = 1
	};
	RecentType m_recentType;

	QActionGroup *m_languageActions;
	QString m_createAccountUrl;
	QString m_changePasswordUrl;
	QString m_editUserInfoUrl;
	QString m_latestVersion;
	QString m_latestUrl;
	QString m_latestForce;

    Q_DISABLE_COPY(PhoneMainWindow)
};

} // namespace Qpid

#endif // QPID_PHONEMAINWINDOW_H
