#ifndef SIPNOTIFY_H
#define SIPNOTIFY_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include <QWidget>
#include "command.h"

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class Ui_SipNotify;
class CommandService;
class UserService;
class SipService;

class SipNotify : public QWidget
{
    Q_OBJECT
																																   
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

public:
    SipNotify::SipNotify(QWidget* parent = 0);
    ~SipNotify();

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	void setError(const QString& code, const QString& reason);
	void registedSucceed();
	void registing();
	void registerError(ns::Service::Error error, const QString& token);
	
	int reconnectServer();

	void getServer();

	void notifySipStatus(const QString& domainCall);

	void setDomainCall(const QString& domainCall);
	void setSipServerList(const QStringList& list);
	void resetReconTimes() {m_reconTimes = 0;}
Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:
	void on_pushButtonNotify_clicked();
	void slotGetServerFinished(const Command::GetServerParam& param, const Command::GetServerResult& result);
private:
	void retranslateTr();
	int m_regCode;
	int m_reconTimes;
	QStringList m_sipServerList;
	QString m_domainCall;
	QMovie* m_movie;

	QPointer<CommandService> m_commandService;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;

	QScopedPointer<Ui_SipNotify> m_ui;
};

} // namespace ns

#endif // SIPNOTIFY_H
