#include <QKeyEvent>
#include <QDesktopWidget>
#include <QDebug>
#include <QUrl>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QCloseEvent>
#include <QWindowStateChangeEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#ifndef QPID_PHONE_ENABLE_TRANSLUCENT
#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#endif // QPID_PHONE_ENABLE_TRANSLUCENT
#include "translatormainwindow.h"
#include "capabilityapplication.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "ui_translatormainwindow.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "TranslatorMainWindow:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

class TranslatorInfo {

public:
	TranslatorInfo() : m_callID(-1)
		,m_callState(CallClosed)
		,m_duration(0)
		,m_callsTaken(0)
		,m_callsSuccessful(0)
		,m_callsMissed(0)
		,m_srcLang()
		,m_dstLang()
		,m_mode(ThreeWay)
		,m_remote()
	{

	}
	~TranslatorInfo(){}

	enum CallState {
		CallClosed = 0,
		CallIncomming = 1,
		CallAnswered = 2,
		CallConnectingCallee = 3,
		CallCalleeAnswered=4
	};

	enum CallMode {
		ThreeWay = 0,
		TwoWay = 1
	};

	int callID() const { return m_callID; }
	void setCallID(int callID) { m_callID = callID; }
	CallState state() const { return m_callState; }
	void setState(CallState callState) { m_callState = callState; }
	int callDuration() const { return m_duration; }
	void setCallDruation(int duration) { m_duration = duration; }
	int callsTaken() const { return m_callsTaken; }
	void increaseCallsTaken() { ++m_callsTaken; }
	int callsSuccessful() const { return m_callsSuccessful; }
	void increaseCallsSuccessful() { ++m_callsSuccessful; }
	int callsMissed() const { return m_callsMissed; }
	void increaseCallsMissed() { ++m_callsMissed; }
	QString srcLang() const { return m_srcLang; }
	QString dstLang() const { return m_dstLang; }
	CallMode mode() const { return m_mode; }
	QString remote() const { return m_remote; }

	void callIncomming(int callId, const QString& srcLang, const QString& dstLang, CallMode mode, const QString& remote) {
		m_callID = callId;
		m_callState = CallIncomming;
		m_srcLang = srcLang;
		m_dstLang = dstLang;
		m_mode = mode;
		m_remote = remote;
	}

	void callAnswered() {
		m_duration = 0;
		m_callState = CallAnswered;
	}

	void callClosed() {
		resetCall();
	}

	void resetAll() {
		m_callID = -1;
		m_duration = 0;
		m_callState = CallClosed;
		m_callsTaken = 0;
		m_callsSuccessful = 0;
		m_callsMissed = 0;
	}

private:
	void resetCall() {
		m_callID = -1;
		m_duration = 0;
		m_callState = CallClosed;
		m_srcLang = QString();
		m_dstLang = QString();
		m_mode = ThreeWay;
		m_remote = QString();
	}

private:
	int m_callID;
	CallState m_callState;
	int m_duration;
	int m_callsTaken;
	int m_callsSuccessful;
	int m_callsMissed;
	QString m_srcLang;
	QString m_dstLang;
	CallMode m_mode;
	QString m_remote;
};

struct TranslatorMainWindowOptions
{
    static const QString view_;
    static const QString media_browser_;
	static const QString course_browser_;
    static const QString phone_;
    static const QString exam_;
    static const QString word_book_;
    static const QString application_;

	static const QString menu_style_;
};

const QString TranslatorMainWindowOptions::view_ = QLatin1String("view");
const QString TranslatorMainWindowOptions::media_browser_ = QLatin1String("media_browser");
const QString TranslatorMainWindowOptions::course_browser_ = QLatin1String("course_browser");
const QString TranslatorMainWindowOptions::phone_ = QLatin1String("phone");
const QString TranslatorMainWindowOptions::exam_ = QLatin1String("exam");
const QString TranslatorMainWindowOptions::word_book_ = QLatin1String("word_book");
const QString TranslatorMainWindowOptions::application_ = QLatin1String("application");
const QString TranslatorMainWindowOptions::menu_style_ = QLatin1String(
"QMenu {"
"	background-color: rgb(39,154,216);"
"	margin: 0px;"
"	color: white;"
"}"
"QMenu::item {"
"	padding: 2px 25px 2px 20px;"
"	border: 1px solid rgb(0,102,204);"
"}"
"QMenu::item:selected {"
"	border-color: darkblue;"
"	background: rgba(100, 100, 100, 150);"
"}"
"QMenu::icon:checked {"
"	background: gray;"
"	border: 1px inset gray;"
"	position: absolute;"
"	top: 1px;"
"	right: 1px;"
"	bottom: 1px;"
"	left: 1px;"
"}"
"QMenu::separator {"
"	height: 2px;"
"	background: lightblue;"
"	margin-left: 10px;"
"	margin-right: 5px;"
"}"
"QMenu::indicator {"
"	width: 13px;"
"	height: 13px;"
"}"
);

TranslatorMainWindow::TranslatorMainWindow(QWidget* parent) :
    MainWindow(parent),
    m_ui(new Ui_TranslatorMainWindow),
    m_opt(new TranslatorMainWindowOptions),
    m_trayIcon(0),
    m_trayIconMenu(0),
	m_accountMenu(0),
	m_statusMenu(0),
/*	m_l(0),*/
	m_callInfo(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

    // Tray icon
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(qApp->windowIcon());
    m_trayIconMenu = new QMenu(this);
	m_trayIconMenu->setStyleSheet(TranslatorMainWindowOptions::menu_style_);
    m_trayIconMenu->addAction(m_ui->actionAbout);
    m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(m_ui->actionUpdate);
	m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_ui->actionQuit);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip(windowTitle());

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(slotTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    readSettings();

	connect(qApp, SIGNAL(sessionTimeOut()), this, SLOT(slotUnLogin()));

	m_ui->pushButtonHangup->hide();

	m_audioNavService = qApp->createAudioService(0);
	m_audioNavService->start();
	m_audioDtmfService = qApp->createAudioService(0);
	connect(m_audioDtmfService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotDtmfOutputStateChanged(QAudio::State)));
	m_audioDtmfService->start();

	m_callInfo = new TranslatorInfo;

}

TranslatorMainWindow::~TranslatorMainWindow()
{
	if (m_audioNavService) {
		m_audioNavService->stop();
	}

	if (m_audioDtmfService) {
		m_audioDtmfService->stopPlay();
		m_audioDtmfService->stop();
	}

	if (m_callInfo) {
		m_callInfo->resetAll();
		delete m_callInfo;
	}
}

UserService* TranslatorMainWindow::userService() const
{
	return m_userService;
}

void TranslatorMainWindow::setUserService(UserService* userService)
{
	if (m_userService) {
		disconnect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
		disconnect(m_userService, SIGNAL(inactive()), this, SLOT(slotUserServiceInactive()));
	}

	m_userService = userService;

	if (m_userService) {
		connect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
		connect(m_userService, SIGNAL(inactive()), this, SLOT(slotUserServiceInactive()));
	}
}

SipService* TranslatorMainWindow::sipService() const
{
	return m_sipService;
}

void TranslatorMainWindow::setSipService(SipService* sipService)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(sipLog(const QString&)), this, SLOT(slotSipLog(const QString&)));
		disconnect(m_sipService, SIGNAL(regState(int, const QString&)), this, SLOT(slotRegState(int, const QString&)));
		disconnect(m_sipService, SIGNAL(incommingCall(int, const QString&, const QString&, const QString&, const QString&)), 
			this, SLOT(slotSipIncommingCall(int, const QString&, const QString&, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)),
			this, SLOT(slotReceiveText(int, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(callState(int)), this, SLOT(slotSipCallState(int)));
	}

	m_sipService = sipService;

	if (m_sipService) {
		connect(m_sipService, SIGNAL(sipLog(const QString&)), this, SLOT(slotSipLog(const QString&)));
		connect(m_sipService, SIGNAL(regState(int, const QString&)), this, SLOT(slotRegState(int, const QString&)));
		connect(m_sipService, SIGNAL(incommingCall(int, const QString&, const QString&, const QString&, const QString&)), 
			this, SLOT(slotSipIncommingCall(int, const QString&, const QString&, const QString&, const QString&)));
		connect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)),
			this, SLOT(slotReceiveText(int, const QString&, const QString&)));
		connect(m_sipService, SIGNAL(callState(int)), this, SLOT(slotSipCallState(int)));
	}
}

AudioService* TranslatorMainWindow::audioService() const
{
	return m_audioService;
}

void TranslatorMainWindow::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotAudioOutputStateChanged(QAudio::State)));
	}

	m_audioService = audioService;

	if (m_audioService) {
		connect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotAudioOutputStateChanged(QAudio::State)));
	}
}

CommandService* TranslatorMainWindow::commandService() const
{
	return m_commandService;
}

void TranslatorMainWindow::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(loadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)), 
			this, SLOT(slotLoadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)));
	}

	m_commandService = commandService;

	if (m_commandService) {
		connect(m_commandService, SIGNAL(loadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)), 
			this, SLOT(slotLoadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)));
	}
}

void TranslatorMainWindow::setPreference(Preference* pref)
{
    PreferenceSupport::setPreference(pref);
    typedef TranslatorMainWindowOptions Opt;

}

// void TranslatorMainWindow::setLoginDialog(LoginDialog *l)
// {
// 	m_l = l;
// }

void TranslatorMainWindow::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef TranslatorMainWindowOptions Opt;
    Preference* pref = preference();
}

bool TranslatorMainWindow::event(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
//        adjustSystemButtons();
    }

    return MainWindow::event(event);
}

void TranslatorMainWindow::changeEvent(QEvent* event)
{
    MainWindow::changeEvent(event);

	if (event->type() == QEvent::WindowStateChange) {
		QWindowStateChangeEvent* e = static_cast< QWindowStateChangeEvent* >(event);
		if (e->oldState() == Qt::WindowMinimized) {
			this->hide();
			this->show();
		}
	}

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void TranslatorMainWindow::closeEvent(QCloseEvent* event)
{
	typedef TranslatorMainWindowOptions Opt;

//     if (!qApp->confirm(Opt::application_, tr("Are you sure to exit the application?"), this)) {
//         event->ignore();
//         return;
//     }
// 
// 	if (qApp->isHideWindow(Opt::application_)) {
// 		event->ignore(); 
// 		hide();
// 		return;
// 	}
	
    MainWindow::closeEvent(event);

	slotQuitApplication();
}

void TranslatorMainWindow::resizeEvent(QResizeEvent* event)
{
    MainWindow::resizeEvent(event);

#ifndef QPID_PHONE_ENABLE_TRANSLUCENT
    const QSize size = event->size();
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(0, 0, size.width() - 1, size.height() - 1, 2, 2);
    setMask(pixmap.createMaskFromColor(Qt::white));
#endif // QPID_PHONE_ENABLE_TRANSLUCENT
}

void TranslatorMainWindow::showMainWindow()
{
#ifdef QPID_ENABLE_SINGLE_APPLICATION
    qApp->setActivationWindow(this);
#endif // QPID_ENABLE_SINGLE_APPLICATION

	if (m_commandService) {
		QDateTime t = QDateTime::currentDateTime();
		Command::LoadConfigParam param;
		param.setLastTime(t.toString(QLatin1String("yyyy-MM-dd")));
		param.setReloadConfig(1);
		m_commandService->loadconfig(param);
	}

	

    show();
	qApp->setQuitOnLastWindowClosed(false);
    m_trayIcon->show();

	/* Register to the sip server*/
	if (m_sipService && m_userService) {
		QString sipServer = m_userService->userServiceHost();
		int splitter = sipServer.indexOf(QLatin1Char(':'));
		if (splitter != -1) 
			sipServer = sipServer.left(splitter);
		m_sipService->registerServer(m_userService->sipAccount(), m_userService->password(), sipServer);
	}
}

// void TranslatorMainWindow::slotUnLogin()
// {
// 	Q_ASSERT(m_l);
// 
// 	if (m_l->isVisible()) {
// 		return;
// 	}
// 
// 	stopPhoneView();
// 
// 	hide();
// }

/*
void TranslatorMainWindow::adjustSystemButtons()
{
    if (isMaximized()) {
        m_ui->sysMaximizeButton->hide();
        m_ui->sysRestoreButton->show();
    } else {
        m_ui->sysMaximizeButton->show();
        m_ui->sysRestoreButton->hide();
    }
}
*/

void TranslatorMainWindow::stopPhoneView()
{
	typedef TranslatorMainWindowOptions Opt;

	// XXX Stop UserService now
	qApp->userService()->stop();

	writeSettings();
}

void TranslatorMainWindow::updateCallDisplay()
{
	
}

void TranslatorMainWindow::slotUserServiceActive()
{
    m_trayIcon->setIcon(windowIcon());
}

void TranslatorMainWindow::slotUserServiceInactive()
{
    // TODO The tray icon' icon will be changed
    m_trayIcon->setIcon(QIcon(windowIcon().pixmap(QSize(16, 16), QIcon::Disabled)));
}

void TranslatorMainWindow::slotSipLog(const QString& msg)
{
//	qDebug() << QPID_DEBUG_PREFIX << msg;
}

void TranslatorMainWindow::slotRegState(int code, const QString& reason)
{
	switch(code) {
		case 200: /* 200/OK*/
			
			break;
		case 403: /* 403/Forbidden */

			break;
		default:
			qDebug() << QPID_DEBUG_PREFIX << code << reason;
			break;
	}
}

void TranslatorMainWindow::slotSipIncommingCall(int callId, const QString& srcLang, const QString& dstLang, const QString& mode, const QString& remote)
{

}

void TranslatorMainWindow::slotSipCallState(int state)
{
	qDebug() << QPID_DEBUG_PREFIX << state;

	switch(state) {
		case 4: /* PJSIP_INV_STATE_CONNECTING */
			break;
		case 5: /* PJSIP_INV_STATE_CONFIRMED */
			
			break;
		case 6: /* PJSIP_INV_STATE_DISCONNECTED */
			
			break;
	}
}

void TranslatorMainWindow::slotReceiveText(int callId, const QString& remote, const QString& text)
{
	
}

void TranslatorMainWindow::slotAudioOutputStateChanged(QAudio::State state)
{
	qDebug() << QPID_DEBUG_PREFIX << "Audio output state changed to " << state;

	switch(state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:
		if (m_audioService && m_callInfo->state()==TranslatorInfo::CallIncomming) {
			m_audioService->startPlay(qApp->tone(QLatin1String("ringing")));
		}
		break;
	}
}

void TranslatorMainWindow::slotDtmfOutputStateChanged(QAudio::State state)
{
	qDebug() << QPID_DEBUG_PREFIX << "Dtmf output state changed to " << state;

	switch(state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:
		if (m_audioDtmfService) {
			m_audioDtmfService->stopPlay();
		}
		break;
	}
}

void TranslatorMainWindow::slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
			if (isMinimized())
				showNormal();
			show();
            raise();
            activateWindow();
            break;
        default:
            break;
    }
}

void TranslatorMainWindow::on_actionAbout_triggered()
{
	
}

void TranslatorMainWindow::on_actionUpdate_triggered()
{
//   qApp->checkUpdates(true);

	
}

void TranslatorMainWindow::on_actionQuit_triggered()
{
	slotQuitApplication();
}

void TranslatorMainWindow::slotMinimizeTray()
{
	hide();
}

void TranslatorMainWindow::slotQuitApplication()
{
	stopPhoneView();
	qApp->quit();
}

void TranslatorMainWindow::slotLoadconfigFinished(const Command::LoadConfigParam& param, const Command::LoadConfigResult& result)
{
	qDebug() << QPID_DEBUG_PREFIX << "Language:";
	foreach(const QString& lang, result.language()) {
		qDebug() << QPID_DEBUG_PREFIX << "--------" << lang;
	}

	qDebug() << QPID_DEBUG_PREFIX << "Country:";
	foreach(const QString& country, result.country()) {
		qDebug() << QPID_DEBUG_PREFIX << "--------" << country;
	}

	qDebug() << QPID_DEBUG_PREFIX << "Service:";
	QMapIterator<QString, QStringList> i(result.service());
	while (i.hasNext()) {
		i.next();
		qDebug() << QPID_DEBUG_PREFIX << "--------" << i.key() << "==>" << i.value();
	}

	qDebug() << QPID_DEBUG_PREFIX << "Major:";
	foreach(const QString& major, result.majorArray()) {
		qDebug() << QPID_DEBUG_PREFIX << "--------" << major;
	}

	qDebug() << QPID_DEBUG_PREFIX << "Last Update Datetime:" << result.lastTime();
}

} // namespace Qpid
