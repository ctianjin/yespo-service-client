#ifndef QPID_CALLEEMAINWINDOW_H
#define QPID_CALLEEMAINWINDOW_H

#include <QScopedPointer>
#include <QSystemTrayIcon>
#include <QtMultimedia/QAudio>
#include "framelessmainwindow.h"
#include "preferencesupport.h"
#include "command.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QTimer;
QT_END_NAMESPACE

namespace Qpid {

class UserService;
class SipService;
class AudioService;
class CommandService;
class Ui_CalleeMainWindow;
struct CalleeMainWindowOptions;
class CalleeInfo;

class CalleeMainWindow : public MainWindow, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(AudioService* audioService READ audioService WRITE setAudioService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    explicit CalleeMainWindow(QWidget* parent = 0);
    virtual ~CalleeMainWindow();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	AudioService* audioService() const;
	void setAudioService(AudioService* audioService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

    virtual void setPreference(Preference* pref);

/*	void setLoginDialog(LoginDialog *l);*/
	
public Q_SLOTS:
    void showMainWindow();
/*	void slotUnLogin();*/

protected:
	virtual bool event(QEvent* event);
    virtual void changeEvent(QEvent* event);
    virtual void closeEvent(QCloseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
//    virtual void keyPressEvent(QKeyEvent *event);
    virtual void applyPreference(const QSet<QString>& paths, bool force);

private Q_SLOTS:
    void slotUserServiceActive();
    void slotUserServiceInactive();
	void slotSipLog(const QString& msg);
	void slotRegState(int code, const QString& reason);
	void slotSipIncommingCall(int callId, const QString& srcLang, const QString& dstLang, const QString& mode, const QString& remote);
	void slotSipCallState(int state);
	void slotReceiveText(int callId, const QString& remote, const QString& text);
	void slotAudioOutputStateChanged(QAudio::State state);
	void slotDtmfOutputStateChanged(QAudio::State state);
    void slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void slotMinimizeTray();
	void slotQuitApplication();
	void slotLoadconfigFinished(const Command::LoadConfigParam& param, const Command::LoadConfigResult& result);

	void on_actionAbout_triggered();
	void on_actionUpdate_triggered();
	void on_actionQuit_triggered();

private:
	void stopPhoneView();
	void updateCallDisplay();
    QScopedPointer<Ui_CalleeMainWindow> m_ui;
    QScopedPointer<CalleeMainWindowOptions> m_opt;
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayIconMenu;
	QMenu* m_accountMenu;
	QMenu* m_statusMenu;
/*	LoginDialog* m_l;*/
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<AudioService> m_audioService;
	QPointer<AudioService> m_audioDtmfService;
	QPointer<AudioService> m_audioNavService;
	QPointer<CommandService> m_commandService;
	CalleeInfo* m_callInfo;
// 	CallSplash* m_callSplash;
// 	QTimer* m_clockTimer;
// 	QTimer* m_durationTimer;

    Q_DISABLE_COPY(CalleeMainWindow)
};

} // namespace Qpid

#endif // QPID_CALLEEMAINWINDOW_H
