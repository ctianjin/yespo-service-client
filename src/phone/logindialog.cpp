#include <QCryptographicHash>
#include <Qdebug>
#include <QWindowStateChangeEvent>
#include <QUrl>
#include "phoneapplication.h"
#include "logindialog.h"
#include "ui_logindialog.h"
#include "userservice.h"
#include "commandservice.h"

namespace Qpid {

struct LoginDialogOptions
{
	static const QString user_names_;
	static const QString user_pwd_;
};

const QString LoginDialogOptions::user_names_ = QLatin1String("user_names");
const QString LoginDialogOptions::user_pwd_ = QLatin1String("user_pwd");

LoginDialog::LoginDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_LoginDialog),
    m_opt(new LoginDialogOptions)
{
    m_ui->setupUi(this);

	m_ui->lineEditAccount->setDefaultText(tr("Enter your login email address"));
	m_ui->lineEditPassword->setDefaultText(tr("Enter your password"));
	m_ui->lineEditPassword->showAsPassword();
   	
	adjustSize();
}

LoginDialog::~LoginDialog()
{
}

UserService* LoginDialog::userService() const
{
	return m_userService;
}

void LoginDialog::setUserService(UserService* userService)
{
	if (m_userService) {
		disconnect(m_userService, SIGNAL(loginFailed(const QString&, const QString&)), this, SLOT(slotLoginFailed(const QString&, const QString&)));
	}

	m_userService = userService;

	if (m_userService) {
		connect(m_userService, SIGNAL(loginFailed(const QString&, const QString&)), this, SLOT(slotLoginFailed(const QString&, const QString&)));
	}
}

CommandService* LoginDialog::commandService() const
{
	return m_commandService;
}

void LoginDialog::setCommandService(CommandService* commandService)
{
	m_commandService = commandService;
}

QString LoginDialog::userName() const
{
	QString r;
	r = m_ui->lineEditAccount->text();
	r.trimmed();
	return r;
}

void LoginDialog::setUserName(const QString& userName)
{
	m_ui->lineEditAccount->inputText(userName);
}

QString LoginDialog::password() const
{
	return m_ui->lineEditPassword->text();
}

void LoginDialog::setPassword(const QString& password)
{
	m_ui->lineEditPassword->inputText(password);
}

void LoginDialog::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef LoginDialogOptions Opt;
    
	Preference* pref = preference();

	if(pref) {
		
	}
}

void LoginDialog::savePreference()
{
	typedef LoginDialogOptions Opt;
	Preference* pref = preference();

	if (pref) {
		
	}
}

QByteArray LoginDialog::encryptPassword(const QByteArray& text, const QByteArray& key)
{
	Q_UNUSED(key);

	QByteArray ret = QCryptographicHash::hash(text, QCryptographicHash::Md5).toHex();
	return QUrl::toPercentEncoding(QString::fromUtf8(ret.constData(), ret.size()));
}

void LoginDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

bool LoginDialog::event(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange) {
		QWindowStateChangeEvent *stateEvent = static_cast<QWindowStateChangeEvent*>(event);
		if (isVisible()) {
			qApp->setQuitOnLastWindowClosed(true);
		}
	}

	return QDialog::event(event);
}

void LoginDialog::finishLogin(bool succeeded)
{
	if (m_userService) {
		disconnect(m_userService, SIGNAL(stopped()), this, SLOT(slotUserServiceStopped()));
		disconnect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
	}

	if (succeeded)
		Q_EMIT loginSucceeded();
	else
		Q_EMIT loginFailed();
}

void LoginDialog::slotUserServiceStopped()
{
    finishLogin(false);
}

void LoginDialog::slotUserServiceActive()
{
    finishLogin(true);
    accept();
}

void LoginDialog::slotLoginFailed(const QString& error, const QString& errorMsg)
{
	qApp->information(errorMsg);
}

void LoginDialog::on_pushButtonLogin_clicked()
{
	if (userName().isEmpty() || password().isEmpty()) {
		qApp->warning(tr("Please enter the user name and password"));
		m_ui->lineEditAccount->setFocus();
		return;
	}

	if (m_userService) {
		connect(m_userService, SIGNAL(stopped()), this, SLOT(slotUserServiceStopped()));
		connect(m_userService, SIGNAL(active()), this, SLOT(slotUserServiceActive()));
	}

	if (m_userService) {
		m_userService->setUserName(userName());
		m_userService->setPassword(password());
		m_userService->start();
	}
}

void LoginDialog::on_pushButtonForget_clicked()
{
	if (m_commandService) {
		Command::ForgetPasswordParam param;
		param.setUserName(userName());
		m_commandService->forgetpassword(param);
	}
}

void LoginDialog::on_pushButtonCreate_clicked()
{
	if (m_commandService) {
		Command::CreateAccountParam param;
		m_commandService->createaccount(param);
	}
}

} // namespace Qpid
