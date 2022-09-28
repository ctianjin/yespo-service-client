#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include "calleedialog.h"
#include "capabilityapplication.h"
#include "ui_calleedialog.h"
#include "configinfo.h"
#include "sipservice.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CalleeDialog:"
#endif // QPID_DEBUG_PREFIX

namespace ns {

CalleeDialog::CalleeDialog(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui_CalleeDialog),
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

	connect(m_ui->pushButtonAnswer, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_2, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_3, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_4, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_5, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_6, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_7, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_8, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_9, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_10, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_11, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_12, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_13, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_14, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_15, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_16, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_17, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_18, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_19, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_20, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_21, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_22, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_23, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_24, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_25, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_26, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_27, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_28, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_29, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));
	connect(m_ui->pushButtonAnswer_30, SIGNAL(clicked()), this, SLOT(slotAnswerCall()));

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

CalleeDialog::~CalleeDialog()
{
	
}

SipService* CalleeDialog::sipService() const
{
	return m_sipService;
}

void CalleeDialog::setSipService(SipService* sipService)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
// 		disconnect(m_sipService, SIGNAL(incommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)), 
// 			this, SLOT(slotSipIncommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));

		disconnect(m_sipService, SIGNAL(incomingCallEx(const QStringList&)), 
			this, SLOT(slotSIPIncomingCallEx(const QStringList&)));
	}

	m_sipService = sipService;
	
	if (m_sipService) {
		m_sipService->start();

		connect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
// 		connect(m_sipService, SIGNAL(incommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)), 
// 			this, SLOT(slotSipIncommingCall(int, int, const QString&, const QString&, const QString&, const QString&, const QString&)));
		connect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));

		connect(m_sipService, SIGNAL(incomingCallEx(const QStringList&)), 
			this, SLOT(slotSIPIncomingCallEx(const QStringList&)));
	}
}

void CalleeDialog::setConfig(ConfigInfo* cfg)
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
				if (lineEdit) {
					QString text = it->callerid();
					if (!it->calleeid().isEmpty()) {
						text += QLatin1String("/");
						text += it->calleeid();
					}
					lineEdit->setText(text);
				}				
			}
		}
	}
}

void CalleeDialog::closeEvent(QCloseEvent * event)
{
	if (m_sipService) {
		disconnect(m_sipService, SIGNAL(regState(int, int, const QString&)), this, SLOT(slotRegState(int, int, const QString&)));
		disconnect(m_sipService, SIGNAL(incommingCall(int, int, const QString&, const QString&, const QString&, const QString&)), 
			this, SLOT(slotSipIncommingCall(int, int, const QString&, const QString&, const QString&, const QString&)));
		disconnect(m_sipService, SIGNAL(callState(int, int)), this, SLOT(slotSipCallState(int, int)));
	}

	QDialog::closeEvent(event);
}

void CalleeDialog::slotRegState(int accountid, int code, const QString& reason)
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

void CalleeDialog::slotSipIncommingCall(int accountId, int callId, const QString& srcLang, const QString& dstLang, const QString& mode, const QString& serviceMode, const QString& remote)
{
	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (accountId == it->accountid()) {
			it->setCallid(callId);
			it->setSourceLang(srcLang);
			it->setTargetLang(dstLang);

//			if (it->autoAnswer()) {
				m_sipService->answerCall(callId);
//			}

			QLabel* label = NULL;
			if (it->widget() != NULL) {
				label = it->widget()->findChild<QLabel*>();
				if (label)
					label->setText(QLatin1String("INCOMMING"));
			}

			break;
		}
	}
}

void CalleeDialog::slotSIPIncomingCallEx(const QStringList& callInContentList)
{
	QList<ConfigItem>::iterator it = m_config->items().begin();
	for (; it!=m_config->items().end(); ++it) {
		if (callInContentList.at(0).toInt() == it->accountid()) {
			it->setCallid(callInContentList.at(1).toInt());
// 			it->setSourceLang(srcLang);
// 			it->setTargetLang(dstLang);

			if (it->autoAnswer()) {
				m_sipService->answerCall(callInContentList.at(1).toInt());
			}

			QLabel* label = NULL;
			if (it->widget() != NULL) {
				label = it->widget()->findChild<QLabel*>();
				if (label)
					label->setText(QLatin1String("INCOMMING"));
			}

			break;
		}
	}
}

void CalleeDialog::slotSipCallState(int callId, int state)
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

void CalleeDialog::slotAnswerCall()
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

				 if (it->callid() >= 0)
					 m_sipService->answerCall(it->callid());
			 }
		 }
	 }
}

void CalleeDialog::slotHangupCall()
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
			}
		}
	}
}

void CalleeDialog::slotTimerout()
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
	}
}

void CalleeDialog::on_pushButtonHangupAll_clicked()
{
	hangupAll();
}

void CalleeDialog::on_pushButtonReload_clicked()
{
	hangupAll();
	unregisterAll();

	if (m_config != NULL)
		m_config->reload(m_ui->lineEditFile->text());
}

void CalleeDialog::hangupAll()
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

void CalleeDialog::unregisterAll()
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
