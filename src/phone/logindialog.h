#ifndef QPID_LOGINDIALOG_H
#define QPID_LOGINDIALOG_H

#include <QPointer>
#include <QScopedPointer>
#include "preferencesupport.h"
#include "framelessdialog.h"

namespace Qpid {

class UserService;
class CommandService;
class Ui_LoginDialog;
struct LoginDialogOptions;

class LoginDialog : public FramelessDialog, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(QString userName READ userName WRITE setUserName)
	Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    explicit LoginDialog(QWidget* parent = 0);
    virtual ~LoginDialog();

	UserService* userService() const;
	void setUserService(UserService* userService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	QString userName() const;
	void setUserName(const QString& userName);

	QString password() const;
	void setPassword(const QString& password);

public Q_SLOTS:

Q_SIGNALS:
    void loginStarted();
    void loginSucceeded();
    void loginFailed();

protected:
	virtual bool event(QEvent* event);
	virtual void changeEvent(QEvent* event);
    virtual void applyPreference(const QSet<QString>& paths, bool force);

private Q_SLOTS:
    void slotUserServiceStopped();
    void slotUserServiceActive();
	void slotLoginFailed(const QString& error, const QString& errorMsg);
	void on_pushButtonLogin_clicked();
	void on_pushButtonForget_clicked();
	void on_pushButtonCreate_clicked();

private:
    void finishLogin(bool succeeded);
    void savePreference();
	QByteArray encryptPassword(const QByteArray& text, const QByteArray& key);

    QScopedPointer<Ui_LoginDialog> m_ui;
    QScopedPointer<LoginDialogOptions> m_opt;
	QPointer<UserService> m_userService;
	QPointer<CommandService> m_commandService;

    Q_DISABLE_COPY(LoginDialog)
};

} // namespace Qpid

#endif // QPID_LOGINDIALOG_H
