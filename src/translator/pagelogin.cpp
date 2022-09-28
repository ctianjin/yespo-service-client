#include <QKeyEvent>
#include <QMovie>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QCryptographicHash>
#include <QUrl>
#include <QDesktopServices>
#include "pagelogin.h"
#include "ui_pagelogin.h"
#include "phoneapplication.h"
#include "commandservice.h"
#include "userservice.h"
#include "simplecrypt.h"
#include "protocolerror.h"
#include "messagepanel.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PageLogin:"
#endif // DEBUG_PREFIX

namespace ns {

const QString PageLogin::partner_login_id = QLatin1String("partner_login_id");

struct PageLoginOptions
{
	static const QString remember_;
	static const QString remember_account_;
	static const QString remember_password_;
	static const QString recent_photo_;
};

const QString PageLoginOptions::remember_ = QLatin1String("remember");
const QString PageLoginOptions::remember_account_ = QLatin1String("remember_account");
const QString PageLoginOptions::remember_password_ = QLatin1String("remember_password");
const QString PageLoginOptions::recent_photo_ = QLatin1String("recent_photo");

PageLogin::PageLogin(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_PageLogin),
	m_encryptPassword()
{
    m_ui->setupUi(this);

	m_ui->labelPhoto->setRadius(m_ui->labelPhoto->width()/2.0, m_ui->labelPhoto->height()/2.0);
	m_ui->labelErrorMsg->hide();

	m_movie = new QMovie(QLatin1String(":/images/ui/loading.gif"));
	m_movie->setSpeed(30);
	m_messagePanel = new MessagePanel(this);
	m_messagePanel->setMovie(m_movie);
	m_messagePanel->hide();

	QString version = qApp->applicationVersion();
#ifdef BUILDSYS_OFFICAL
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);
#endif
	m_ui->labelLoginVersion->setText(tr("V : ") + version);
}

PageLogin::~PageLogin()
{
	 
}

UserService* PageLogin::userService() const
{
	return m_userService;
}

void PageLogin::setUserService(UserService* userService)
{
	if (m_userService) {
		disconnect(m_userService->commandService(), SIGNAL(httpError(ns::Service::Error, const QString&, const QString&)),
			this, SLOT(slotServiceError(ns::Service::Error, const QString&, const QString&)));
		disconnect(m_userService, SIGNAL(loginSucceed()), this, SLOT(slotLoginSucceed()));
		disconnect(m_userService, SIGNAL(loginFailed(const QString&, const QString&)), 
			this, SLOT(slotLoginFailed(const QString&, const QString&)));
	}

	m_userService = userService;
	if (m_userService) {
		connect(m_userService->commandService(), SIGNAL(httpError(ns::Service::Error, const QString&, const QString&)),
			this, SLOT(slotServiceError(ns::Service::Error, const QString&, const QString&)));
		connect(m_userService, SIGNAL(loginSucceed()), this, SLOT(slotLoginSucceed()), Qt::QueuedConnection);
		connect(m_userService, SIGNAL(loginFailed(const QString&, const QString&)), 
			this, SLOT(slotLoginFailed(const QString&, const QString&)), Qt::QueuedConnection);
	}
}

void PageLogin::setPreference(Preference* pref)
{
	PreferenceSupport::setPreference(pref);
}

QString PageLogin::userName() const
{
	QString r;
	r = m_ui->lineEditUserName->text();
	r = r.trimmed();
	return r;
}

void PageLogin::setUserName(const QString& userName)
{
	m_ui->lineEditUserName->setText(userName);
}

QString PageLogin::password() const
{
	return m_ui->lineEditPassword->text();
}

void PageLogin::setPassword(const QString& password)
{
	m_ui->lineEditPassword->setText(password);
}

void PageLogin::setVersion(QString version)
{
#ifdef BUILDSYS_OFFICAL
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);
#endif
	m_ui->labelLoginVersion->setText(tr("Version : ") +version);
}

void PageLogin::activeLogin()
{
	m_ui->pushButtonLogin->activateWindow();
}

void PageLogin::logout()
{
	if (m_userService) {
		m_userService->stop();
	}

	typedef PageLoginOptions Opt;
	Preference* pref = preference();

	if (pref) {
		bool remember = pref->getBool(Opt::remember_, false);
		if(!remember) {
			m_ui->lineEditPassword->setText(QString());

			QString photoFile = pref->getString(Opt::recent_photo_);
			if (!photoFile.isEmpty()) {
				QPixmap pic(photoFile);
				QPixmap mask  = QLatin1String(":/images/ui/login_bg_mask.png");
				m_ui->labelPhoto->setMaskPixmap(pic.scaled(m_ui->labelPhoto->size()), mask);
			}
		}
	}
}

void PageLogin::setForgetPasswordLink(const QString& link)
{
	m_forgetPasswordLink = link;
}

void PageLogin::accessSvrError(const QString& errorMsg)
{
	hideMask();

	m_ui->labelErrorMsg->show();

	m_ui->labelErrorMsg->setText(errorMsg);
}

void PageLogin::autoLogin(QString userName, QString password)
{
	setUserName(userName);
	m_encryptPassword = password;
	setPassword(password);
	on_pushButtonLogin_clicked();

	qApp->processEvents();
}

void PageLogin::hideMask()
{
	m_messagePanel->hide();
	m_movie->stop();
}

void PageLogin::slotStateChanged(QAbstractAnimation::State newState)
{
	m_ui->pushButtonBack->setEnabled(newState != QAbstractAnimation::Running);
}

void PageLogin::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

void PageLogin::on_pushButtonForgetPassword_clicked()
{
	if (userName().isEmpty() || m_forgetPasswordLink.isEmpty())
		return;
	QUrl url(m_forgetPasswordLink);
	url.addQueryItem(partner_login_id, userName());
	QDesktopServices::openUrl(url);
}

void PageLogin::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);

	if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {
		on_pushButtonLogin_clicked();
	}
}

void PageLogin::applyPreference(const QSet<QString>& paths, bool force)
{
	typedef PageLoginOptions Opt;
	Preference* pref = preference();

	if (pref) {
		bool remember = pref->getBool(Opt::remember_, false);
		m_ui->checkBoxSavePassword->setChecked(remember);
		if (remember) {
			SimpleCrypt crypto(Q_UINT64_C(0xafd5d610aa473c39));
			m_ui->lineEditUserName->setText(pref->getString(Opt::remember_account_));
			m_ui->lineEditPassword->setText(crypto.decryptToString(pref->getString(Opt::remember_password_)));

// 			m_encryptPassword = pref->getString(Opt::remember_password_);
// 			m_ui->lineEditPassword->setText(m_encryptPassword);
			
			QString photoFile = pref->getString(Opt::recent_photo_);
			if (!photoFile.isEmpty()) {
				QPixmap pic(photoFile);
				QPixmap mask  = QLatin1String(":/images/ui/login_bg_mask.png");
				m_ui->labelPhoto->setMaskPixmap(pic.scaled(m_ui->labelPhoto->size()), mask);
			}
		} 
	}
}

void PageLogin::slotServiceError(ns::Service::Error error, const QString& token, const QString& reason)
{
	qWarning() << DEBUG_PREFIX << Protocol::errorText(reason);
	hideMask();

	m_ui->labelErrorMsg->show();

	if (error > 0) {
		m_ui->labelErrorMsg->setText(Protocol::errorText(reason));
	} 

	Q_EMIT finish(false);
}

void PageLogin::slotLoginSucceed()
{
	typedef PageLoginOptions Opt;
	Preference* pref = preference();
	if (pref) {
		SimpleCrypt crypto(Q_UINT64_C(0xafd5d610aa473c39));
		pref->putString(Opt::remember_account_, m_ui->lineEditUserName->text().trimmed());
		pref->putString(Opt::remember_password_, crypto.encryptToString(m_ui->lineEditPassword->text().trimmed()));
//		pref->putString(Opt::remember_password_, m_encryptPassword);
		pref->putBool(Opt::remember_, m_ui->checkBoxSavePassword->isChecked());
	}

 	//hideMask();
 
 	Q_EMIT finish(true);
}

void PageLogin::slotLoginFailed(const QString& error, const QString& errorMsg)
{
	hideMask();

	m_ui->labelErrorMsg->show();
// 	if (!errorMsg.isEmpty()) {
// 		m_ui->labelErrorMsg->setText(errorMsg);
// 	} else {
// 		m_ui->labelErrorMsg->setText(Protocol::errorText(error));
// 	}

	if (!error.isEmpty()) {
		m_ui->labelErrorMsg->setText(Protocol::errorText(error));
	} else if (!errorMsg.isEmpty()) {
		m_ui->labelErrorMsg->setText(errorMsg);
	}


	Q_EMIT finish(false);
}

void PageLogin::on_pushButtonLogin_clicked()
{
	m_ui->labelErrorMsg->hide();

// 	if (userName().isEmpty()) {
// 		m_ui->labelErrorMsg->show();
// 		m_ui->labelErrorMsg->setText(tr("Please enter the Login ID/Email/Mobile"));
// 		m_ui->lineEditUserName->setFocus();
// 		return;
// 	}
// 
// 	if (password().isEmpty()) {
// 		m_ui->labelErrorMsg->show();
// 		m_ui->labelErrorMsg->setText(tr("Please enter the password"));
// 		m_ui->lineEditPassword->setFocus();
// 		return;
// 	}

	if (m_userService) {
		m_movie->start();
		m_messagePanel->setGeometry(0,0,size().width(), size().height());
		m_messagePanel->show();

// 		if (password() != m_encryptPassword) {
// 			QByteArray encrypt = QCryptographicHash::hash(password().toAscii(), QCryptographicHash::Md5).toHex();
// 			m_encryptPassword = QString::fromAscii(encrypt.constData());
// 		}

		m_userService->setUserName(userName());
		m_userService->setPassword(password());
		m_userService->start();
	}
}

void PageLogin::on_pushButtonBack_clicked()
{
	Q_EMIT back();
}

void PageLogin::retranslateTr()
{
	QString version = qApp->applicationVersion();
#ifdef BUILDSYS_OFFICAL
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);
#endif
	m_ui->labelLoginVersion->setText(tr("V : ") + version);
}

} // namespace ns
