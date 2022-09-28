#ifndef QPID_PAGEWELCOME_H
#define QPID_PAGEWELCOME_H

#include <QtCore/QScopedPointer>
#include <QWidget>
#include <QPropertyAnimation>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class UserService;
class Ui_PageWelcome;
class PageLogin;
class MessagePanel;

class PageWelcome : public QWidget, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)

public:
    PageWelcome(QWidget* parent = 0);
    ~PageWelcome();

	UserService* userService() const;
	void setUserService(UserService* userService);

	void setVersion(QString version);

	void setError(const QString& error);

	virtual void setPreference(Preference* pref);

	void logout();

	PageLogin* loginPage() const;

	void autoLogin(QString userName, QString password);

Q_SIGNALS:
	void login();
	void stateChanged(QAbstractAnimation::State newState);
protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:
	void on_pushButtonLogin_clicked();
	void slotHideLoginWidget();
private:
	void retranslateTr();

	QPointer<UserService> m_userService;
	QScopedPointer<Ui_PageWelcome> m_ui;
	QScopedPointer<PageLogin> m_widgetLogin;
};

} // namespace ns

#endif // QPID_PAGEWELCOME_H
