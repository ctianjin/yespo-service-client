#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include "callerdialog.h"
#include "capabilityapplication.h"
#include "ui_callerdialog.h"
#include "configinfo.h"
#include "sipservice.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CallerDialog:"
#endif // QPID_DEBUG_PREFIX

namespace ns {

CallerDialog::CallerDialog(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui_CallerDialog),
	m_config(0),
	m_timer(0)
{
    m_ui->setupUi(this);

	m_widgets.push_back(m_ui->widgetInfo);
	m_widgets.push_back(m_ui->widgetInfo_2);
	m_widgets.push_back(m_ui->widgetInfo_3);
	m_widgets.push_back(m_ui->widgetInfo_4);
	m_widgets.push_back(m_ui->widgetInfo_5);
	m_widgets.push_back(m_ui->widgetInfo_6);
	m_widgets.push_back(m_ui->widgetInfo_7);
	m_widgets.push_back(m_ui->widgetInfo_8);
	m_widgets.push_back(m_ui->widgetInfo_9);
	m_widgets.push_back(m_ui->widgetInfo_10);
	m_widgets.push_back(m_ui->widgetInfo_11);
	m_widgets.push_back(m_ui->widgetInfo_12);
	m_widgets.push_back(m_ui->widgetInfo_13);
	m_widgets.push_back(m_ui->widgetInfo_14);
	m_widgets.push_back(m_ui->widgetInfo_15);
	m_widgets.push_back(m_ui->widgetInfo_16);
	m_widgets.push_back(m_ui->widgetInfo_17);
	m_widgets.push_back(m_ui->widgetInfo_18);
	m_widgets.push_back(m_ui->widgetInfo_19);
	m_widgets.push_back(m_ui->widgetInfo_20);
	m_widgets.push_back(m_ui->widgetInfo_21);
	m_widgets.push_back(m_ui->widgetInfo_22);
	m_widgets.push_back(m_ui->widgetInfo_23);
	m_widgets.push_back(m_ui->widgetInfo_24);
	m_widgets.push_back(m_ui->widgetInfo_25);
	m_widgets.push_back(m_ui->widgetInfo_26);
	m_widgets.push_back(m_ui->widgetInfo_27);
	m_widgets.push_back(m_ui->widgetInfo_28);
	m_widgets.push_back(m_ui->widgetInfo_29);
	m_widgets.push_back(m_ui->widgetInfo_30);

	connect(m_ui->pushButtonCall, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_2, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_3, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_4, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_5, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_6, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_7, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_8, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_9, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_10, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_11, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_12, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_13, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_14, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_15, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_16, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_17, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_18, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_19, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_20, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_21, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_22, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_23, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_24, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_25, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_26, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_27, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_28, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_29, SIGNAL(clicked()), this, SLOT(slotMakeCall()));
	connect(m_ui->pushButtonCall_30, SIGNAL(clicked()), this, SLOT(slotMakeCall()));

	connect(m_ui->pushButtonHangup, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_2, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_3, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_4, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_5, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_6, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_7, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_8, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_9, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_10, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_11, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_12, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_13, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_14, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_15, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_16, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_17, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_18, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_19, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_20, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_21, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_22, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_23, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_24, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_25, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_26, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_27, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_28, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_29, SIGNAL(clicked()), this, SLOT(slotHangupCall()));
	connect(m_ui->pushButtonHangup_30, SIGNAL(clicked()), this, SLOT(slotHangupCall()));

	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimerout()));
	m_timer->start();

	m_ui->pushButtonReload->hide();
}

CallerDialog::~CallerDialog()
{
	
}

SipService* CallerDialog::sipService() const
{
	return m_sipService;
}

void CallerDialog::setSipService(SipService* sipService)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));
	}

	m_sipService = sipService;
	
	if (m_sipService) {
		m_sipService->start();
		connect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
		connect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));
	}
}

void CallerDialog::setConfig(ConfigInfo* cfg)
{
	if (m_config != NULL) {
		hangupAll();
		unregisterAll();
	}

	m_config = cfg;

	if (m_config != NULL) {

		m_ui->lineEditFile->setText(m_config->fileName());

		if (!m_config->isEmpty()) {
			QList<ConfigItem>::iterator it = m_config->items().begin();
			int widgetIndex = 0;
			for (; it!=m_config->items().end(); ++it,++widgetIndex) {
				if (widgetIndex >= m_sipService->maxAccountCount())
					break;
				qDebug() << "Register server with : " << it->callerid() << it->password() << m_config->server();
				it->setWidget(m_widgets.at(widgetIndex));
				int accountid = m_sipService->registerServer(it->callerid(), it->password(), m_config->server());
				if (accountid != -1) {
					qDebug() << "Account id is : " << accountid;
					it->setAccountid(accountid);
				}

				QLineEdit* lineEdit = it->widget()->findChild<QLineEdit*>();
				if (lineEdit)
				{
					lineEdit->setText(it->callerid()+QLatin1String("/")+it->calleeid());
				}				
			}
		}
	}
}

void CallerDialog::closeEvent(QCloseEvent * event)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));
	}

	QDialog::closeEvent(event);
}

void CallerDialog::slotRegState(int accountid, int code, const QString& reason)
{
	QWidget* widget = NULL;
	QLabel* label = NULL;
	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (accountid == it->accountid()) {
			widget = it->widget();
			label = widget->findChild<QLabel*>();
			break;
		}
	}

	if (label == NULL)
		return;

	switch(code) {
		case 200: /* 200/OK*/
			label->setText(QLatin1String("IDLE"));
			if (it != m_config->items().end())
				it->setRegStatus(ConfigItem::REGISTERED);
			break;
		default:
			label->setText(QString::number(code));
			if (it != m_config->items().end())
				it->setRegStatus(ConfigItem::UNREGISTER);
			break;
	}
}

void CallerDialog::slotSipCallState(int callId, int state)
{
	qDebug() << QPID_DEBUG_PREFIX << callId << state;

	QWidget* widget = NULL;
	QLabel* label = NULL;
	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (callId == it->callid()) {
			widget = it->widget();
			if (widget != NULL)
				label = widget->findChild<QLabel*>();
			break;
		}
	}

	if (label == NULL)
		return;


//0 	PJSIP_INV_STATE_NULL,	    /**< Before INVITE is sent or received  */
//1	PJSIP_INV_STATE_CALLING,	    /**< After INVITE is sent		    */
//2 	PJSIP_INV_STATE_INCOMING,	    /**< After INVITE is received.	    */
//3 	PJSIP_INV_STATE_EARLY,	    /**< After response with To tag.	    */
//4 	PJSIP_INV_STATE_CONNECTING,	    /**< After 2xx is sent/received.	    */
//5 	PJSIP_INV_STATE_CONFIRMED,	    /**< After ACK is sent/received.	    */
//6 	PJSIP_INV_STATE_DISCONNECTED,   /**< Session is terminated.		    */

	switch(state) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			label->setText(QLatin1String("INCOMMING"));
			if (it != m_config->items().end()) {
				it->setCallStatus(ConfigItem::INCOMMING);
			}
			break;
		case 3:
			label->setText(QLatin1String("RINGBACK"));
			if (it != m_config->items().end()) {
				it->setCallStatus(ConfigItem::RINGBACK);
			}
			break;
		case 4:
			break;
		case 5:
			label->setText(QLatin1String("ANSWERED"));
			if (it != m_config->items().end()) {
				it->setCallStatus(ConfigItem::ANSWERED);
			}
			break;
		case 6:
			label->setText(QLatin1String("IDLE"));
			if (it != m_config->items().end()) {
				it->setCallStatus(ConfigItem::IDLE);
				it->setCallid(-1);
				it->setCallDuration(0);
			}
			break;
	}
}

void CallerDialog::slotMakeCall()
{
	if (m_config == NULL)
		return;

	 QPushButton* call = qobject_cast<QPushButton*>(sender());
	 if (call!=NULL) {
		 QWidget* widget = NULL;
		 QLabel* label = NULL;
		 QList<ConfigItem>::iterator it = m_config->items().begin();
		 for (; it!=m_config->items().end(); ++it) {
			 if (call->parentWidget() == it->widget()) {

				 QString dstUri = QLatin1String("sip:");
				 dstUri += it->calleeid();
				 dstUri += QLatin1String("@");
				 dstUri += m_config->server();

				 int callid = m_sipService->makeCall(it->accountid(), dstUri, it->sourceLang(), it->targetLang(), QString(), QString());
				 qDebug() << "Make call " << it->accountid() << " callid is : " << callid << it->sourceLang() << it->targetLang();
				 it->setCallid(callid);
				 break;
			 }
		 }
	 }
}

void CallerDialog::slotHangupCall()
{
	if (!m_config || m_config->isEmpty())
		return;

	QPushButton* hangup = qobject_cast<QPushButton*>(sender());
	if (hangup!=NULL) {
		QWidget* widget = NULL;
		QLabel* label = NULL;
		QList<ConfigItem>::iterator it = m_config->items().begin();
		for (; it!=m_config->items().end(); ++it) {
			if (hangup->parentWidget()==it->widget() && it->callid()>=0) {
				m_sipService->hangupCall(it->callid());
				break;
			}
		}
	}
}

void CallerDialog::slotTimerout()
{
	if (!m_config || m_config->isEmpty())
		return;

	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		
		if (it->callStatus() == ConfigItem::ANSWERED) {
			it->setCallDuration(it->callDuration()+1);
			QLabel* label = it->widget()->findChild<QLabel*>();
			if (label != NULL)
			{
				int duration = it->callDuration();
				const QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60);
				QString format;
				if (duration > 3600) {
					format = QLatin1String("hh:mm:ss");
				} else {
					format = QLatin1String("mm:ss");
				}
				label->setText(tr("%1").arg(currentTime.toString(format)));
			}
		}

		if (it->absoluteTime()!=0 && it->callDuration()>=it->absoluteTime()) {
			m_sipService->hangupCall(it->callid());
		}

		QString calleeid = it->calleeid();
		if (it->autoCall()>0 
			&& it->regStatus()>=ConfigItem::REGISTERED 
			&& it->callStatus()==ConfigItem::IDLE 
			&& !calleeid.isEmpty() 
			&& it->callid()==-1) 
		{
			it->setIdleDuration(it->idleDuration()+1);

			if (it->idleDuration() >= it->autoCall())
			{
				QString dstUri = QLatin1String("sip:");
				dstUri += it->calleeid();
				dstUri += QLatin1String("@");
				dstUri += m_config->server();

				int callid = m_sipService->makeCall(it->accountid(), dstUri, it->sourceLang(), it->targetLang(), QString(), QString());
				qDebug() << "Make call " << it->accountid() << " callid is : " << callid << it->sourceLang() << it->targetLang();
				it->setCallid(callid);
				it->setIdleDuration(0);
			}

		} else {
			it->setIdleDuration(0);
		}
	}
}

void CallerDialog::on_pushButtonHangupAll_clicked()
{
	hangupAll();
}

void CallerDialog::on_pushButtonReload_clicked()
{
	hangupAll();
	unregisterAll();

	if (m_config != NULL)
		m_config->reload(m_ui->lineEditFile->text());
}

void CallerDialog::hangupAll()
{
	if (!m_config || m_config->isEmpty())
		return;

	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (it->regStatus()==ConfigItem::REGISTERED && it->callid()>=0) {
			m_sipService->hangupCall(it->callid());
		}
	}
}

void CallerDialog::unregisterAll()
{
	if (!m_config || m_config->isEmpty())
		return;

	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (it->accountid()>=0) {
			m_sipService->unregisterServer(it->accountid());
		}
	}
}

} // namespace Qpid
