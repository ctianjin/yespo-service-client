#include "sipnotify.h"
#include "ui_sipnotify.h"
#include "commandservice.h"
#include "userservice.h"
#include "sipservice.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QMovie>

#define  RECONNECTTIMES 3
#ifndef DEBUG_PREFIX
#	define DEBUG_PREFIX "SipNotify:"
#endif // DEBUG_PREFIX
namespace ns {

SipNotify::SipNotify(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_SipNotify),
	m_reconTimes(0)
{
    m_ui->setupUi(this);
	
	m_movie = new QMovie(QLatin1String(":/images/ui/connecting.gif"));
	m_movie->setSpeed(30);
	m_ui->labelMovie->setMovie(m_movie);

	hide();
}

SipNotify::~SipNotify()
{
	 
}

CommandService* SipNotify::commandService() const 
{
	return m_commandService;
}

void SipNotify::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(getserverFinished(Command::GetServerParam, Command::GetServerResult)),
			this, SLOT(slotGetServerFinished(Command::GetServerParam, Command::GetServerResult)));
	}

	m_commandService = commandService;

	if (m_commandService) {
		connect(m_commandService, SIGNAL(getserverFinished(Command::GetServerParam, Command::GetServerResult)),
			this, SLOT(slotGetServerFinished(Command::GetServerParam, Command::GetServerResult)));
	}
}

UserService* SipNotify::userService() const 
{
	return m_userService;
}

void SipNotify::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;

	if (m_userService) {
	}
}

SipService* SipNotify::sipService() const 
{
	return m_sipService;
}

void SipNotify::setSipService(SipService* sipService) 
{
	if (m_sipService) {
	}

	m_sipService = sipService;

	if (m_sipService) {
	}
}

void SipNotify::setError(const QString& code, const QString& reason)
{
	if (m_movie) {
		m_movie->stop(); 
	} 
#if 0
	m_ui->labelMovie->hide();
	m_ui->labelNotify->setText(tr("Unable to connect to server, please reconnect(%1)").arg(code));
#endif
	int responseCode = code.toInt();
	bool reconnect = false;
	switch(responseCode) {
		case 408:
		case 503:
		case 502:
			reconnect = true;
			break;
		default:
			break;
	}
#if 0
	m_ui->pushButtonNotify->setVisible(reconnect);
	m_ui->labelNotifyIcon->show();
#endif

	if (reconnect) {
		if (m_reconTimes >= RECONNECTTIMES) {
			m_ui->labelMovie->hide();
			m_ui->labelNotify->setText(tr("Unable to connect to server, please reconnect(%1)").arg(code));
			m_ui->pushButtonNotify->setVisible(reconnect);
			m_ui->labelNotifyIcon->show();
			return;
		}
		on_pushButtonNotify_clicked();
	} else {
		m_ui->labelMovie->hide();
		m_ui->labelNotify->setText(tr("Unable to connect to server, please reconnect(%1)").arg(code));
		m_ui->pushButtonNotify->setVisible(reconnect);
		m_ui->labelNotifyIcon->show();
	}
} 

void SipNotify::registedSucceed()
{
	if (m_movie) {
		m_movie->stop();
	}

	m_reconTimes = 0;

	hide();
}

void SipNotify::registing()
{
	m_ui->pushButtonNotify->hide();
	m_ui->labelNotify->setText(tr("Connecting..."));
	m_ui->labelNotifyIcon->hide();
	m_ui->labelMovie->show();
	m_movie->start();
}

void SipNotify::registerError(ns::Service::Error error, const QString& token)
{
	qWarning() << DEBUG_PREFIX << error << token;
	m_ui->labelNotify->setText(tr("Network unavailable, please try again later %1").arg(token));
	m_ui->labelNotifyIcon->show();
	m_ui->pushButtonNotify->show();
	m_ui->labelMovie->hide();
	if (m_movie) {
		m_movie->stop();
	}
}

int SipNotify::reconnectServer()
{
	qWarning() << DEBUG_PREFIX << "sipServiceHost:" << m_domainCall;
	int result = 0;
	if (m_sipService && m_userService) {
		m_sipService->unregisterServer();

		if (!m_sipServerList.isEmpty()) {
			if (m_sipServerList.count() == 1) {
				m_sipService->registerServer(m_userService->callerId(), m_userService->secret(),
					m_userService->sipServiceHost());
				m_domainCall = m_userService->sipServiceHost();
			} else {
				int i;
				for (i = 0; i < m_sipServerList.count(); i++) {
					if (m_domainCall != m_sipServerList.at(i)) {
						m_sipService->registerServer(m_userService->callerId(), m_userService->secret(),
							m_sipServerList.at(i));
						m_userService->setServiceSipHost(m_sipServerList.at(i));
						m_domainCall = m_sipServerList.at(i);
						notifySipStatus(m_domainCall);
						break;
					}
				}
				if (i == m_sipServerList.count()) {
					m_sipService->registerServer(m_userService->callerId(), m_userService->secret(),
						m_userService->sipServiceHost());
					m_domainCall = m_userService->sipServiceHost();
				}
			}
		}
		else {
			qWarning() << DEBUG_PREFIX << "sip server list is empty!";
			m_sipService->registerServer(m_userService->callerId(), m_userService->secret(),
				m_userService->sipServiceHost());
			m_domainCall = m_userService->sipServiceHost();
		}
	}	

	qWarning() << DEBUG_PREFIX << "sipServiceHost:" << m_domainCall;

	return result;
}

void SipNotify::getServer()
{
	if (m_commandService) {
		Command::GetServerParam param;
		m_commandService->getserver(param);
	}
}

void SipNotify::notifySipStatus(const QString& domainCall)
{
	if (m_commandService) {
		Command::SetClientInfoParam param;
		param.setLastDomainCall(domainCall);
		m_commandService->setclientinfo(param);
	}
}

void SipNotify::setDomainCall(const QString& domainCall)
{
	m_domainCall = domainCall;
}

void SipNotify::setSipServerList(const QStringList& list)
{
	m_sipServerList = list;
}

void SipNotify::on_pushButtonNotify_clicked()
{
	registing();

	m_reconTimes += 1;

	reconnectServer();
}

void SipNotify::slotGetServerFinished(const Command::GetServerParam& param, const Command::GetServerResult& result)
{
	qWarning() << DEBUG_PREFIX << result.domainCall();
	if (result.common().error().isEmpty()) {
		m_sipServerList = result.sipServerList();

		if (m_sipServerList.isEmpty() && m_sipService) {
			m_sipService->registerServer(m_userService->callerId(), m_userService->secret(), m_domainCall);
			return;
		}

		reconnectServer();
	} else {
		qWarning() << DEBUG_PREFIX << "getserver failed";
		setError(QLatin1String("1000"), QString());
	}
}

void SipNotify::changeEvent(QEvent* event)
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

void SipNotify::retranslateTr()
{

}

} // namespace ns
