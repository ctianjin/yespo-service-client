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
#include <QMovie>
#include <QFontMetricsF>
#include <QVariantMap>
#include <QDesktopServices>
#include <QAction>
#include <QActionGroup>
#include <QLocale>
#include <QRegExpValidator>
#ifndef QPID_PHONE_ENABLE_TRANSLUCENT
#include <QBitmap>
#include <QPainter>
#include <QPixmap>
#endif // QPID_PHONE_ENABLE_TRANSLUCENT
#include "phonemainwindow.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "mediaservice.h"
#include "ui_phonemainwindow.h"
#include "logindialog.h"
#include "aboutdialog.h"
#include "audionavigate.h"
#include "callinfo.h"
#include "callsplash.h"
#include "updatedialog.h"
#include "messagepanel.h"
#include "settingsdialog.h"
#include "simplecrypt.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneMainWindow:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneMainWindowOptions
{
    static const QString host_;
	static const QString account_;
	static const QString password_;
	static const QString remember_;
	static const int max_name_length_;
};

const QString PhoneMainWindowOptions::host_ = QLatin1String("application.host");
const QString PhoneMainWindowOptions::account_ = QLatin1String("application.account");
const QString PhoneMainWindowOptions::password_ = QLatin1String("application.password");
const QString PhoneMainWindowOptions::remember_ = QLatin1String("application.remember");
const int PhoneMainWindowOptions::max_name_length_ = 254;

PhoneMainWindow::PhoneMainWindow(QWidget* parent) :
    MainWindow(parent),
    m_ui(new Ui_PhoneMainWindow),
    m_opt(new PhoneMainWindowOptions),
    m_trayIcon(0),
    m_trayIconMenu(0),
	m_accountMenu(0),
	m_statusMenu(0),
	m_l(0),
	m_callSplash(0),
	m_movie(0),
	m_messagePanel(0),
	m_sipAccountID(-1),
	m_contactsFilterMenu(0),
	m_contactType(ContactTypeAll),
	m_recentFilterMenu(0),
	m_recentType(RecentTypeAll),
	m_languageActions(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->stackedWidget->setCurrentWidget(m_ui->pageLogin);
	m_ui->widgetCallMinimize->hide();
	m_ui->labelMessage->hide();
	m_ui->lineEditAccount->setMaxLength(PhoneMainWindowOptions::max_name_length_);
//	m_ui->lineEditAccount->setValidator(new QRegExpValidator(QRegExp(QLatin1String("[_a-zA-Z0-9]+$")))); //Email has so many regex, so not set limitation

 	m_ui->lineEditAccount->setDefaultText(tr("Enter your user ID or Email address"));
 	m_ui->lineEditPassword->setDefaultText(tr("Enter your password"));
 	m_ui->lineEditPassword->showAsPassword();

	m_ui->lineEditSearch->setDefaultText(tr("Search Contact"));

	m_contactsFilterMenu = new QMenu(this);
	m_contactsFilterMenu->addAction(m_ui->actionAll);
	m_contactsFilterMenu->addAction(m_ui->actionVoice_Easy_Users);
	m_contactsFilterMenu->addAction(m_ui->actionPhone_Numbers);
	m_ui->pushButtonContactsFilter->setMenu(m_contactsFilterMenu);
	m_ui->labelFilterDescription->setVisible(false);
	m_recentFilterMenu = new QMenu(this);
	m_recentFilterMenu->addAction(m_ui->actionRecentAll);
	m_recentFilterMenu->addAction(m_ui->actionRecentMissed);
	m_ui->pushButtonRecentFilter->setMenu(m_recentFilterMenu);

	QHeaderView* header = m_ui->treeWidgetContacts->header();
	m_ui->treeWidgetContacts->setColumnCount(ContactItemBase::MaxColumn);
	m_ui->treeWidgetContacts->setFocusPolicy(Qt::NoFocus);
	m_ui->treeWidgetContacts->setAttribute(Qt::WA_MacShowFocusRect, false);
	header->setResizeMode(ContactItemBase::PhotoColumn, QHeaderView::ResizeToContents);
	header->setResizeMode(ContactItemBase::StatusColumn, QHeaderView::Interactive);
	header->setResizeMode(ContactItemBase::LabelColumn, QHeaderView::Stretch);
	m_ui->treeWidgetContacts->setColumnWidth(ContactItemBase::StatusColumn, ContactItemBase::statusColumnWidth);

	header = m_ui->treeWidgetRecent->header();
	m_ui->treeWidgetRecent->setColumnCount(ItemBase::MaxColumn);
	m_ui->treeWidgetRecent->setFocusPolicy(Qt::NoFocus);
	m_ui->treeWidgetRecent->setAttribute(Qt::WA_MacShowFocusRect, false);
	header->setResizeMode(ItemBase::LabelColumn, QHeaderView::Stretch);
	header->setResizeMode(ItemBase::DirectionColumn, QHeaderView::ResizeToContents);

    // Tray icon
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(qApp->windowIcon());
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_ui->actionAbout_Voice_Easy);
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(m_ui->actionQuit);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip(windowTitle());

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(slotTrayIconActivated(QSystemTrayIcon::ActivationReason)));

	resize(QSize(760, 600));
    readSettings();
	setMinimumHeight(600);

	connect(qApp, SIGNAL(sessionTimeOut()), this, SLOT(slotUnLogin()));

	m_audioNavService = qApp->createAudioService(0);
	m_audioNavService->start();

#ifdef QPID_DTMF_ENABLE
	m_audioDtmfService = qApp->createAudioService(0);
	connect(m_audioDtmfService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotDtmfOutputStateChanged(QAudio::State)));
	m_audioDtmfService->start();
#endif

	m_audioNavDlg = new AudioNavigateDialog(this);
	if (m_audioNavDlg) {
		m_audioNavDlg->setAudioService(m_audioNavService);
	}

	m_movie = new QMovie(QLatin1String(":/images/ui/loading.gif"));
	m_messagePanel = new MessagePanel(m_ui->frameLoginMiddle);
//	m_messagePanel->setLabel(tr("Please waiting..."));
	m_messagePanel->setMovie(m_movie);

	connect(m_ui->phoneView, SIGNAL(callingMinimize(bool)), this, SLOT(slotCallingMinimize(bool)));
	connect(m_ui->widgetCallMinimize, SIGNAL(maximize()), this, SLOT(slotCallingMaximize()));

	m_languageActions = new QActionGroup(this);
	m_languageActions->addAction(m_ui->actionEnglish);
	m_languageActions->addAction(m_ui->actionChinese);
	m_ui->actionEnglish->setData(QString(QLatin1String("en_US")));
	m_ui->actionChinese->setData(QString(QLatin1String("zh_CN")));
	connect(m_languageActions, SIGNAL(triggered(QAction*)), this, SLOT(slotLanguageChanged(QAction*)));
	{
		/* Set the language menu item checked when startup */
		const QString languageCountry = qApp->language();
		if (m_ui->actionEnglish->data().toString() == languageCountry)
			m_ui->actionEnglish->setChecked(true);
		else if (m_ui->actionChinese->data().toString() == languageCountry)
			m_ui->actionChinese->setChecked(true);

		slotLanguageChanged(m_languageActions->checkedAction());
	}

	m_ui->menuChange_Language->removeAction(m_ui->actionChinese);

	connect(m_ui->treeWidgetContacts, SIGNAL(viewProfile(QTreeWidgetItem*)), this, SLOT(slotViewProfile(QTreeWidgetItem*)));
	connect(m_ui->treeWidgetContacts, SIGNAL(remove(QTreeWidgetItem*)), this, SLOT(slotRemoveContact(QTreeWidgetItem*)));
	connect(m_ui->treeWidgetRecent, SIGNAL(remove(QTreeWidgetItem*)), this, SLOT(slotRemoveHistory(QTreeWidgetItem*)));

}

PhoneMainWindow::~PhoneMainWindow()
{
	if (m_audioNavService) {
		m_audioNavService->stop();
	}

#ifdef QPID_DTMF_ENABLE
	if (m_audioDtmfService) {
		m_audioDtmfService->stopPlay();
		m_audioDtmfService->stop();
	}
#endif

	m_ui->phoneView->setCurrentCall(qApp->call());
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

		disconnect(m_userService, SIGNAL(loginFailed(const QString&, const QString&, bool)), this, SLOT(slotLoginFailed(const QString&, const QString&, bool)));
		disconnect(m_userService, SIGNAL(photoDownloaded(const QString&, const QString&)), this, SLOT(slotPhotoDownloaded(const QString&, const QString&)));
	}

	m_userService = userService;
	m_ui->phoneView->setUserService(m_userService);
	m_ui->widgetCallMinimize->setUserService(m_userService);

	if (m_userService) {
		connect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
		connect(m_userService, SIGNAL(inactive()), this, SLOT(slotUserServiceInactive()));

		connect(m_userService, SIGNAL(loginFailed(const QString&, const QString&, bool)), this, SLOT(slotLoginFailed(const QString&, const QString&, bool)));
		connect(m_userService, SIGNAL(photoDownloaded(const QString&, const QString&)), this, SLOT(slotPhotoDownloaded(const QString&, const QString&)));
	}
}

SipService* PhoneMainWindow::sipService() const
{
	return m_sipService;
}

void PhoneMainWindow::setSipService(SipService* sipService)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(sipLog(const QString&)), this, SLOT(slotSipLog(const QString&)));
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
		disconnect(m_sipService, SIGNAL(incommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)), 
			this, SLOT(slotSipIncommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)),
			this, SLOT(slotReceiveText(int, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(receiveDtmf(int,int)), this, SLOT(slotReceiveDtmf(int,int)));
		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));
		disconnect(m_sipService, SIGNAL(buddyStateChanged(int, int)), this, SLOT(slotBuddyStateChanged(int, int)));
	}

	m_sipService = sipService;
	m_ui->phoneView->setSipService(m_sipService);
	m_ui->widgetCallMinimize->setSipService(m_sipService);

//	m_sipService->streamWave(QLatin1String("d:/sleep.wav"));

	if (m_sipService) {
		connect(m_sipService, SIGNAL(sipLog(const QString&)), this, SLOT(slotSipLog(const QString&)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(incommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)), 
			this, SLOT(slotSipIncommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(receiveText(int, const QString&, const QString&)),
			this, SLOT(slotReceiveText(int, const QString&, const QString&)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(receiveDtmf(int,int)), this, SLOT(slotReceiveDtmf(int,int)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)), Qt::QueuedConnection);
		connect(m_sipService, SIGNAL(buddyStateChanged(int, int)), this, SLOT(slotBuddyStateChanged(int, int)), Qt::QueuedConnection);
	}
}

AudioService* PhoneMainWindow::audioService() const
{
	return m_audioService;
}

void PhoneMainWindow::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
		disconnect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotAudioOutputStateChanged(QAudio::State)));
	}

	m_audioService = audioService;

	if (m_audioService) {
		connect(m_audioService, SIGNAL(audioOutputStateChanged(QAudio::State)), this, SLOT(slotAudioOutputStateChanged(QAudio::State)));
	}
}

CommandService* PhoneMainWindow::commandService() const
{
	return m_commandService;
}

void PhoneMainWindow::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(phoneloadconfigFinished(Command::PhoneLoadConfigParam, Command::PhoneLoadConfigResult)), 
			this, SLOT(slotPhoneLoadconfigFinished(Command::PhoneLoadConfigParam, Command::PhoneLoadConfigResult)));
		disconnect(m_commandService, SIGNAL(phoneregisterFinished(Command::PhoneRegisterParam, Command::PhoneRegisterResult)), 
			this, SLOT(slotPhoneRegisterFinished(Command::PhoneRegisterParam, Command::PhoneRegisterResult)));
		disconnect(m_commandService, SIGNAL(phoneforgetpasswordFinished(Command::PhoneForgetPasswordParam, Command::PhoneForgetPasswordResult)), 
			this, SLOT(slotPhoneForgetPasswordFinished(Command::PhoneForgetPasswordParam, Command::PhoneForgetPasswordResult)));
		disconnect(m_commandService, SIGNAL(phonecontactlistFinished(Command::PhoneContactListParam, Command::PhoneContactListResult)), 
			this, SLOT(slotPhoneContactListFinished(Command::PhoneContactListParam, Command::PhoneContactListResult)));
		disconnect(m_commandService, SIGNAL(phoneaddcontactFinished(Command::PhoneAddContactParam, Command::PhoneAddContactResult)), 
			this, SLOT(slotPhoneAddContactFinished(Command::PhoneAddContactParam, Command::PhoneAddContactResult)));
		disconnect(m_commandService, SIGNAL(phoneupdatecontactFinished(Command::PhoneUpdateContactParam, Command::PhoneUpdateContactResult)), 
			this, SLOT(slotPhoneUpdateContactFinished(Command::PhoneUpdateContactParam, Command::PhoneUpdateContactResult)));
		disconnect(m_commandService, SIGNAL(phonedelcontactFinished(Command::PhoneDeleteContactParam, Command::PhoneDeleteContactResult)), 
			this, SLOT(slotPhoneDeleteContactFinished(Command::PhoneDeleteContactParam, Command::PhoneDeleteContactResult)));
		disconnect(m_commandService, SIGNAL(phonedetailcontactFinished(Command::PhoneDetailContactParam, Command::PhoneDetailContactResult)), 
			this, SLOT(slotPhoneDetailContactFinished(Command::PhoneDetailContactParam, Command::PhoneDetailContactResult)));
		disconnect(m_commandService, SIGNAL(phonegethistorycdrFinished(Command::PhoneGetHistoryCDRParam, Command::PhoneGetHistoryCDRResult)),
			this, SLOT(slotPhoneGetHistoryCDRFinished(Command::PhoneGetHistoryCDRParam, Command::PhoneGetHistoryCDRResult)));
// 		disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
// 			this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));
		disconnect(m_commandService, SIGNAL(phoneedituserinfoFinished(Command::PhoneEditUserInfoParam, Command::PhoneEditUserInfoResult)),
			this, SLOT(slotPhoneEditUserInfoFinished(Command::PhoneEditUserInfoParam, Command::PhoneEditUserInfoResult)));
		disconnect(m_commandService, SIGNAL(phonechangepasswordFinished(Command::PhoneChangePasswordParam, Command::PhoneChangePasswordResult)),
			this, SLOT(slotPhoneChangePasswordFinished(Command::PhoneChangePasswordParam, Command::PhoneChangePasswordResult)));
		disconnect(m_commandService, SIGNAL(phonenewpasswordFinished(Command::PhoneNewPasswordParam, Command::PhoneNewPasswordResult)),
			this, SLOT(slotPhoneNewPasswordFinished(Command::PhoneNewPasswordParam, Command::PhoneNewPasswordResult)));
		disconnect(m_commandService, SIGNAL(phonegetcdridFinished(Command::PhoneGetCDRIDParam, Command::PhoneGetCDRIDResult)),
			this, SLOT(slotPhoneGetCDRIDFinished(Command::PhoneGetCDRIDParam, Command::PhoneGetCDRIDResult)));
		disconnect(m_commandService, SIGNAL(phonedelcdrFinished(Command::PhoneDeleteCDRParam, Command::PhoneDeleteCDRResult)),
			this, SLOT(slotPhoneDeleteCDRFinished(Command::PhoneDeleteCDRParam, Command::PhoneDeleteCDRResult)));

	}

	m_commandService = commandService;
	m_ui->phoneView->setCommandService(m_commandService);

	if (m_commandService) {
		connect(m_commandService, SIGNAL(phoneloadconfigFinished(Command::PhoneLoadConfigParam, Command::PhoneLoadConfigResult)), 
			this, SLOT(slotPhoneLoadconfigFinished(Command::PhoneLoadConfigParam, Command::PhoneLoadConfigResult)));
		connect(m_commandService, SIGNAL(phoneregisterFinished(Command::PhoneRegisterParam, Command::PhoneRegisterResult)), 
			this, SLOT(slotPhoneRegisterFinished(Command::PhoneRegisterParam, Command::PhoneRegisterResult)));
		connect(m_commandService, SIGNAL(phoneforgetpasswordFinished(Command::PhoneForgetPasswordParam, Command::PhoneForgetPasswordResult)), 
			this, SLOT(slotPhoneForgetPasswordFinished(Command::PhoneForgetPasswordParam, Command::PhoneForgetPasswordResult)));
		connect(m_commandService, SIGNAL(phonecontactlistFinished(Command::PhoneContactListParam, Command::PhoneContactListResult)), 
			this, SLOT(slotPhoneContactListFinished(Command::PhoneContactListParam, Command::PhoneContactListResult)));
		connect(m_commandService, SIGNAL(phoneaddcontactFinished(Command::PhoneAddContactParam, Command::PhoneAddContactResult)), 
			this, SLOT(slotPhoneAddContactFinished(Command::PhoneAddContactParam, Command::PhoneAddContactResult)));
		connect(m_commandService, SIGNAL(phoneupdatecontactFinished(Command::PhoneUpdateContactParam, Command::PhoneUpdateContactResult)), 
			this, SLOT(slotPhoneUpdateContactFinished(Command::PhoneUpdateContactParam, Command::PhoneUpdateContactResult)));
		connect(m_commandService, SIGNAL(phonedelcontactFinished(Command::PhoneDeleteContactParam, Command::PhoneDeleteContactResult)), 
			this, SLOT(slotPhoneDeleteContactFinished(Command::PhoneDeleteContactParam, Command::PhoneDeleteContactResult)));
		connect(m_commandService, SIGNAL(phonedetailcontactFinished(Command::PhoneDetailContactParam, Command::PhoneDetailContactResult)), 
			this, SLOT(slotPhoneDetailContactFinished(Command::PhoneDetailContactParam, Command::PhoneDetailContactResult)));
		connect(m_commandService, SIGNAL(phonegethistorycdrFinished(Command::PhoneGetHistoryCDRParam, Command::PhoneGetHistoryCDRResult)),
			this, SLOT(slotPhoneGetHistoryCDRFinished(Command::PhoneGetHistoryCDRParam, Command::PhoneGetHistoryCDRResult)));
// 		connect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)),
// 			this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));
		connect(m_commandService, SIGNAL(phoneedituserinfoFinished(Command::PhoneEditUserInfoParam, Command::PhoneEditUserInfoResult)),
			this, SLOT(slotPhoneEditUserInfoFinished(Command::PhoneEditUserInfoParam, Command::PhoneEditUserInfoResult)));
		connect(m_commandService, SIGNAL(phonechangepasswordFinished(Command::PhoneChangePasswordParam, Command::PhoneChangePasswordResult)),
			this, SLOT(slotPhoneChangePasswordFinished(Command::PhoneChangePasswordParam, Command::PhoneChangePasswordResult)));
		connect(m_commandService, SIGNAL(phonenewpasswordFinished(Command::PhoneNewPasswordParam, Command::PhoneNewPasswordResult)),
			this, SLOT(slotPhoneNewPasswordFinished(Command::PhoneNewPasswordParam, Command::PhoneNewPasswordResult)));
		connect(m_commandService, SIGNAL(phonegetcdridFinished(Command::PhoneGetCDRIDParam, Command::PhoneGetCDRIDResult)),
			this, SLOT(slotPhoneGetCDRIDFinished(Command::PhoneGetCDRIDParam, Command::PhoneGetCDRIDResult)));
		connect(m_commandService, SIGNAL(phonedelcdrFinished(Command::PhoneDeleteCDRParam, Command::PhoneDeleteCDRResult)),
			this, SLOT(slotPhoneDeleteCDRFinished(Command::PhoneDeleteCDRParam, Command::PhoneDeleteCDRResult)));
	}
}

MediaService* PhoneMainWindow::mediaService() const
{
	return m_mediaService;
}

void PhoneMainWindow::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
		disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotUserPhotoDownloaded(const QString&, const QString&)));
		disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotUserPhotoDownloadFailed(const QString&, const QString&)));
	}

	m_mediaService = mediaService;
	m_ui->phoneView->setMediaService(m_mediaService);

	if (m_mediaService) {
		connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotUserPhotoDownloaded(const QString&, const QString&)));
		connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)),
			this, SLOT(slotUserPhotoDownloadFailed(const QString&, const QString&)));
	}
}

void PhoneMainWindow::setPreference(Preference* pref)
{
    PreferenceSupport::setPreference(pref);
    typedef PhoneMainWindowOptions Opt;

}

void PhoneMainWindow::setLoginDialog(LoginDialog *l)
{
	m_l = l;
}

QString PhoneMainWindow::userName() const
{
	QString r;
	r = m_ui->lineEditAccount->text();
	r.trimmed();
	return r;
}

void PhoneMainWindow::setUserName(const QString& userName)
{
	m_ui->lineEditAccount->inputText(userName);
}

QString PhoneMainWindow::password() const
{
	return m_ui->lineEditPassword->text();
}

void PhoneMainWindow::setPassword(const QString& password)
{
	m_ui->lineEditPassword->inputText(password);
}

void PhoneMainWindow::serverSetting()
{
#ifdef QPID_PHONE_DYNAMIC_SERVER
	SettingsDialog dlg(this);
	dlg.exec();
#endif
}

void PhoneMainWindow::phoneLoadConfig()
{
	Q_ASSERT(m_commandService && "Call PhoneMainWindow::setCommandService() first!");

	QDateTime t = QDateTime::currentDateTime();
	Command::PhoneLoadConfigParam param;
	param.setLastTime(t.toString(QLatin1String("yyyy-MM-dd")));
	param.setReloadConfig(1);
	m_commandService->phoneloadconfig(param);
}

void PhoneMainWindow::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef PhoneMainWindowOptions Opt;
    Preference* pref = preference();

	if(pref) {
		m_userService->setServiceHost(pref->getString(Opt::host_));

		m_ui->checkBoxAutoSignin->setChecked( (pref->getString(Opt::remember_).toInt()==1)?true:false );
		if (m_ui->checkBoxAutoSignin->isChecked()) {
			SimpleCrypt crypto(Q_UINT64_C(0xafd5d610aa473c39));
			m_ui->lineEditAccount->inputText(pref->getString(Opt::account_));
			m_ui->lineEditPassword->inputText(crypto.decryptToString(pref->getString(Opt::password_)));
		}
	}
}
bool PhoneMainWindow::event(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
//        adjustSystemButtons();
    }

    return MainWindow::event(event);
}

void PhoneMainWindow::changeEvent(QEvent* event)
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
			retranslateTr();
            break;
        default:
            break;
    }
}

void PhoneMainWindow::closeEvent(QCloseEvent* event)
{
	typedef PhoneMainWindowOptions Opt;

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

void PhoneMainWindow::resizeEvent(QResizeEvent* event)
{
    MainWindow::resizeEvent(event);
}

void PhoneMainWindow::keyPressEvent(QKeyEvent *event)
{
	MainWindow::keyPressEvent(event);

	if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {
		if (m_ui->stackedWidget->currentWidget() == m_ui->pageLogin) {
			on_pushButtonSignin_clicked();
		}
	}
}

void PhoneMainWindow::showMainWindow()
{
#ifdef QPID_ENABLE_SINGLE_APPLICATION
    qApp->setActivationWindow(this);
#endif // QPID_ENABLE_SINGLE_APPLICATION

	typedef PhoneMainWindowOptions Opt;
	Preference* pref = preference();

	SimpleCrypt crypto(Q_UINT64_C(0xafd5d610aa473c39));
	pref->putString(Opt::account_, m_ui->lineEditAccount->text().trimmed());
	pref->putString(Opt::password_, crypto.encryptToString(m_ui->lineEditPassword->text().trimmed().toLocal8Bit()));
	pref->putString(Opt::remember_, m_ui->checkBoxAutoSignin->isChecked()?QLatin1String("1"):QLatin1String("0"));

	m_ui->stackedWidget->setCurrentWidget(m_ui->pageMain);

	m_contactType = ContactTypeAll;
	updateContactTypeAction();
	m_recentType = RecentTypeAll;
	updateRecentTypeAction();

    show();
	qApp->setQuitOnLastWindowClosed(false);
    m_trayIcon->show();

	const User::PhoneHistoryList historys = m_userService->findPhoneHistory();
	foreach (const User::PhoneHistory& h, historys) {
		PhoneHistoryItem *newItem = new PhoneHistoryItem(m_ui->treeWidgetRecent, h);
		newItem->setUserService(m_userService);
		m_phoneHistoryItems.insert(h.meetCdrId(), newItem);
	}

	m_userService->clearPhoneContact();
	/* Add live interpretor */
	User::Contact contact;
	contact.setAddBookId(QLatin1String("-1"));
	contact.setContactName(tr("Live interpreter"));
	contact.setCid(QLatin1String("-1"));
	contact.setSmallPhoto(QString());
	contact.setLargePhoto(QString());
	contact.setPhone(QString());
	contact.setVoiceraId(QLatin1String("Live interpreter"));
	contact.setSipCallerId(QString());
	contact.setSourceLanguage(QString());
	contact.setTargetLanguage(QString());
	contact.setOnlineStatus(QLatin1String("1"));
	ContactItem *newItem = new ContactItem(m_ui->treeWidgetContacts, contact);
	m_contactItems.insert(contact.addBookId(), newItem);

	/* Register to the sip server*/
	if (m_sipService && m_userService) {
		QString sipServer = m_userService->userServiceHostNoPort();
		m_sipAccountID = 1;//m_sipService->registerServer(m_userService->sipAccount(), m_userService->password(), sipServer);
		qApp->setSIPAccountID(m_sipAccountID);
	}

	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)), 
			this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

		Command::PhoneContactListParam param;
		param.setSearchType(QLatin1String("01"));
		m_commandService->phonecontactlist(param);

		//Get Signed in user information
		{
			if (m_mediaService) {
				connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
					this, SLOT(slotUserPhotoDownloaded(const QString&, const QString&)));
				connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
					this, SLOT(slotUserPhotoDownloadFailed(const QString&, const QString&)));
			}
			connect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)), 
				this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

			Command::PhoneGetUserInfoParam param;
//			param.setVoiceraID(m_userService->userName());
			m_commandService->phonegetuserinfo(param);
		}
	}

	m_ui->phoneView->signIn();
	m_ui->phoneView->setPSTNSupported(qApp->PSTNSupported());
}

void PhoneMainWindow::slotUnLogin()
{
	Q_ASSERT(m_l);

	if (m_l->isVisible()) {
		return;
	}

	stopPhoneView();

	hide();
// 	if (m_l) {
// 		m_l->show();
// 		if (m_l->isSavePassword())
// 			m_l->login();
// 	}
}

void PhoneMainWindow::retranslateTr()
{
	m_ui->lineEditAccount->setDefaultText(tr("Enter your user ID or Email address"));
	m_ui->lineEditPassword->setDefaultText(tr("Enter your password"));
	m_ui->lineEditSearch->setDefaultText(tr("Search Contact"));
	if (m_userService) {
//		m_ui->lineEditCredit->setText(tr("VE Credit: %1").arg(m_userService->balance()));
	}
}

void PhoneMainWindow::finishLogin(bool succeeded)
{
	if (m_userService) {
		disconnect(m_userService, SIGNAL(stopped()), this, SLOT(slotUserServiceStopped()));
	}

	if (succeeded) {
		QTimer::singleShot(2000, this, SLOT(showMainWindow()));
	}
}

void PhoneMainWindow::stopPhoneView()
{
	typedef PhoneMainWindowOptions Opt;

	if (qApp->call()->callID() != -1)
		m_sipService->hangupCall(qApp->call()->callID());

	// XXX Stop UserService now
	qApp->userService()->stop();

	writeSettings();
}

void PhoneMainWindow::doContactFilter(const QString& text)
{
	QList<ContactItem*> contactItemList = m_contactItems.values();
	m_ui->treeWidgetContacts->setUpdatesEnabled(false);
	foreach (ContactItem* contactItem, contactItemList) {
		Q_ASSERT(contactItem);
		bool hidden = false;
		QString label = contactItem->data(ContactItemBase::LabelColumn, Qt::DisplayRole).toString();
		hidden = !label.contains(text, Qt::CaseInsensitive);
		if (m_contactType==ContactTypeVoiceEasy && contactItem->contact().voiceraId().isEmpty())
			hidden = true;
		else if (m_contactType==ContactTypeNumber && !contactItem->contact().voiceraId().isEmpty())
			hidden = true;
		/* Always show live interpreter */
		if (contactItem->contact().addBookId()==QLatin1String("-1"))
			hidden = false;
		contactItem->setHidden(hidden);
	}
	m_ui->treeWidgetContacts->setUpdatesEnabled(true);
}

void PhoneMainWindow::doPhoneHistoryFilter(const QString& text)
{
	QList<PhoneHistoryItem*> phoneHistoryItemList = m_phoneHistoryItems.values();
	m_ui->treeWidgetRecent->setUpdatesEnabled(false);
	foreach (PhoneHistoryItem* phoneHistoryItem, phoneHistoryItemList) {
		Q_ASSERT(phoneHistoryItem);
		bool hidden = false;
		QString label = phoneHistoryItem->data(ItemBase::LabelColumn, Qt::DisplayRole).toString();
		label = label.split(QLatin1String("\n")).at(0);
		hidden = !label.contains(text, Qt::CaseInsensitive);
		if (m_recentType==RecentTypeMissed && !phoneHistoryItem->isCallMissed())
			hidden = true;
		phoneHistoryItem->setHidden(hidden);
	}
	m_ui->treeWidgetRecent->setUpdatesEnabled(true);
}

void PhoneMainWindow::updateContactTypeAction()
{
	m_ui->actionAll->setChecked(m_contactType==ContactTypeAll);
	m_ui->actionVoice_Easy_Users->setChecked(m_contactType==ContactTypeVoiceEasy);
	m_ui->actionPhone_Numbers->setChecked(m_contactType==ContactTypeNumber);

	if (m_contactType==ContactTypeAll)
		m_ui->pushButtonContactsFilter->setText(m_ui->actionAll->text());
	else if(m_contactType==ContactTypeVoiceEasy)
		m_ui->pushButtonContactsFilter->setText(m_ui->actionVoice_Easy_Users->text());
	else
		m_ui->pushButtonContactsFilter->setText(m_ui->actionPhone_Numbers->text());

	m_ui->frameContactFilter->update();
}

void PhoneMainWindow::updateRecentTypeAction()
{
	m_ui->actionRecentAll->setChecked(m_recentType==RecentTypeAll);
	m_ui->actionRecentMissed->setChecked(m_recentType==RecentTypeMissed);

	if (m_recentType==RecentTypeAll)
		m_ui->pushButtonRecentFilter->setText(m_ui->actionRecentAll->text());
	else
		m_ui->pushButtonRecentFilter->setText(m_ui->actionRecentMissed->text());

	m_ui->frameRecentFilter->update();
}

void PhoneMainWindow::disconnectPhotoSignal()
{
	disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
		this, SLOT(slotUserPhotoDownloaded(const QString&, const QString&)));
	disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
		this, SLOT(slotUserPhotoDownloadFailed(const QString&, const QString&)));
}

void PhoneMainWindow::slotUserServiceStopped()
{
	finishLogin(false);
}

void PhoneMainWindow::slotUserServiceActive()
{
	finishLogin(true);

    m_trayIcon->setIcon(windowIcon());
}

void PhoneMainWindow::slotUserServiceInactive()
{
    // TODO The tray icon' icon will be changed
    m_trayIcon->setIcon(QIcon(windowIcon().pixmap(QSize(16, 16), QIcon::Disabled)));
}

void PhoneMainWindow::slotLoginFailed(const QString& error, const QString& errorMsg, bool command)
{
	m_ui->pushButtonSignin->setEnabled(true);
	m_ui->lineEditAccount->setReadOnly(false);
	m_ui->lineEditPassword->setReadOnly(false);
	m_messagePanel->setVisible(false);

	if (command) {
		m_ui->labelMessage->setText(errorMsg);
	}else {
		m_ui->labelMessage->setText(phoneErrorMsg(errorMsg));
	}
	m_ui->labelMessage->show();
}

void PhoneMainWindow::slotSipLog(const QString& msg)
{
//	qDebug() << QPID_DEBUG_PREFIX << msg;
}

void PhoneMainWindow::slotRegState(int account, int code, const QString& reason)
{
	m_ui->pushButtonSignin->setEnabled(true);
	m_ui->lineEditAccount->setReadOnly(false);
	m_ui->lineEditPassword->setReadOnly(false);
	m_messagePanel->setVisible(false);

	switch(code) {
		case 200: /* 200/OK*/
//			m_ui->labelStatusCaption->setText(tr("Waiting for calls"));
			break;
		default:
			qDebug() << QPID_DEBUG_PREFIX << code << reason;
//			m_ui->labelStatusCaption->setText(tr("Call Unavailable"));
			break;
	}
}

void PhoneMainWindow::slotSipIncommingCall(int account, int callId, const QString& srcLang, const QString& dstLang, 
	const QString& mode, const QString& serviceMode, const QString& remote)
{
	qDebug() << account << callId << srcLang << dstLang << mode << serviceMode << remote;

	/* remote eg: "leegoex" <sip:5450@58.64.141.118> */	
	QRegExp exp(QLatin1String("sip:(\\d+)[^@]"));
	exp.indexIn(remote, 0);
	QString extensionNumber = exp.cap(0).remove(QLatin1String("sip:"));
	QString callerName = remote.section(QRegExp(QLatin1String("[\"]")), 1, 1);
	QString tempSrcLang = qApp->languageFull(srcLang.toUpper());
	QString tempDstLang = qApp->languageFull(dstLang.toUpper());
	QString tempSeviceText = isFreeCall(serviceMode)?tr("Free"):tr("Premium");

	if (m_callSplash!=NULL) {
		disconnect(m_callSplash, SIGNAL(answer()), this, SLOT(slotSplashAnswer()));
		disconnect(m_callSplash, SIGNAL(decline()), this, SLOT(slotSplashDecline()));
		delete m_callSplash;
	}
	m_callSplash = new CallSplash(QApplication::desktop());
	connect(m_callSplash, SIGNAL(answer()), this, SLOT(slotSplashAnswer()));
	connect(m_callSplash, SIGNAL(decline()), this, SLOT(slotSplashDecline()));
	
	QString photoFile;
	User::Contact contact = m_userService->findPhoneContactByNumber(extensionNumber);
	if (contact.isValid() && !contact.photofile().isEmpty()) {
		photoFile = contact.photofile();
	} else {
		photoFile = QLatin1String(":/images/ui/default_photo.jpg");
	}
	m_callSplash->setCallDetails(callId, callerName, tempSrcLang, tempDstLang, tempSeviceText, photoFile);
	m_callSplash->slowShow();

	m_audioService->startPlay(qApp->tone(QLatin1String("ringing")));
	m_sipService->ringringCall(callId);

	qApp->call()->setCallID(callId);
	qApp->call()->setCallerNumber(extensionNumber);
	qApp->call()->setCallerName(callerName);
	qApp->call()->setCallState(Call::CallingIn);
	m_ui->phoneView->callIncomming();
	m_ui->phoneView->showCallingWindow();
}

void PhoneMainWindow::slotSipCallState(int callId, int state)
{
	qDebug() << QPID_DEBUG_PREFIX << callId << state;
	switch(state) {
		case 1: /*PJSIP_INV_STATE_CALLING*/
			qApp->call()->setCallState(Call::CallingOut);
			m_audioService->startPlay(qApp->tone(QLatin1String("ringback")));
			m_ui->widgetCallMinimize->callOut();
			m_ui->phoneView->callOut();
			m_ui->phoneView->showCallingWindow();
			break;
		case 4: /* PJSIP_INV_STATE_CONNECTING */
			break;
		case 5: /* PJSIP_INV_STATE_CONFIRMED */
			m_ui->widgetCallMinimize->callConfirmed();
			m_ui->phoneView->callConfirmed();
			if (m_audioService)
				m_audioService->stopPlay();
			if (m_callSplash)
				m_callSplash->slowHide();
			qApp->call()->setCallState(Call::CallConfirmed);
			break;
		case 6: /* PJSIP_INV_STATE_DISCONNECTED */
			m_ui->widgetCallMinimize->callClosed();
			m_ui->phoneView->callClosed();
			m_ui->phoneView->closeCallingWindow();
			m_ui->widgetCallMinimize->hide();
			if (m_audioService)
				m_audioService->stopPlay();
			if (m_callSplash)
				m_callSplash->slowHide();
			qApp->call()->setCallState(Call::CallClosed);
			QTimer::singleShot(1000, this, SLOT(slotDelayCDR()));
			break;
	}
}

void PhoneMainWindow::slotReceiveText(int callId, const QString& remote, const QString& text)
{
	if (text==QLatin1String("*#01#")) {
		qApp->call()->setCallState(Call::CallingInterpreter);
	} else if(text==QLatin1String("*#02#")) {
		qApp->call()->setCallState(Call::CallingCallee);
	} else {
		qApp->call()->setCallState(Call::CallConnected);
	}

	m_ui->widgetCallMinimize->callConnected(text);
	m_ui->phoneView->callConnected(text);
}

void PhoneMainWindow::slotReceiveDtmf(int callId, int dtmf)
{
}

void PhoneMainWindow::slotBuddyStateChanged(int buddyID, int state)
{
	QList<ContactItem*> contactItemList = m_contactItems.values();
	foreach (ContactItem* contactItem, contactItemList) {
		if (contactItem && contactItem->buddyID() == buddyID) {
			contactItem->setOnlineStatus(QString::number(state));
			break;
		}
	}
}

void PhoneMainWindow::slotAudioOutputStateChanged(QAudio::State state)
{
	switch(state)
	{
	case QAudio::ActiveState:
		break;
	case QAudio::SuspendedState:
		break;
	case QAudio::StoppedState:
		break;
	case QAudio::IdleState:

		qDebug() << QPID_DEBUG_PREFIX << qApp->call()->state();

		if (m_audioService && qApp->call()->state()==Call::CallingIn) {
			m_audioService->startPlay(qApp->tone(QLatin1String("ringing")));
		}
		if (m_audioService && qApp->call()->state()==Call::CallingOut) {
			m_audioService->startPlay(qApp->tone(QLatin1String("ringback")));
		}
		break;
	}
}

#ifdef QPID_DTMF_ENABLE
void PhoneMainWindow::slotDtmfOutputStateChanged(QAudio::State state)
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
#endif

void PhoneMainWindow::slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
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

void PhoneMainWindow::on_actionSign_Out_triggered()
{
	m_ui->stackedWidgetViews->setCurrentWidget(m_ui->pageContacts);
	m_ui->phoneView->signOut();

	disconnectPhotoSignal();
	disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)), 
		this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

	m_ui->labelPhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
	m_ui->lineEditSearch->setText(QString());
	m_contactType = ContactTypeAll;
	updateContactTypeAction();
	m_recentType = RecentTypeAll;
	updateRecentTypeAction();

	if (!m_contactItems.isEmpty()) {
		QList<ContactItem*> contactItemList = m_contactItems.values();
		foreach (ContactItem* contactItem, contactItemList) {
			if (m_sipService)
				m_sipService->buddySubscribeCancel(contactItem->buddyID());
			delete contactItem;
		}
		m_contactItems.clear();
	}

	if (!m_phoneHistoryItems.isEmpty()) {
		QList<PhoneHistoryItem*> phoneHistoryItemList = m_phoneHistoryItems.values();
		foreach (PhoneHistoryItem* phoneHistoryItem, phoneHistoryItemList) {
			delete phoneHistoryItem;
		}
		m_phoneHistoryItems.clear();
	}
	
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageLogin);
	on_pushButtonContacts_clicked();

	int SIPAccountID = qApp->SIPAccountID();
	if (m_sipService && SIPAccountID>=0) {
		m_sipService->unregisterServer(SIPAccountID);
	}

	if (m_userService) {
		m_userService->stop();
	}
}

void PhoneMainWindow::on_actionClose_triggered()
{
	showMinimized();
}

void PhoneMainWindow::on_actionAll_triggered()
{
	m_contactType = ContactTypeAll;
	updateContactTypeAction();
	doContactFilter(QString());
}

void PhoneMainWindow::on_actionVoice_Easy_Users_triggered()
{
	m_contactType = ContactTypeVoiceEasy;
	updateContactTypeAction();
	doContactFilter(QString());
}

void PhoneMainWindow::on_actionPhone_Numbers_triggered()
{
	m_contactType = ContactTypeNumber;
	updateContactTypeAction();
	doContactFilter(QString());
}

void PhoneMainWindow::on_actionRecentAll_triggered()
{
	m_recentType = RecentTypeAll;
	updateRecentTypeAction();
	doPhoneHistoryFilter(QString());	
}

void PhoneMainWindow::on_actionRecentMissed_triggered()
{
	m_recentType = RecentTypeMissed;
	updateRecentTypeAction();
	doPhoneHistoryFilter(QString());	
}

void PhoneMainWindow::on_actionUpdate_triggered()
{
	UpdateDialog up2date(this);
	up2date.setCommandService(m_commandService);
	up2date.setMediaService(m_mediaService);
	up2date.startUpdate();
	up2date.exec();
}

void PhoneMainWindow::on_actionAbout_Voice_Easy_triggered()
{
	AboutDialog aboutDlg;
	aboutDlg.exec();
}

void PhoneMainWindow::on_actionQuit_triggered()
{
	slotQuitApplication();
}

void PhoneMainWindow::on_pushButtonSignin_clicked()
{
	typedef PhoneMainWindowOptions Opt;
	Preference* pref = preference();

	m_ui->labelMessage->hide();

	if (userName().isEmpty()) {
		qApp->warning(tr("Please enter your login email or user ID."));
		m_ui->lineEditAccount->setFocus();
		return;
	}

	if (password().isEmpty()) {
		qApp->warning(tr("Please enter your password."));
		m_ui->lineEditPassword->setFocus();
		return;
	}

	if (m_userService) {
		connect(m_userService, SIGNAL(stopped()), this, SLOT(slotUserServiceStopped()));
	}

	if (m_userService && pref) {
		m_ui->pushButtonSignin->setEnabled(false);
		m_ui->lineEditAccount->setReadOnly(true);
		m_ui->lineEditPassword->setReadOnly(true);
		m_messagePanel->setVisible(true);
		m_userService->setServiceHost(pref->getString(Opt::host_));
		m_userService->setUserName(userName());
		m_userService->setPassword(password());
		m_userService->start();
	}
}

void PhoneMainWindow::on_pushButtonCreateAccount_clicked()
{
	typedef PhoneMainWindowOptions Opt;
	Preference* pref = preference();

	QString tempUrl = m_createAccountUrl;
	tempUrl.prepend(pref->getString(Opt::host_));
	tempUrl.prepend(QLatin1String("http://"));
	QUrl url;
	url.setUrl(tempUrl);

	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_pushButtonForgetPassword_clicked()
{
	typedef PhoneMainWindowOptions Opt;
	Preference* pref = preference();

	QString tempUrl = m_changePasswordUrl;
	tempUrl.prepend(pref->getString(Opt::host_));
	tempUrl.prepend(QLatin1String("http://"));
	QUrl url;
	url.setUrl(tempUrl);

	QDesktopServices::openUrl(url);
}

void PhoneMainWindow::on_pushButtonAddCredit_clicked()
{
	qDebug() << QPID_DEBUG_PREFIX << "Add Credit clicked";
}

void PhoneMainWindow::on_pushButtonContacts_clicked()
{
	m_ui->pushButtonContacts->setChecked(true);
	m_ui->pushButtonRecent->setChecked(false);
	m_ui->stackedWidgetViews->setCurrentWidget(m_ui->pageContacts);
	doPhoneHistoryFilter(QString());
}

void PhoneMainWindow::on_pushButtonRecent_clicked()
{
	m_ui->pushButtonContacts->setChecked(false);
	m_ui->pushButtonRecent->setChecked(true);
	m_ui->stackedWidgetViews->setCurrentWidget(m_ui->pageRecent);
	doContactFilter(QString());
}

void PhoneMainWindow::on_treeWidgetContacts_itemClicked (QTreeWidgetItem * item, int column)
{
	if (item && item->type() == ContactItemBase::ContactItemType) {
        ContactItem* contactItem = static_cast<ContactItem*>(item);
		if (contactItem && contactItem->contact().addBookId()!=QLatin1String("-1")) {
			Command::PhoneDetailContactParam param;
			param.setAddBookID(contactItem->contact().addBookId());
			m_commandService->phonedetailcontact(param);
		} else {
			//Live interpreter
			m_ui->phoneView->showNeedInterpreterWindow();
		}
	}
}

void PhoneMainWindow::on_treeWidgetRecent_itemClicked(QTreeWidgetItem * item, int column)
{
	if (item && item->type() == ItemBase::PhoneHistoryItemType) {
		PhoneHistoryItem* historyItem = static_cast<PhoneHistoryItem*>(item);
		User::Contact contact = m_userService->findPhoneContactByNumber(historyItem->phoneHistory().callerId());
		m_ui->phoneView->showHistoryWindow(contact, historyItem->phoneHistory());
	}
}

void PhoneMainWindow::on_lineEditSearch_textChanged(const QString& text)
{
	const QString filter = m_ui->lineEditSearch->text();
	if (m_ui->stackedWidgetViews->currentWidget() == m_ui->pageContacts) {
		doContactFilter(filter);
	} else {
		doPhoneHistoryFilter(filter);
	}
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

void PhoneMainWindow::slotPhoneLoadconfigFinished(const Command::PhoneLoadConfigParam& param, const Command::PhoneLoadConfigResult& result)
{
	qApp->clearSupportedService();
	qApp->clearMajor();

	if (result.callMode().isValid() && result.callMode().canConvert<QVariantMap>()) {
		QVariantMap map = result.callMode().toMap();
		QMapIterator<QString, QVariant> i(map);
		while (i.hasNext()) {
			i.next();
			qDebug() << QPID_DEBUG_PREFIX << i.key() << i.value().toStringList();
		}
	}
	if (result.serviceLanguage().isValid() && result.serviceLanguage().canConvert<QVariantMap>()) {
		QVariantMap map = result.serviceLanguage().toMap();
		QMapIterator<QString, QVariant> i(map);
		while (i.hasNext()) {
			i.next();
			qApp->addSupportedService(i.key(), i.value().toStringList());
		}
	}
	if (result.majorArray().isValid() && result.majorArray().canConvert<QVariantMap>()) {
		QVariantMap map = result.majorArray().toMap();
		QMapIterator<QString, QVariant> i(map);
		while (i.hasNext()) {
			i.next();
			qApp->addMajor(i.key(), i.value().toString());
		}
	}
	if (result.useApp().isValid() && result.useApp().canConvert<QVariantMap>()) {
		QVariantMap map = result.useApp().toMap();
		QMapIterator<QString, QVariant> i(map);
		while (i.hasNext()) {
			i.next();
			qDebug() << QPID_DEBUG_PREFIX << i.key() << i.value().toString();
		}
	}

	QString m_latestVersion = result.version();
	QString m_latestUrl = result.downloadUrl();
	QString m_latestForce = result.forceUpdate();
	m_latestUrl.prepend(m_userService->userServiceHost());
	m_latestUrl.prepend(QLatin1String("http://"));

	qDebug() << QPID_DEBUG_PREFIX << result.verifyCode();
	qDebug() << QPID_DEBUG_PREFIX << result.versionName();

	m_createAccountUrl = result.createAccountUrl();
	m_changePasswordUrl = result.changePasswordUrl();
	m_editUserInfoUrl = result.editUserInfoUrl();

	qApp->setPSTNSupported(result.PSTNSupported());

	UpdateDialog up2date(this);
	up2date.setCommandService(m_commandService);
	up2date.setMediaService(m_mediaService);
	if (up2date.startUpdate(m_latestVersion, m_latestUrl, m_latestForce))
		up2date.exec();
}

void PhoneMainWindow::slotPhoneRegisterFinished(const Command::PhoneRegisterParam& param, const Command::PhoneRegisterResult& result)
{
	qDebug() << QPID_DEBUG_PREFIX << result.error();
	qDebug() << QPID_DEBUG_PREFIX << result.errorMsg();

	qDebug() << QPID_DEBUG_PREFIX << result.SIPCallerID();
	qDebug() << QPID_DEBUG_PREFIX << result.nickname();
	qDebug() << QPID_DEBUG_PREFIX << result.voiceraID();
	qDebug() << QPID_DEBUG_PREFIX << result.nativeLanguage();
	qDebug() << QPID_DEBUG_PREFIX << result.smallPhoto();
	qDebug() << QPID_DEBUG_PREFIX << result.largePhoto();
}

void PhoneMainWindow::slotPhoneForgetPasswordFinished(const Command::PhoneForgetPasswordParam& param, const Command::PhoneForgetPasswordResult& result)
{
	qDebug() << QPID_DEBUG_PREFIX << result.error();
	qDebug() << QPID_DEBUG_PREFIX << result.errorMsg();
}

void PhoneMainWindow::slotPhoneContactListFinished(const Command::PhoneContactListParam& param, const Command::PhoneContactListResult& result)
{
	User::Contact contact;
	contact.setUserName(m_userService->userName());
	contact.setCid(result.ID());
	contact.setAddBookId(result.addBookID());
	contact.setContactName(result.contactName());
	contact.setSmallPhoto(result.smallPhoto());
	contact.setLargePhoto(result.largePhoto());
	contact.setSipCallerId(result.SIPCallerID());
	QStringList phoneList;
	QVariant phone = result.phone();
	if (phone.isValid() && phone.canConvert<QVariantList>()) {
		foreach (const QVariant& m, phone.toList()) {
			QVariantMap details = m.toMap();
			phoneList << (details.value(QLatin1String("country_short")).toString() + QLatin1String("|") + details.value(QLatin1String("mobile")).toString());
		}
	}
	contact.setPhone(phoneList.join(QLatin1String(",")));
	contact.setVoiceraId(result.voiceraID());
	contact.setSourceLanguage(result.sourceLanguage());
	contact.setTargetLanguage(result.targetLanguage());
	contact.setOnlineStatus(result.onlineStatus());
	contact.setSystemUser(result.systemUser());
	if (m_userService) {
		m_userService->addPhoneContact(contact);
	}

	ContactItem *newItem = new ContactItem(m_ui->treeWidgetContacts, contact);
	m_contactItems.insert(contact.addBookId(), newItem);

	/* Buddy presence subscribe */
	if (m_sipService && !contact.sipCallerId().isEmpty()) {
		QString buddyURI = QString(QLatin1String("sip:%1@%2")).arg(contact.sipCallerId(), m_userService->userServiceHostNoPort());
		int buddyID = m_sipService->buddySubscribe(buddyURI);
		newItem->setBuddyID(buddyID);
	}
}

void PhoneMainWindow::slotPhoneAddContactFinished(const Command::PhoneAddContactParam& param, const Command::PhoneAddContactResult& result)
{
	if (result.error() != QLatin1String("0")) {
		qApp->warning(phoneErrorMsg(result.errorMsg()));
		return;
	}

	User::Contact contact;
	contact.setUserName(m_userService->userName());
	contact.setCid(result.ID());
	contact.setAddBookId(result.addBookID());
	m_ui->phoneView->getContactDetail(contact);
	if (m_userService) {
		qDebug() << contact.smallPhoto();
		m_userService->addPhoneContact(contact);
	}

	ContactItem *newItem = new ContactItem(m_ui->treeWidgetContacts, contact);
	m_contactItems.insert(contact.addBookId(), newItem);
	m_ui->treeWidgetContacts->scrollToItem(newItem);

	/* Buddy presence subscribe */
	if (m_sipService && !contact.sipCallerId().isEmpty()) {
		QString buddyURI = QString(QLatin1String("sip:%1@%2")).arg(contact.sipCallerId(), m_userService->userServiceHostNoPort());
		int buddyID = m_sipService->buddySubscribe(buddyURI);
		newItem->setBuddyID(buddyID);
	}

	qApp->information(tr("Contact successfully added."));
	if (m_ui->stackedWidgetViews->currentWidget() != m_ui->pageContacts) {
		m_ui->stackedWidgetViews->setCurrentWidget(m_ui->pageContacts);
	}

	m_ui->treeWidgetContacts->clearSelection();
	m_ui->treeWidgetContacts->setItemSelected(newItem, true);
	m_ui->phoneView->showContactWindow(newItem->contact(), QString(), QString(), QString(), QString());

}

void PhoneMainWindow::slotPhoneUpdateContactFinished(const Command::PhoneUpdateContactParam& param, const Command::PhoneUpdateContactResult& result)
{
	if (result.error() != QLatin1String("0")) {
		qApp->warning(phoneErrorMsg(result.errorMsg()));
		return;
	}

	qDebug() << QPID_DEBUG_PREFIX << param.addBookID();
	qDebug() << QPID_DEBUG_PREFIX << param.updateType();
	qDebug() << QPID_DEBUG_PREFIX << param.oldPhone();
	qDebug() << QPID_DEBUG_PREFIX << param.phoneCountry();
	qDebug() << QPID_DEBUG_PREFIX << param.phone();
	qDebug() << QPID_DEBUG_PREFIX << param.deletePhone();
	qDebug() << QPID_DEBUG_PREFIX << param.voiceraID();
	qDebug() << QPID_DEBUG_PREFIX << param.name();
	qDebug() << QPID_DEBUG_PREFIX << param.sourceLanguage();
	qDebug() << QPID_DEBUG_PREFIX << param.targetLanguage();

	ContactItem *item = m_contactItems.value(param.addBookID());
	if (item) {
	
		if (param.updateType() == UpdateContactOptions::updatePhone_) {
			item->updateContactPhone(param.oldPhone(), param.phoneCountry(), param.phone());
		} else if (param.updateType() == UpdateContactOptions::deletePhone_) {
			item->deleteContactPhone(param.deletePhone());
		} else if (param.updateType() == UpdateContactOptions::updateLanguage_) {
			item->updateInterpretationLanguage(param.sourceLanguage(), param.targetLanguage());
		}
		m_userService->updatePhoneContact(item->contact());
		m_ui->phoneView->updateContactWindow(item->contact());
	}	
}

void PhoneMainWindow::slotPhoneDeleteContactFinished(const Command::PhoneDeleteContactParam& param, const Command::PhoneDeleteContactResult& result)
{
	if (result.error() != QLatin1String("0")) {
		qApp->warning(phoneErrorMsg(result.errorMsg()));
		return;
	}

	ContactItem *item = m_contactItems.value(param.addBookID());
	if (item) {
		if (m_sipService)
			m_sipService->buddySubscribeCancel(item->buddyID());
		if (m_userService)
			m_userService->deletePhoneContact(param.addBookID());
		m_contactItems.remove(param.addBookID());
		delete item;
	}

	QTreeWidgetItem* treeItem = m_ui->treeWidgetContacts->currentItem();
	if (treeItem && treeItem->type() == ContactItemBase::ContactItemType) {
		ContactItem* contactItem = static_cast<ContactItem*>(treeItem);
		if (contactItem && contactItem->contact().addBookId()!=QLatin1String("-1")) {
			Command::PhoneDetailContactParam param;
			param.setAddBookID(contactItem->contact().addBookId());
			m_commandService->phonedetailcontact(param);
		} else {
			//Live interpreter
			m_ui->phoneView->showNeedInterpreterWindow();
		}
	}
}

void PhoneMainWindow::slotPhoneDetailContactFinished(const Command::PhoneDetailContactParam& param, const Command::PhoneDetailContactResult& result)
{
	const ContactItem* contactItem = m_contactItems.value(param.addBookID());
	if (contactItem) {
		m_ui->phoneView->showContactWindow(contactItem->contact(), 
			result.nativeLanguage(), result.nickname(), result.country(), result.major());
	}
}

void  PhoneMainWindow::slotPhoneGetHistoryCDRFinished(const Command::PhoneGetHistoryCDRParam& param, const Command::PhoneGetHistoryCDRResult& result)
{

}

void  PhoneMainWindow::slotPhoneGetUserInfoFinished(const Command::PhoneGetUserInfoParam& param, const Command::PhoneGetUserInfoResult& result)
{
	disconnect(m_commandService, SIGNAL(phonegetuserinfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)), 
		this, SLOT(slotPhoneGetUserInfoFinished(Command::PhoneGetUserInfoParam, Command::PhoneGetUserInfoResult)));

	if (result.voiceraID() == m_userService->userName()) {

		if (result.nickname().isEmpty()) {
			m_ui->lineEditName->setText(result.voiceraID());
		} else {
			m_ui->lineEditName->setText(result.nickname());
		}
//		m_userService->setBalance(result.balance());
		m_ui->lineEditCredit->setText(tr("VE Credit: %1").arg(result.balance()));
		if (!result.smallPhoto().isEmpty() && m_mediaService) {
			QString fileName = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
			fileName.append(QString(QLatin1String("/%1")).arg(m_userService->userName()));
			{
				//remove the file if exist
				if (QFile::exists(fileName))
					QFile::remove(fileName);
			}
			m_mediaService->downloadFile(result.smallPhoto(), fileName);
		} else {
			disconnectPhotoSignal();
		}
	}
}

void  PhoneMainWindow::slotPhoneEditUserInfoFinished(const Command::PhoneEditUserInfoParam& param, const Command::PhoneEditUserInfoResult& result)
{

}

void  PhoneMainWindow::slotPhoneChangePasswordFinished(const Command::PhoneChangePasswordParam& param, const Command::PhoneChangePasswordResult& result)
{

}

void  PhoneMainWindow::slotPhoneNewPasswordFinished(const Command::PhoneNewPasswordParam& param, const Command::PhoneNewPasswordResult& result)
{

}

void  PhoneMainWindow::slotPhoneGetCDRIDFinished(const Command::PhoneGetCDRIDParam& param, const Command::PhoneGetCDRIDResult& result)
{
	User::PhoneHistory history;

	history.setUserName(m_userService->userName());
	history.setCid(result.ID());
	history.setCallerId(result.callerID());
	history.setDeductionTime(result.deductionTime());
	history.setMeetCdrId(result.meetCDRID());
	history.setCallTime(result.callTime());
	history.setContactName(result.contactName());
	history.setCallStatus(result.callStatus());
	history.setSmallPhoto(result.smallPhoto());
	history.setLargePhoto(result.largePhoto());
	history.setDeductionCost(result.deductionCost());
	history.setSourceLanguage(result.sourceLanguage());
	history.setTargetLanguage(result.targetLanguage());
	history.setCallMode(result.callMode());
	history.setPstnCountry(result.PSTNCountry());
	history.setVoiceraId(result.voiceraID());
	User::Contact contact = m_userService->findPhoneContactByNumber(result.callerID());
	if (contact.isValid()) {
		history.setPhotofile(contact.photofile());
	}

	PhoneHistoryItem *newItem = new PhoneHistoryItem(m_ui->treeWidgetRecent, history);
	if (newItem) {
		newItem->setUserService(m_userService);
		m_userService->addPhoneHistory(history);
		m_phoneHistoryItems.insert(history.meetCdrId(), newItem);
	}
}

void  PhoneMainWindow::slotPhoneDeleteCDRFinished(const Command::PhoneDeleteCDRParam& param, const Command::PhoneDeleteCDRResult& result)
{
	if (result.error() != QLatin1String("0")) {
		qApp->warning(phoneErrorMsg(result.errorMsg()));
		return;
	}

	PhoneHistoryItem *item = m_phoneHistoryItems.value(param.meetCDRID());
	if (item) {
		if (m_userService)
			m_userService->deletePhoneHistory(param.meetCDRID());
		m_phoneHistoryItems.remove(param.meetCDRID());
		delete item;
	}

	QTreeWidgetItem* treeItem = m_ui->treeWidgetRecent->currentItem();
	if (treeItem && treeItem->type() == ItemBase::PhoneHistoryItemType) {
		PhoneHistoryItem* historyItem = static_cast<PhoneHistoryItem*>(treeItem);
		User::Contact contact = m_userService->findPhoneContactByNumber(historyItem->phoneHistory().callerId());
		m_ui->phoneView->showHistoryWindow(contact, historyItem->phoneHistory());
	} else {
		m_ui->phoneView->showHomeWindow();
	}
}

void PhoneMainWindow::slotPhotoDownloaded(const QString& addBookID, const QString& fileName)
{
	ContactItem* contactItem = m_contactItems.value(addBookID);
	if (contactItem) {
		contactItem->setPhoto(fileName);
	}
}

void PhoneMainWindow::slotUserPhotoDownloadFailed(const QString& link, const QString& reason)
{
	disconnectPhotoSignal();
}

void PhoneMainWindow::slotUserPhotoDownloaded(const QString& photoLink, const QString& fileName)
{
	disconnectPhotoSignal();
	m_ui->labelPhoto->setPixmap(QPixmap(fileName));
	m_ui->phoneView->setUserPhoto(fileName);
}

void PhoneMainWindow::slotDelayCDR()
{
	if (m_commandService) {
		Command::PhoneGetCDRIDParam param;
		param.setCDRID(m_userService->recentPhoneHistoryID());
		m_commandService->phonegetcdrid(param);
	}
}

void PhoneMainWindow::slotCallingMinimize(bool minimize)
{
	m_ui->widgetCallMinimize->showMinimized(true);
}

void PhoneMainWindow::slotCallingMaximize()
{
	m_ui->widgetCallMinimize->hide();
	m_ui->phoneView->showCallingWindow();
}

void PhoneMainWindow::slotSplashAnswer()
{
	if (m_sipService) {
		m_sipService->answerCall(qApp->call()->callID());
	}
}

void PhoneMainWindow::slotSplashDecline()
{
	if (m_sipService) {
		m_sipService->rejectCall(qApp->call()->callID());
	}
}

void PhoneMainWindow::slotLanguageChanged(QAction* language)
{
	if (language) {
		language->setChecked(true);
		QString languageCountry = language->data().toString();
//		qApp->changeLanguage(languageCountry);
	}
}

void PhoneMainWindow::slotViewProfile(QTreeWidgetItem * item)
{
	if (item && item->type() == ContactItemBase::ContactItemType) {
		m_ui->treeWidgetContacts->setCurrentItem(item);
		ContactItem* contactItem = static_cast<ContactItem*>(item);
		if (contactItem && contactItem->contact().addBookId()!=QLatin1String("-1")) {
			Command::PhoneDetailContactParam param;
			param.setAddBookID(contactItem->contact().addBookId());
			m_commandService->phonedetailcontact(param);
		} else {
			//Live interpreter
			m_ui->phoneView->showNeedInterpreterWindow();
		}
	}
}

void PhoneMainWindow::slotRemoveContact(QTreeWidgetItem * item)
{
	if (item && item->type() == ContactItemBase::ContactItemType) {
		ContactItem* contactItem = static_cast<ContactItem*>(item);
		if (m_commandService) {
			int result = qApp->question(tr("Delete %1 from contacts?").arg(contactItem->contact().contactName().isEmpty()?contactItem->contact().voiceraId():contactItem->contact().contactName()), 
				this, PromptDialog::yes);
			if (result == 1) {
				Command::PhoneDeleteContactParam param;
				param.setAddBookID(contactItem->contact().addBookId());
				m_commandService->phonedelcontact(param);
			}
		}
	}
}

void PhoneMainWindow::slotRemoveHistory(QTreeWidgetItem * item)
{
	if (item && item->type() == ItemBase::PhoneHistoryItemType) {
		PhoneHistoryItem* historyItem = static_cast<PhoneHistoryItem*>(item);
		if (m_commandService) {
			Command::PhoneDeleteCDRParam param;
			param.setMeetCDRID(historyItem->phoneHistory().meetCdrId());
			m_commandService->phonedelcdr(param);
		}
	}
}

} // namespace Qpid
