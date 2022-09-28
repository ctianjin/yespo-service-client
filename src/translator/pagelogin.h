#ifndef PAGELOGIN_H
#define PAGELOGIN_H

#include <QtCore/QScopedPointer>
#include <QWidget>
#include <QAbstractAnimation>
#include <QPointer>
#include "preferencesupport.h"
#include "service.h"

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class UserService;
class CommandService;
class Ui_PageLogin;
class MessagePanel;

class PageLogin : public QWidget, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)

public:
	static const QString partner_login_id;

    PageLogin(QWidget* parent = 0);
    ~PageLogin();

	UserService* userService() const;
	void setUserService(UserService* userService);

	virtual void setPreference(Preference* pref);

	QString userName() const;
	void setUserName(const QString& userName);

	QString password() const;
	void setPassword(const QString& password);

	void setVersion(QString version);

	void activeLogin();

	void logout();

	void setForgetPasswordLink(const QString& link);

	void accessSvrError(const QString& errorMsg);

public Q_SLOTS:
	void autoLogin(QString userName, QString password);
	void hideMask();
	void slotStateChanged(QAbstractAnimation::State newState);
	void on_pushButtonForgetPassword_clicked();
Q_SIGNALS:
	void back();
	void finish(bool succeed);

protected:
	virtual void changeEvent(QEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

private	Q_SLOTS:
	void slotServiceError(ns::Service::Error error, const QString& token, const QString& reason);
	void slotLoginSucceed();
	void slotLoginFailed(const QString& error, const QString& errorMsg);

	void on_pushButtonLogin_clicked();
	void on_pushButtonBack_clicked();

private:
	void retranslateTr();

	QMovie* m_movie;
	MessagePanel* m_messagePanel;

	QString m_encryptPassword;
	QString m_forgetPasswordLink;

	QPointer<UserService> m_userService;
	QScopedPointer<Ui_PageLogin> m_ui;
};

} // namespace ns

#endif // PAGELOGIN_H
