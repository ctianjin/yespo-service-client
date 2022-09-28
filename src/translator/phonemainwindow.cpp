#include <QKeyEvent>
#include <QDesktopWidget>
#include <QDebug>
#include <QUrl>
#include <QTime>
#include <QTimer>
#include <QTextDocument>
#include <QDir>
#include <QSettings>
#include <QDateTime>
#include <QtNetwork/QHostInfo>
#include <QPropertyAnimation>
#include <QtNetwork/QNetworkCookie>
#include <QCloseEvent>
#include <QWindowStateChangeEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMovie>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtSystemInfo/QSystemDeviceInfo> 
#include <QDesktopServices>
#include <QGraphicsOpacityEffect>
#ifndef BUILDSYS_PHONE_ENABLE_TRANSLUCENT
#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#endif // BUILDSYS_PHONE_ENABLE_TRANSLUCENT
#include "phonemainwindow.h"
#include "phoneapplication.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "photoservice.h"
#include "im.h"
#include "cameraservice.h"
#include "yespoproto.h"
#include "ui_phonemainwindow.h"
#include "aboutdialog.h"
#include "audionavigate.h"
#include "tonegoglobal.h"
#include "updatedialog.h"
#include "sippreferencedialog.h"
#include "messagepanel.h"
#include "settingsdialog.h"
#include "boxshadow.h"
#include "pagelogin.h"
#include "remotevideodialog.h"
#include "closemethoddialog.h"
#include "widgetphototranslation.h"
#include "sipnotify.h"
#include "reportnotify.h"
#include "callmode.h"

#include "browser.h"
#include "settings/settingsgeneralpage.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PhoneMainWindow:"
#endif // DEBUG_PREFIX

namespace ns {

const QString PhoneMainWindow::token = QLatin1String("token");
const QString PhoneMainWindow::deviceId = QLatin1String("device_id");
const QString PhoneMainWindow::language = QLatin1String("lang");
const QString PhoneMainWindow::callRecordId = QLatin1String("meet_cdr_id");
const QString PhoneMainWindow::source = QLatin1String("source");
const QString PhoneMainWindow::stunServer1 = QLatin1String("stun.voipbuster.com");
const QString PhoneMainWindow::stunServer2 = QLatin1String("stun.voxgratia.org");
const QString PhoneMainWindow::stunServer3 = QLatin1String("stun.voipstunt.com");
struct PhoneMainWindowOptions
{
	static const QString host_;
	static const QString sip_host_;
	static const QString remember_choose_;
    static const QString always_top_;
	static const QString show_trayicon_;
	static const QString close_window_;
	static const QString language_;
	static const QString close_callin_;
	static const QString callin_prompt_;
	static const QString update_method_;
};
const QString PhoneMainWindowOptions::host_ = QLatin1String("application.host");
const QString PhoneMainWindowOptions::sip_host_ = QLatin1String("application.sip_host");
const QString PhoneMainWindowOptions::remember_choose_ = QLatin1String("ui.settings.general.remember_choose");
const QString PhoneMainWindowOptions::always_top_ = QLatin1String("ui.settings.general.always_top");
const QString PhoneMainWindowOptions::show_trayicon_ = QLatin1String("ui.settings.general.show_trayicon");
const QString PhoneMainWindowOptions::close_window_ = QLatin1String("ui.settings.general.close_window");
const QString PhoneMainWindowOptions::language_ = QLatin1String("ui.settings.languages.language");
const QString PhoneMainWindowOptions::close_callin_ = QLatin1String("ui.settings.callin.close_callin");
const QString PhoneMainWindowOptions::callin_prompt_ = QLatin1String("ui.settings.callin.callin_prompt");
const QString PhoneMainWindowOptions::update_method_ = QLatin1String("ui.settings.updatemethod.update_method");

PhoneMainWindow::PhoneMainWindow(QWidget* parent) :
    FramelessMainWindow(parent),
    m_ui(new Ui_PhoneMainWindow),
    m_opt(new PhoneMainWindowOptions),
    m_trayIcon(0),
    m_trayIconMenu(0),
	m_accountMenu(0),
	m_countReload(5),
	m_networkAddress(),
	m_audioNavDlg(new AudioNavigateDialog),
	m_settings(new SettingsDialog),
	m_widgetPhotoTranslation(new WidgetPhotoTranslation),
	m_reportNotify(new ReportNotify),
	m_callMode(new CallMode),
	m_remoteVideoDialog(new RemoteVideoDialog),
	m_sipPreferenceDialog(new SipPreferenceDialog),
	m_closeMethodDialog(new CloseMethodDialog),
	m_browser(new Browser),
	m_browserReportList(new Browser),
	m_proto(new YespoProto)
{
#ifdef BUILDSYS_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // BUILDSYS_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	if (m_widgetPhotoTranslation) {
		m_widgetPhotoTranslation->setMediaService(qApp->mediaService());
		m_widgetPhotoTranslation->hide();
	}

	if (m_browserReportList && m_browser) {
		connect(m_browserReportList, SIGNAL(requestNewBrowser(const QString&)),
			this, SLOT(slotRequestReport(const QString&)));
		connect(m_browser, SIGNAL(requestRefreshBrowser()), this, SLOT(slotRequestRefreshBrowser()));
	}

	m_ui->pushButtonSipSetting->hide();

	m_animation = new QPropertyAnimation(m_remoteVideoDialog, "geometry");

	connect(m_animation, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),
		this, SLOT(slotAnimationStateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));

	BoxShadow *boxShadow = new BoxShadow();
	boxShadow->setBlurRadius(35.0);
	boxShadow->setDistance(12.0);
	boxShadow->setColor(QColor(0, 0, 0, 70));
	m_ui->window->setGraphicsEffect(boxShadow);
	m_ui->window->setAttribute(Qt::WA_TranslucentBackground);

    // Tray icon
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(qApp->windowIcon());
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_ui->actionAbout);
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(m_ui->actionUpdate);
	m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_ui->actionQuit);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip(windowTitle());

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(slotTrayIconActivated(QSystemTrayIcon::ActivationReason))); 

	m_accountMenu = new QMenu(this);
	m_accountMenu->addAction(m_ui->actionHome);
//  m_accountMenu->addAction(m_ui->actionReport);
	m_accountMenu->addAction(m_ui->actionHistory);
	m_accountMenu->addAction(m_ui->actionProfile);
	m_accountMenu->addSeparator();
	m_accountMenu->addAction(m_ui->actionSettings);
	m_accountMenu->addAction(m_ui->actionDevice);
	m_accountMenu->addSeparator();
 	m_accountMenu->addAction(m_ui->actionProfile);
 	m_accountMenu->addAction(m_ui->actionAbout);
 	m_accountMenu->addSeparator();
	m_accountMenu->addAction(m_ui->actionPassword);
	m_accountMenu->addAction(m_ui->actionLogout);
	m_accountMenu->addAction(m_ui->actionQuit);
	m_ui->pushButtonSystemMenu->setMenu(m_accountMenu);

    readSettings();

	m_audioNavService = qApp->createAudioService(0);
	m_audioNavService->start();

#ifdef BUILDSYS_DTMF_ENABLE
	m_audioDtmfService = qApp->createAudioService(0);
	connect(m_audioDtmfService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotDtmfOutputStateChanged(QAudio::State)));
	m_audioDtmfService->start();
#endif

	importLanguages();

	if (m_audioNavDlg) {
		m_audioNavDlg->setAudioService(m_audioNavService);
	}

	if (m_reportNotify) {
		connect(m_reportNotify, SIGNAL(requestReportList()), this, SLOT(slotRequestReportList()));
	}

	m_ui->stackedWidget->setCurrentWidget(m_ui->pageWelcome);
	connect(m_ui->pageWelcome->loginPage(), SIGNAL(finish(bool)), this, SLOT(slotLoginFinished(bool)));
	connect(m_ui->pageFrameWindow, SIGNAL(callIn()), this, SLOT(slotMainCallIn()));
	connect(m_ui->pageFrameWindow, SIGNAL(callConfirmed(bool)), this, SLOT(slotMainCallConfirmed(bool)));
	connect(m_ui->pageFrameWindow, SIGNAL(callClosed()), this, SLOT(slotMainCallClosed()));
	connect(m_ui->pageFrameWindow, SIGNAL(callSucceed(const QString&)), this, SLOT(slotMainCallSucceed(const QString&)));
	m_ui->pushButtonSystemMenu->hide();
	m_ui->pushButtonSystemHome->hide();
	if (m_proto) {
		m_ui->pageCalling->setProto(m_proto);
		m_ui->pageFrameWindow->setProto(m_proto);
	}
	m_ui->pageFrameWindow->setPageCalling(m_ui->pageCalling);
	
	connect(m_ui->pushButtonSystemHome, SIGNAL(clicked()), m_ui->pageFrameWindow, SLOT(showHome()));

	connect(m_closeMethodDialog, SIGNAL(showTrayIcon(bool)), this, SLOT(slotShowTrayIcon(bool)));
	connect(m_settings->generalPage(), SIGNAL(alwaysOnTop(bool)), this, SLOT(slotAlwaysOnTop(bool)));
	connect(m_settings->generalPage(), SIGNAL(showTrayIcon(bool)), this, SLOT(slotShowTrayIcon(bool)));

	connect(m_sipPreferenceDialog.data(), SIGNAL(transportChanged(int)), this, SLOT(slotTransportChanged(int)));

	connect(this, SIGNAL(notifyMove(const QPoint&)), m_remoteVideoDialog, SLOT(slotMove(const QPoint&)));

	/* Video support */
	connect(m_ui->pageCalling, SIGNAL(showVideo()), this, SLOT(showVideo()));

	QNetworkAccessManager* manger = new QNetworkAccessManager();
	//QNetworkReply* reply = manger->get(QNetworkRequest(QUrl(QLatin1String("http://checkip.dyndns.org:8245/"))));
	QNetworkReply* reply = manger->get(QNetworkRequest(QUrl(QLatin1String("http://city.ip138.com/ip2city.asp"))));
	connect(manger, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotAssignIP(QNetworkReply*)));

	connect(m_proto, SIGNAL(socketError(const QString&)), this, SLOT(slotSocketError(const QString&)));
	connect(m_proto, SIGNAL(connected()), this, SLOT(slotConnected()));
	connect(m_proto, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
	connect(m_proto, SIGNAL(loginFinished(int)), this, SLOT(slotLoginFinished(int)));
	connect(m_proto, SIGNAL(logoutFinished()), this, SLOT(slotLogoutFinished()));
	connect(m_proto, SIGNAL(repetitionLoginNotify()), this, SLOT(slotReLogin()));
}

PhoneMainWindow::~PhoneMainWindow()
{
	if (m_audioNavService) {
		m_audioNavService->stop();
	}

#ifdef BUILDSYS_DTMF_ENABLE
	if (m_audioDtmfService) {
		m_audioDtmfService->stopPlay();
		m_audioDtmfService->stop();
	}
#endif
}

UserService* PhoneMainWindow::userService() const
{
	return m_userService;
}

void PhoneMainWindow::setUserService(UserService* userService)
{
	if (m_userService) {
		disconnect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
		disconnect(m_userService, SIGNAL(inactive()), this, SLOT(slotUserServiceInactive()));
	}

	m_userService = userService;
	m_ui->pageCalling->setUserService(m_userService);
	m_ui->pageWelcome->setUserService(m_userService);
	m_ui->pageFrameWindow->setUserService(m_userService);

	if (m_userService) {
		connect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
		connect(m_userService, SIGNAL(inactive()), this, SLOT(slotUserServiceInactive()));
	}
}

SipService* PhoneMainWindow::sipService() const
{
	return m_sipService;
}

void PhoneMainWindow::setSipService(SipService* sipService)
{
	if (m_sipService) {
	}

	m_sipService = sipService;
	
	if (m_remoteVideoDialog) {
		m_ui->pageCalling->setRemoteVideoWidget(m_remoteVideoDialog);
		m_remoteVideoDialog->setSipService(m_sipService);
	}

	m_ui->pageFrameWindow->setSipService(m_sipService);
	m_ui->pageCalling->setSipService(m_sipService);

	if (m_sipService) {
	}
}

AudioService* PhoneMainWindow::audioService() const
{
	return m_audioService;
}

void PhoneMainWindow::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
	}

	m_audioService = audioService;
	m_ui->pageFrameWindow->setAudioService(m_audioService);

	if (m_audioService) {
	}
}

CommandService* PhoneMainWindow::commandService() const
{
	return m_commandService;
}

void PhoneMainWindow::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(httpError(ns::Service::Error, const QString&, const QString&)),
			this, SLOT(slotServiceError(ns::Service::Error, const QString&, const QString&)));
		disconnect(m_commandService, SIGNAL(loadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)), 
			this, SLOT(slotLoadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)));
		disconnect(m_commandService, SIGNAL(setclientinfoFinished(Command::SetClientInfoParam, Command::SetClientInfoResult)),
			this, SLOT(slotSetClientInfoFinished(Command::SetClientInfoParam, Command::SetClientInfoResult)));
// 		disconnect(m_commandService, SIGNAL(getserverFinished(Command::GetServerParam, Command::GetServerResult)),
// 			this, SLOT(slotGetServerFinished(Command::GetServerParam, Command::GetServerResult)));
	}

	m_commandService = commandService;
	m_ui->pageFrameWindow->setCommandService(m_commandService);

	{
		QtMobility::QSystemDeviceInfo deviceInfo;
		qWarning() << DEBUG_PREFIX << deviceInfo.uniqueDeviceID().constData();
		m_commandService->setCommonItem(QLatin1String("partner"), QLatin1String("1.0"), QLocale().name(), 
			QString::fromLocal8Bit(deviceInfo.uniqueDeviceID().constData()), 
			QLatin1String("Windows"));
	}

	if (m_commandService) {
		connect(m_commandService, SIGNAL(httpError(ns::Service::Error, const QString&, const QString&)),
			this, SLOT(slotServiceError(ns::Service::Error, const QString&, const QString&)));
		connect(m_commandService, SIGNAL(loadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult)), 
			this, SLOT(slotLoadconfigFinished(Command::LoadConfigParam, Command::LoadConfigResult))/*, Qt::QueuedConnection*/);
		connect(m_commandService, SIGNAL(setclientinfoFinished(Command::SetClientInfoParam, Command::SetClientInfoResult)),
			this, SLOT(slotSetClientInfoFinished(Command::SetClientInfoParam, Command::SetClientInfoResult)));
// 		connect(m_commandService, SIGNAL(getserverFinished(Command::GetServerParam, Command::GetServerResult)),
// 			this, SLOT(slotGetServerFinished(Command::GetServerParam, Command::GetServerResult)));
	}
}

PhotoService* PhoneMainWindow::photoService() const
{
	return m_photoService;
}

void PhoneMainWindow::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
	}

	m_photoService = photoService;
	m_ui->pageFrameWindow->setPhotoService(m_photoService);

	if (m_photoService) {
	}
}

CameraService* PhoneMainWindow::cameraService() const
{
	return m_cameraService;
}

void PhoneMainWindow::setCameraService(CameraService* cameraService)
{
	if (m_cameraService) {
	}

	m_cameraService = cameraService;
	m_ui->pageFrameWindow->setCameraService(m_cameraService);
	m_ui->pageCalling->setCameraService(m_cameraService);
	if (m_audioNavDlg)
		m_audioNavDlg->setCameraService(m_cameraService);

	if (m_cameraService) {
	}
}

WidgetPhotoTranslation* PhoneMainWindow::widgetPhotoTranslation() const 
{
	if (m_widgetPhotoTranslation)
		return m_widgetPhotoTranslation;
	return NULL;
}

void PhoneMainWindow::setPreference(Preference* pref)
{
    PreferenceSupport::setPreference(pref);
    typedef PhoneMainWindowOptions Opt;

	m_ui->pageWelcome->setPreference(new Preference(pref, QLatin1String("ui.settings.general")));
	m_sipPreferenceDialog->setPreference(new Preference(pref, QLatin1String("ui.service.sip")));
	m_closeMethodDialog->setPreference(new Preference(pref, QLatin1String("ui.settings.general")));
	m_ui->pageFrameWindow->setPreference(pref);

	QString language = pref->getString(Opt::language_, QLatin1String("zh_CN")); 
	qApp->changeLanguage(language);

	if (m_userService) {
		m_userService->setServiceHost(pref->getString(Opt::host_, QString())); 
		m_userService->setServiceSipHost(pref->getString(Opt::sip_host_, QString()));  

#ifdef BUILDSYS_DEMO 
		m_userService->setServiceHost(QLatin1String("demo-api.yesporar.com")); 
#endif

#if defined(BUILDSYS_LOCAL) && !defined(BUILDSYS_DEMO)
		m_userService->setServiceHost(QLatin1String("192.168.10.245")); 
#endif
	}
}

void PhoneMainWindow::loadConfig()
{
	if (m_commandService) {
		QDateTime t = QDateTime::currentDateTime();
		Command::LoadConfigParam param;
		param.setLastTime(t.toString(QLatin1String("yyyy-MM-dd")));
		param.setReloadConfig(1);
		m_commandService->loadconfig(param);
	}
}

QString PhoneMainWindow::getNetworkAddress() const 
{
	return m_networkAddress;
}

bool PhoneMainWindow::parseServer(QStringList serverList, StunAddress4& address, QString& server)
{
	bool ret;
	if (serverList.count() > 1) {
		ret = stunParseServerName(serverList.first().toAscii().data(), address);
		if (!ret) {
			qDebug() << serverList.first() << "is not a valid host name";
			serverList.removeFirst();
			parseServer(serverList, address, server);
		} else {
			server = serverList.first();
			return true;
		}
	} else if (serverList.count() == 1) {
		ret = stunParseServerName(serverList.first().toAscii().data(), address);
		if (!ret) {
			qDebug() << serverList.first() << "is not a valid host name";
			return false;
		} else {
			server = serverList.first();
			return true;
		}
	} else {
		server = QString();	
		return false;
	}
	return true;
}

void PhoneMainWindow::getNetworkAddressFormStun(QString& networkAddr)
{
	QStringList stunList;
	stunList << stunServer1 << stunServer2 << stunServer3;
	bool verbose = false;

	StunAddress4 stunServerAddr;
	stunServerAddr.addr=0;

	int srcPort=0;
	StunAddress4 sAddr;
	sAddr.addr = 0;
	sAddr.port = 0;

	QString server;
	bool ret = parseServer(stunList, stunServerAddr, server);
	if (ret) {
		if (stunTest(stunServerAddr, 1, verbose, &sAddr)) {
			QHostAddress host(sAddr.addr);
			networkAddr = host.toString();
			qWarning() << DEBUG_PREFIX << "stun:" << server << sAddr.addr << sAddr.port << networkAddr;
		}
		else {
			qWarning() << DEBUG_PREFIX << "could not get sAddr.addr";
		}
	} else {
		qWarning() << DEBUG_PREFIX << "could not parse host";
		networkAddr = QString();
	}
}

void PhoneMainWindow::autoLogin(const QString& userName, const QString& password)
{
	m_ui->pageWelcome->autoLogin(userName, password);
}

void PhoneMainWindow::notifySipStatus(const QString& sipServiceHost)
{
	if (m_commandService) {
		Command::SetClientInfoParam param;
		param.setLastDomainCall(m_domainCall);
		m_commandService->setclientinfo(param);
	}
}

void PhoneMainWindow::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef PhoneMainWindowOptions Opt;
    Preference* pref = preference();

	if(pref) {

	}
}

void PhoneMainWindow::changeEvent(QEvent* event)
{
    FramelessMainWindow::changeEvent(event);

	switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
			retranslateTr();
            break;
		case QEvent::WindowStateChange:
			if (isActiveWindow()) {
				if (m_ui->pageCalling->videoEnabled()) {
					if (m_remoteVideoDialog) {
						if (m_remoteVideoDialog->isMinimized())
							m_remoteVideoDialog->showNormal();
						m_remoteVideoDialog->raise();
						m_remoteVideoDialog->activateWindow();
					}
				} 
			}
			if(isMinimized()) {
				if (m_ui->pageCalling->videoEnabled()) {
					if (m_remoteVideoDialog)
						m_remoteVideoDialog->showMinimized();
				}
			} 
			break;
        default:
            break;
    }
}

void PhoneMainWindow::closeEvent(QCloseEvent* event)
{
    FramelessMainWindow::closeEvent(event);

	typedef PhoneMainWindowOptions Opt;
	Preference* pref = preference();
	if (pref) {
		int closeWindow;
		bool remember = pref->getBool(Opt::remember_choose_, false);
		if (!remember) {
			if (m_closeMethodDialog->exec() == QDialog::Accepted) {
				closeWindow = pref->getInt(Opt::close_window_, 0);
				if (closeWindow == 0) {
					setVisible(false);
					event->ignore();
					return;
				}
			} else {
				event->ignore();
				return;
			}
		} else {
			closeWindow = pref->getInt(Opt::close_window_, 0);
			if (closeWindow == 0) {
				setVisible(false);
				return;
			}
		}
	} 

	//FIXME: Disable the close button to avoid double un-initialize the service
	m_ui->pushButtonSystemClose->setEnabled(false);

	slotQuitApplication();
}

void PhoneMainWindow::showEvent(QShowEvent *event)
{
	QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority); 
	
	FramelessMainWindow::showEvent(event);
}

void PhoneMainWindow::showMainWindow()  
{
#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    qApp->setActivationWindow(this);
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

	m_ui->frameCaption->setStyleSheet(QLatin1String("#frameCaption { border-bottom: 1px solid #015c97; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #0078c7, stop: 1.0 #016aae);}"));
	m_ui->pageFrameWindow->showMainWindow();
	m_ui->pageFrameWindow->setHistoryLink(m_homeLink);

	if (m_widgetPhotoTranslation)
		m_ui->pageFrameWindow->setWidgetPhotoTranslation(m_widgetPhotoTranslation);

	m_ui->stackedWidget->setCurrentWidget(m_ui->pageFrameWindow);
#if 0
	m_ui->pageCalling->initCall();
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageCalling);
#endif
	m_ui->pushButtonSystemMenu->show(); 
	m_ui->pushButtonSystemHome->show(); 

// 	if (m_sipService && m_userService) {
// 		m_sipService->registerServer(m_userService->callerId(), m_userService->secret(), 
// 			m_userService->sipServiceHost());
// 	}

// 	if (m_iMessage && m_userService) {
// 		m_iMessage->setLoginUid(m_userService->callerId());
// 		m_iMessage->setLoginToken(QLatin1String("aaaaaaaa"));
// 		m_iMessage->setLoginNetworkType(1);
// 		m_iMessage->setLoginUserType(1);
// 		m_iMessage->setImAddress(m_userService->statusServiceHostNoPort());
// 		m_iMessage->setImPort(9500);
// 		m_iMessage->login();
// 	}

// 	m_domainCall = m_userService->sipServiceHost();	
// 
// 	notifySipStatus(m_domainCall);
// 
// 	qDebug() << DEBUG_PREFIX << m_userService->sipServerList();

// 	if(m_sipNotify) {
// 		m_ui->pageFrameWindow->setSipNotify(m_sipNotify);
// 		if (m_userService) {
// 			m_sipNotify->setDomainCall(m_userService->sipServiceHost());
// 			m_sipNotify->setSipServerList(m_userService->sipServerList());
// 		}
// 	}

	if (m_reportNotify) {
		m_ui->pageFrameWindow->setReportNotify(m_reportNotify);
	}

	if (m_callMode) {
		//m_ui->pageCalling->setCallMode(m_callMode);
		m_ui->pageFrameWindow->setCallMode(m_callMode);
	}

	 show();

	 typedef PhoneMainWindowOptions Opt;
	 Preference* pref = preference();
	 if (pref) {
		 int closeWindow = pref->getInt(Opt::close_window_, 0);
		 bool showTrayIcon = pref->getBool(Opt::show_trayicon_, true);

		 if (closeWindow==0) {
			 m_trayIcon->setVisible(true);
		 } else {
			 m_trayIcon->setVisible(showTrayIcon);
		 }
	 }

	 on_actionDevice_triggered();
	if (m_networkAddress.isEmpty())
		getNetworkAddressFormStun(m_networkAddress);
	if (!m_networkAddress.isEmpty()) {
		m_ui->pageFrameWindow->setNetworkAddress(m_networkAddress);
		if (m_audioNavDlg)
			m_audioNavDlg->setNetworkAddress(m_networkAddress);
	}
	  qApp->setQuitOnLastWindowClosed(false);
}

void PhoneMainWindow::showMinimized()
{
	FramelessMainWindow::showMinimized();
}

void PhoneMainWindow::logout()
{
	m_ui->pushButtonSystemMenu->hide(); 
	m_ui->pushButtonSystemHome->hide();

	if (m_settings) {
		m_settings->hide();
	}

	if (m_audioNavDlg) {
		m_audioNavDlg->hide();
	}

	if (m_browser)
		m_browser->hide();

	if (m_browserReportList)
		m_browserReportList->hide();

	if (m_proto && m_userService) {
		LogoutReq req;
		req.setUserId((qint64)m_userService->callerId().toLongLong());
		m_proto->logout(req);
	}

	m_ui->frameCaption->setStyleSheet(QLatin1String(" #frameCaption {	background: transparent;}"));

 	m_ui->pageFrameWindow->logout();
 	m_ui->pageWelcome->logout();
	m_ui->pageWelcome->setVersion(qApp->applicationVersion());

	m_ui->stackedWidget->setCurrentWidget(m_ui->pageWelcome);

	qApp->userService()->stop();

	qApp->setQuitOnLastWindowClosed(true);
}

void PhoneMainWindow::showVideo()
{
	if (isMinimized()) {
		showNormal();
	} else {
		show();
	}
	raise();
	activateWindow();
	initVideoWidget();
}

void PhoneMainWindow::initVideoWidget()
{
	QRect screen = QApplication::desktop()->availableGeometry();
	int positionLeft = screen.width() - geometry().right();
	if (m_remoteVideoDialog) {
		if (positionLeft < m_remoteVideoDialog->width()) {
			move(screen.width() - m_remoteVideoDialog->width() - width(), height() / 2);
		}
		m_remoteVideoDialog->setPosition(width());
	}

	SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	m_animation->setDuration(500);
	m_animation->setStartValue(QRect(geometry().right() - m_remoteVideoDialog->width() - 35, geometry().top() + 35, 0, m_remoteVideoDialog->height()));
	m_animation->setEndValue(QRect(geometry().right() - 35, geometry().top() + 35, m_remoteVideoDialog->width(), m_remoteVideoDialog->height()));
	m_animation->start();
	if (m_remoteVideoDialog)
		m_remoteVideoDialog->showNormal();
	SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void PhoneMainWindow::slotAssignIP(QNetworkReply* reply)
{
	if(reply->error() == QNetworkReply::NoError)	{
		QString ipaddress = QString::fromUtf8(reply->readAll().data());

		QTextDocument doc;
		doc.setHtml( ipaddress );

		QString text = doc.toPlainText();
		QString data = QString();
		for(int i = 0; i < text.length(); i++) {
			if(text[i].isDigit() || text[i] == QLatin1Char('.'))
				data.append(text[i]);
		}
		m_networkAddress = data/*.right(format.length() - 20).trimmed()*/;
		qWarning() << DEBUG_PREFIX << doc.toPlainText() << m_networkAddress;

		m_ui->pageFrameWindow->setNetworkAddress(m_networkAddress);
	}

	reply->deleteLater();
}

void PhoneMainWindow::slotRequestReportList()
{
	if (m_noReportListLink.isEmpty() || !m_commandService)
		return;
	if (!m_browserReportList)
		return;
	QUrl url(m_noReportListLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(language, qApp->language());
	//url.addQueryItem(token, m_commandService->token());

	QNetworkRequest request;
	addCommonRequestHeaders(request);

	qDebug() << DEBUG_PREFIX << request.url();

	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_commandService->token()).toAscii()).toBase64());
	//request.setRawHeader("SetCookieHeader", m_browser->requestCookies());

	request.setUrl(url);

	qDebug() << DEBUG_PREFIX << url.toString() << m_browser->requestCookies();
	request.setRawHeader("Set-Cookie", m_browser->requestCookies());
	m_browserReportList->loadRequest(request);
}

void PhoneMainWindow::slotRequestReport(const QString& cdrId)
{
	if (cdrId.isEmpty() || m_addReportLink.isEmpty() || !m_commandService) 
		return;

	if (!m_browser)
		return;

	QUrl url(m_addReportLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(language, qApp->language());
	//url.addQueryItem(language, QLatin1String("zh"));
	url.addQueryItem(source, QLatin1String("list"));
	url.addQueryItem(callRecordId,cdrId);

	QNetworkRequest request;
	addCommonRequestHeaders(request);

	qDebug() << DEBUG_PREFIX << request.url() << m_browser->requestCookies();

	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_commandService->token()).toAscii()).toBase64());
	//	request.setRawHeader("SetCookieHeader", m_browser->requestCookies());

	request.setUrl(url);

	qDebug() << DEBUG_PREFIX << url.toString() << m_browser->requestCookies();
	request.setRawHeader("Set-Cookie", m_browser->requestCookies());
	m_browser->loadRequest(request);
}

void PhoneMainWindow::slotRequestRefreshBrowser()
{
	if (m_browserReportList) {
		m_browserReportList->reload();
		m_ui->pageFrameWindow->refreshReportNum();
	}
}

void PhoneMainWindow::stopPhoneView()
{
	// XXX Stop UserService now
	// XXX As UserService will not clean the host while cleanUp()!
	if (m_proto && m_userService) {
		m_proto->disconnect();
		LogoutReq req;
		req.setUserId((qint64)m_userService->callerId().toLongLong());
		m_proto->logout(req);
	}
	qApp->userService()->setServiceHost(QString());
	qApp->userService()->stop();

	writeSettings();
}

void PhoneMainWindow::importLanguages()
{
	QFile file(QLatin1String(":/data/language.xml"));
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
		return;

	QDomDocument doc;
	doc.setContent(&file);

	QDomElement root = doc.documentElement();
	if (root.isNull())
		return;

	QDomNodeList items = root.elementsByTagName(QLatin1String("item"));
	int index = 0;
	for (; index < items.count(); ++index) {
		QDomNode item = items.at(index);
		if (item.isNull())
			continue;
		QDomElement eleShort = item.firstChildElement(QLatin1String("short"));
		QDomElement eleFull = item.firstChildElement(QLatin1String("full"));
		if (eleShort.isNull() || eleFull.isNull())
			continue;
		m_languages.insert(eleShort.text(), eleFull.text());
	}		
}

void PhoneMainWindow::retranslateTr()
{
	
}

void PhoneMainWindow::slotUserServiceActive()
{
    m_trayIcon->setIcon(windowIcon());
}

void PhoneMainWindow::slotUserServiceInactive()
{
    // TODO The tray icon' icon will be changed
    m_trayIcon->setIcon(QIcon(windowIcon().pixmap(QSize(16, 16), QIcon::Disabled)));
}

#ifdef BUILDSYS_DTMF_ENABLE
void PhoneMainWindow::slotDtmfOutputStateChanged(QAudio::State state)
{
	qDebug() << DEBUG_PREFIX << "Dtmf output state changed to " << state;

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
#endif

void PhoneMainWindow::slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
			if (isMinimized()) {
				showNormal();
			} else {
				show();
			}
            raise();
            activateWindow();
            break;
        default:
            break; 
    }
}

void PhoneMainWindow::on_actionAbout_triggered()
{
	AboutDialog aboutDlg;
	aboutDlg.exec();
}

void PhoneMainWindow::on_actionUpdate_triggered()
{
	if (m_updateDialog) {
		m_updateDialog->setUserUpdate(true);
	}

	loadConfig();
}

void PhoneMainWindow::on_actionQuit_triggered()
{
	slotQuitApplication();
}

void PhoneMainWindow::on_actionSettings_triggered()
{
	if (m_settings) {
		m_settings->setUserService(m_userService);
		m_settings->setPhotoService(m_photoService);
		m_settings->setCommandService(m_commandService);
		m_settings->show();
	}
}

void PhoneMainWindow::on_actionDevice_triggered()
{
	if (m_audioNavDlg) {
		m_audioNavDlg->initTest();
		m_audioNavDlg->show();
	}
}

void PhoneMainWindow::on_actionLogout_triggered()
{
	logout();
}

void PhoneMainWindow::on_actionHome_triggered()
{
	if (m_homeLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_homeLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_actionReport_triggered()
{
	if (m_reportLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_reportLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_actionHistory_triggered()
{
	if (m_cdrLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_cdrLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	qDebug() << DEBUG_PREFIX << url.toString();
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_actionProfile_triggered()
{
	if (m_profileLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_profileLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_actionPrivacy_triggered()
{
	if (m_privacyLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_privacyLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_actionPassword_triggered()
{
	if (m_passwordLink.isEmpty() || !m_commandService)
		return;
	QUrl url(m_passwordLink);
	url.addQueryItem(deviceId, m_commandService->deviceId());
	url.addQueryItem(token, m_commandService->token());
	//url.addQueryItem(language, qApp->language());
	url.addQueryItem(language, QLatin1String("zh"));
	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_pushButtonSipSetting_clicked()
{
// 	if (m_sipPreferenceDialog) {
// 		m_sipPreferenceDialog->show();
// 	}
}

void PhoneMainWindow::slotLoginFinished(bool succeed)
{
	if (succeed) {
		if (m_commandService && m_userService) {
			m_commandService->setCookies(m_userService->cookies());
			m_commandService->setToken(m_userService->token());
		}

		qDebug() << m_userService->imServiceHost();
		if (m_proto && m_userService) {
			m_proto->connect(m_userService->imServiceHostNoPort(), 9500);
		}

		//showMainWindow();
		
	} else {
		qApp->userService()->stop();
	}
}

void PhoneMainWindow::slotMainCallIn()
{
	if (isMinimized()) {
		QApplication::alert(this);
		m_ui->pageFrameWindow->callSplashNotify();
	}
	else {
		typedef PhoneMainWindowOptions Opt;
		Preference* pref = preference();
		if (pref) {

			bool closeCallin = pref->getBool(Opt::close_callin_, false);
	//		int callinPrompt = pref->getInt(Opt::callin_prompt_, 0);

			if (!closeCallin/* && callinPrompt==2*/) {			
				if (isMinimized())
					showNormal();
				if (isHidden())
					show();
					raise();
					activateWindow();
			}
		}
	}
}
void PhoneMainWindow::slotMainCallConfirmed(bool callLocal)
{
	if (isMinimized()) {
		showNormal();
	}
	if (isHidden()) {
		show();
	}
	raise();
	activateWindow();

	if (m_callMode)
		m_ui->pageCalling->setCallMode(m_callMode);

	m_ui->pageCalling->initCall(callLocal);
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageCalling);
}

void PhoneMainWindow::slotMainCallClosed()
{
	m_ui->pageCalling->uninitCall();

	if (m_ui->stackedWidget->currentWidget() != m_ui->pageWelcome) {
		m_ui->stackedWidget->setCurrentWidget(m_ui->pageFrameWindow);
	}
	
	if (m_remoteVideoDialog)
		m_remoteVideoDialog->closeWindow();

	//m_ui->stackedWidget->setCurrentWidget(m_ui->pageFrameWindow);
}

void PhoneMainWindow::slotMainCallSucceed(const QString& cdrId)
{
	if (cdrId.isEmpty() || m_addReportLink.isEmpty() || !m_browser)
		return;

	if (m_browserReportList && m_browserReportList->isVisible())
		m_browserReportList->reload();

	if (m_browser) {
		QUrl url(m_addReportLink);

		if (m_commandService) {
			url.addQueryItem(deviceId, m_commandService->deviceId());
			//url.addQueryItem(token, m_commandService->token());
		}

		url.addQueryItem(callRecordId, cdrId);
		url.addQueryItem(language, qApp->language());

		QNetworkRequest request;
		addCommonRequestHeaders(request);

		qDebug() << DEBUG_PREFIX << request.url();

		if (m_userService)
			request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_commandService->token()).toAscii()).toBase64());
		//request.setRawHeader("SetCookieHeader", m_browser->requestCookies());

		request.setUrl(url);

		qDebug() << DEBUG_PREFIX << url.toString() << m_browser->requestCookies();

		request.setRawHeader("Set-Cookie", m_browser->requestCookies());

		if (!m_browser->url().isEmpty() && m_browser->isVisible()) {
			qDebug() << DEBUG_PREFIX << m_browser->url().toString();
			PromptDialog promptDialog;
			if (promptDialog.question(tr("Current report is not finished yet, are you sure to cover this?"), PromptDialog::yes, false) == PromptDialog::Accepted) {
				//m_browser->setUrl(url);
				m_browser->loadRequest(request);
			}
		} else {
			//m_browser->setUrl(url);
			m_browser->loadRequest(request);
		}
	}
}

void PhoneMainWindow::slotAlwaysOnTop(bool on)
{
#ifdef Q_OS_WIN
	if (on) {
		SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	} else {
		SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
#else
	Qt::WindowFlags flags = windowFlags();
	if (on) {
		flags |= Qt::WindowStaysOnTopHint;
	} else {
		flags &= ~Qt::WindowStaysOnTopHint;
	}
	setWindowFlags(flags);
	show();
#endif
}

void PhoneMainWindow::slotShowTrayIcon(bool show)
{
	m_trayIcon->setVisible(show);
}

void PhoneMainWindow::slotTransportChanged(int transport)
{
// 	if (m_sipService) {
// 		m_sipService->reconfig(transport);
// 	}
}

void PhoneMainWindow::slotAnimationStateChanged(QAbstractAnimation::State newState,
												QAbstractAnimation::State oldState)
{
	if (newState != QAbstractAnimation::Running) {
		if (m_remoteVideoDialog) {
			m_remoteVideoDialog->update();
			m_remoteVideoDialog->startDuration();
			m_remoteVideoDialog->startNetworkDetect();
		}	
	}
}

void PhoneMainWindow::slotSocketError(const QString& errorMsg)
{
	qWarning() << DEBUG_PREFIX << "Socket error occurred";
	if (m_ui->stackedWidget->currentWidget() != m_ui->pageWelcome)
		logout();
	m_ui->pageWelcome->loginPage()->accessSvrError(errorMsg);

	qApp->userService()->stop();
}

void PhoneMainWindow::slotConnected()
{
	qDebug() << DEBUG_PREFIX << "socket to access server has been connected";
	if (m_proto && m_userService) {
		LoginReq req;
		req.setUserId((qint64)m_userService->callerId().toLongLong());
		req.setToken(m_userService->token());
		req.setUserType(1);
		m_proto->login(req);
	}
}

void PhoneMainWindow::slotDisconnected()
{
}

void PhoneMainWindow::slotLoginFinished(int rspCode)
{
	if (rspCode == 0) {
		m_ui->pageWelcome->loginPage()->hideMask();
		showMainWindow();
	} else {
		m_ui->pageWelcome->loginPage()->accessSvrError(tr("Sign in to access server failed"));
		qApp->userService()->stop();
	}
}

void PhoneMainWindow::slotLogoutFinished()
{
	if (m_proto)
		m_proto->disconnect();
}

void PhoneMainWindow::slotReLogin()
{
	qWarning() << DEBUG_PREFIX << "Repetition login, quit";
	logout();
}

void PhoneMainWindow::addCommonRequestHeaders(QNetworkRequest& request)
{
	int timestamp = QDateTime::currentDateTime().toTime_t();
	QString strTimestamp = QString::number(timestamp);
	QByteArray appid  = QByteArray("A61201281860668");
	QByteArray appkey = QByteArray("10990ED5D-92AE-4E43-8470-5FCC66A550B3");
	QByteArray secret = QCryptographicHash::hash(appid + appkey + strTimestamp.toLocal8Bit(), QCryptographicHash::Sha1).toHex().constData();

	QString time = strTimestamp.prepend(QLatin1String("."));
	secret.append(QByteArray(time.toAscii()));

	request.setRawHeader("X-YesPo-AppId", appid);
	request.setRawHeader("X-YesPo-AppSecret", secret);
	request.setRawHeader("X-YesPo-AppClient", "partner");
	request.setRawHeader("X-YesPo-AppVer", "v1");
}

void PhoneMainWindow::slotMinimizeTray()
{
	hide();
}

void PhoneMainWindow::slotQuitApplication()
{
	stopPhoneView();
	qApp->quit();
}

void PhoneMainWindow::slotServiceError(ns::Service::Error error, const QString& token, const QString& reason)
{
	qWarning() << DEBUG_PREFIX << error << token << reason;
}

void PhoneMainWindow::slotLoadconfigFinished(const Command::LoadConfigParam& param, const Command::LoadConfigResult& result)
{
	if (result.config().isValid() && result.config().canConvert<QVariantMap>()) {
		QVariantMap configMap = result.config().toMap();
		QVariant configList = configMap.value(QLatin1String("list"));
		if (configList.isValid() && configList.canConvert<QVariantMap>()) {
			QVariantMap configListMap = configList.toMap();
			QVariant configListLink = configListMap.value(QLatin1String("link"));
			if (configListLink.isValid() && configListLink.canConvert<QVariantMap>()) {
				QVariantMap linkMap = configListLink.toMap();
			
				m_cdrLink = linkMap.value(QLatin1String("cdrhistory")).toString();
				m_passwordLink = linkMap.value(QLatin1String("changepwd")).toString();
				m_forgetPasswordLink = linkMap.value(QLatin1String("forgetpwd")).toString();
				m_homeLink = linkMap.value(QLatin1String("home")).toString();
				m_profileLink = linkMap.value(QLatin1String("info")).toString();
				m_privacyLink = linkMap.value(QLatin1String("privacy")).toString();
				m_reportLink = linkMap.value(QLatin1String("report")).toString();
				m_noReportListLink = linkMap.value(QLatin1String("noreportlist")).toString();
				m_addReportLink = linkMap.value(QLatin1String("addreport")).toString();
			}

			m_notice = configListMap.value(QLatin1String("notice")).toString();
		}
	}

	qDebug() << DEBUG_PREFIX << result.client(); 
	if (result.client().isValid() && result.client().canConvert<QVariantMap>()) {
		typedef PhoneMainWindowOptions Opt;
		Preference* pref = preference();
		int updateMethod = -1;

		QVariantMap clientMap = result.client().toMap();
		m_clientVersion = clientMap.value(QLatin1String("version")).toString();

		QVariant clientList = clientMap.value(QLatin1String("list"));
		if (clientList.isValid() && clientList.canConvert<QVariantMap>()) {

			QVariantMap clientListMap = clientList.toMap();
			m_clientLink = clientListMap.value(QLatin1String("downloadurl")).toString();
			m_clientForce = clientListMap.value(QLatin1String("ismustupgrade")).toString();

//			m_clientVersion = QLatin1String("5");

			if (pref) {
				updateMethod = pref->getInt(Opt::update_method_, 0);
			}
			
 			if (!m_updateDialog) {
 				m_updateDialog.reset(new UpdateDialog(this, updateMethod));
 			}
 
 			if (m_updateDialog) {
 				if (m_updateDialog->updateMethod() != updateMethod) {
 					m_updateDialog->setUpdateMethod(updateMethod);
 				}
 				m_updateDialog->setMediaService(qApp->mediaService());
 				if (m_updateDialog->startUpdate(m_clientVersion, m_clientLink, m_clientForce==QLatin1String("1"))) {
 					m_updateDialog->exec();
 				}
 			}		
		}
	}

	m_ui->pageWelcome->loginPage()->setForgetPasswordLink(m_forgetPasswordLink);
}

void PhoneMainWindow::slotSetClientInfoFinished(const Command::SetClientInfoParam& param, const Command::SetClientInfoResult& result)
{
	if (result.common().error().isEmpty()) {
		qWarning() << "setclientinfo:" << param.lastDomainCall();
	}
}

} // namespace ns
