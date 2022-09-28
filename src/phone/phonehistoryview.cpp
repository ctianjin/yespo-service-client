#include <QDebug>
#include <QDateTime>
#include "phonehistoryview.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_phonehistoryview.h"
#include "sipservice.h"
#include "commandservice.h"
#include "callinfo.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneHistoryView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneHistoryViewOptions
{

};

PhoneHistoryView::PhoneHistoryView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneHistoryView),
    m_opt(new PhoneHistoryViewOptions)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->frameOpetatePanel->hide();

	connect(m_ui->contactProfile, SIGNAL(callNumber(const QString&, bool)), this, SLOT(slotCallNumber(const QString&, bool)));
	connect(m_ui->contactProfile, SIGNAL(editNumber(const QString&, const QString&, const QString&)), 
		this, SLOT(slotEditNumber(const QString&, const QString&, const QString&)));
	connect(m_ui->contactProfile, SIGNAL(removeNumber(const QString&)), this, SLOT(slotRemoveNumber(const QString&)));
	connect(m_ui->contactProfile, SIGNAL(simpleCall()), this, SLOT(slotRedial()));
	connect(m_ui->contactProfile, SIGNAL(addToContact()), this, SLOT(slotAddToContact()));
}

PhoneHistoryView::~PhoneHistoryView()
{

}

void PhoneHistoryView::showHistory(const User::Contact& contact, 
				 const User::PhoneHistory& history)
{
	m_contact = contact;
	if (m_contact.isValid()) {
		m_ui->contactProfile->setSimpleShow(false);
		m_ui->contactProfile->setInterpretation(qApp->languageFull(contact.sourceLanguage()), 
			qApp->languageFull(contact.targetLanguage()));
	} else { 
		m_ui->contactProfile->setSimpleShow(true);
	}

	m_history = history;

	if (isCenterCall(m_history.callMode())) {
		m_ui->contactProfile->setPhoto(QLatin1String(":/images/ui/call_live_interpreter.jpg"));
		m_ui->contactProfile->setDisplayName(tr("Live Interpreter"));
		m_ui->contactProfile->setAddToContact(false);
	} else {
		m_ui->contactProfile->setPhoto(m_contact.photofile());
		if (m_contact.isValid()) {
			m_ui->contactProfile->setAddToContact(false);
			m_ui->contactProfile->setDisplayName(m_contact.contactName().isEmpty()?m_contact.voiceraId():m_contact.contactName());
		} else {
			m_ui->contactProfile->setDisplayName(m_history.contactName().isEmpty()?m_history.callerId():m_history.contactName());
			if (m_history.voiceraId().isEmpty())
				m_ui->contactProfile->setAddToContact(false);
			else
				m_ui->contactProfile->setAddToContact(false);
		}
	}
	
	m_ui->contactProfile->setPhoneNumbers(contact.phone());
	m_ui->contactProfile->setOnlineStatus(contact.onlineStatus());

	if (isInterpreterCall(m_history.callMode()) || isCenterCall(m_history.callMode())) {
		m_ui->labelLanguageExchange->show();
		m_ui->labelTargetLanguage->show();
		m_ui->labelSourceLanguage->setText(qApp->languageFull(m_history.sourceLanguage()));
		m_ui->labelTargetLanguage->setText(qApp->languageFull(m_history.targetLanguage()));
	} else {
		m_ui->labelLanguageExchange->hide();
		m_ui->labelTargetLanguage->hide();
		m_ui->labelSourceLanguage->setText(tr("Direct call"));
	}

	if ((m_history.callStatus()==QLatin1String("1") || m_history.callStatus()==QLatin1String("2"))) {
		m_ui->lineEditDirection->setText(tr("Outgoing"));
	} else {
		m_ui->lineEditDirection->setText(tr("Incoming"));
	}

	if (m_history.callStatus()==QLatin1String("3")) {
		m_ui->lineEditCallStatus->setText(tr("Missed"));
	} else if (m_history.deductionTime()==QLatin1String("0")) {
		m_ui->lineEditCallStatus->setText(tr("No answer"));
	} else {
		m_ui->lineEditCallStatus->setText(tr("Answered"));
	}

	QDateTime callTime = QDateTime::fromTime_t(m_history.callTime().toInt());
	m_ui->lineEditCallTime->setText(callTime.toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));

	QString callDuration;
	int duration = m_history.deductionTime().toInt();
	const QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60);
	callDuration += QString::number(currentTime.hour()*60+currentTime.minute());
	callDuration += QLatin1Char('\'');
	callDuration += tr("%1").arg(currentTime.second(), 2, 10, QLatin1Char('0'));
	callDuration += QLatin1Char('\"');
	m_ui->lineEditDuration->setText(callDuration);

	m_ui->lineEditCharge->setText(m_history.deductionCost());
}

void PhoneHistoryView::updateContact(const User::Contact& contact)
{
	m_contact = contact;

	QString displayName = m_contact.contactName();
	if (displayName.isEmpty())
		displayName = m_contact.voiceraId();
	m_ui->contactProfile->setPhoto(m_contact.photofile());
	m_ui->contactProfile->setDisplayName(displayName);
	m_ui->contactProfile->setPhoneNumbers(contact.phone());
	m_ui->contactProfile->setOnlineStatus(contact.onlineStatus());
}

void PhoneHistoryView::setPSTNSupported(const QStringList& supported)
{
	m_ui->contactProfile->setPSTNSupported(supported);
}

void PhoneHistoryView::changeEvent(QEvent* event)
{
	AbstractPhoneView::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

SipService* PhoneHistoryView::sipService() const
{
	return m_sipService;
}

void PhoneHistoryView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

CommandService* PhoneHistoryView::commandService() const
{
	return m_commandService;
}

void PhoneHistoryView::setCommandService(CommandService* commandService)
{
	if (m_commandService) {}

	m_commandService = commandService;

	if (m_commandService) {}
}

void PhoneHistoryView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneHistoryView::signOut()
{
	AbstractPhoneView::signOut();
}

void PhoneHistoryView::slotCallNumber(const QString& number, bool interpreter)
{
	qDebug() << QPID_DEBUG_PREFIX << number << interpreter;

	if (m_sipService && qApp->call()->state()==Call::CallClosed) {
		QString targetNumber;
		QString serviceMode;
		QString callCountry;
		if (number.isEmpty()) {
			/* Empty number means to call Voice Easy ID */
			targetNumber = m_contact.sipCallerId();
			serviceMode = (interpreter ? ServiceModeOptions::VEFree_ : ServiceModeOptions::VEDirect_);
		} else {
			/* PSTN number has the format as CN|1XXXXXXXXXX */
			QStringList tempNumber = number.split(QLatin1String("|"));
			if (tempNumber.size() != 2) {
				qWarning() << QPID_DEBUG_PREFIX << "Invalid number format" << number;
				return;
			}
			targetNumber.prepend(QLatin1String("00"));
			targetNumber = qApp->countryCode(tempNumber[0]) + tempNumber[1];
			serviceMode = (interpreter ? ServiceModeOptions::PSTNFree_ : ServiceModeOptions::PSTNDirect_);
			callCountry = tempNumber[0];
		}
		qApp->call()->setServiceMode(serviceMode);
		qApp->call()->setCalleeNumber(m_contact.sipCallerId());
		qApp->call()->setCallState(Call::CallingPreOut);
		int callID = m_sipService->makeCall(qApp->SIPAccountID(), qApp->makeDestinationUrl(targetNumber, interpreter), 
			m_contact.sourceLanguage(), m_contact.targetLanguage(), serviceMode, callCountry);
		qApp->call()->setCallID(callID);
		if (callID == -1)
			qApp->call()->setCallState(Call::CallClosed);
	}
}

void PhoneHistoryView::slotEditNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone)
{
	qDebug() << QPID_DEBUG_PREFIX << oldPhone << phoneCountry << phone;

	if (m_commandService) {
		Command::PhoneUpdateContactParam param;
		param.setAddBookID(m_contact.addBookId());
		param.setUpdateType(UpdateContactOptions::updatePhone_);

		if (oldPhone.isEmpty()) {
			param.setPhoneCountry(phoneCountry);
			param.setPhone(phone);
		} else {
			param.setOldPhone(oldPhone);
			param.setPhoneCountry(phoneCountry);
			param.setPhone(phone);
		}

		m_commandService->phoneupdatecontact(param);
	}
}

void PhoneHistoryView::slotRemoveNumber(const QString& oldPhone)
{
	qDebug() << QPID_DEBUG_PREFIX << oldPhone;

	if (m_commandService) {
		Command::PhoneUpdateContactParam param;
		param.setAddBookID(m_contact.addBookId());
		param.setUpdateType(UpdateContactOptions::deletePhone_);
		param.setDeletePhone(oldPhone);
		m_commandService->phoneupdatecontact(param);
	}
}

void PhoneHistoryView::slotRedial()
{
	on_pushButtonRedialHistory_clicked();
}

void PhoneHistoryView::slotAddToContact()
{
// 	if (m_commandService && !m_history.voiceraId().isEmpty()) {
// 		Command::PhoneAddContactParam param;
// 		param.setVoiceraID(m_history.voiceraId());
// 		m_commandService->phoneaddcontact(param);
// 	}
}

void PhoneHistoryView::on_pushButtonRedialHistory_clicked()
{
	if (m_sipService && qApp->call()->state()==Call::CallClosed) {

		qDebug() << m_history.callMode() << m_history.callerId() << m_history.sourceLanguage() << m_history.targetLanguage() << m_history.pstnCountry();

		qApp->call()->setServiceMode(m_history.callMode());
		qApp->call()->setCalleeNumber(m_history.callerId());
		qApp->call()->setCallState(Call::CallingPreOut);
		qApp->call()->setCalleeName(m_history.contactName());
		int callID = m_sipService->makeCall(qApp->SIPAccountID(), 
			qApp->makeDestinationUrl(m_history.callerId(), isInterpreterCall(m_history.callMode())), 
			m_history.sourceLanguage(), m_history.targetLanguage(), m_history.callMode(), m_history.pstnCountry());
		qApp->call()->setCallID(callID);
		if (callID == -1)
			qApp->call()->setCallState(Call::CallClosed);
	}
}

void PhoneHistoryView::on_pushButtonDeleteHistory_clicked()
{
	if (m_commandService) {
		Command::PhoneDeleteCDRParam param;
		param.setMeetCDRID(m_history.meetCdrId());
		m_commandService->phonedelcdr(param);
	}
}

void PhoneHistoryView::retranslateTr()
{
	if (isInterpreterCall(m_history.callMode()) || isCenterCall(m_history.callMode())) {
		m_ui->labelLanguageExchange->show();
		m_ui->labelTargetLanguage->show();
		m_ui->labelSourceLanguage->setText(qApp->languageFull(m_history.sourceLanguage()));
		m_ui->labelTargetLanguage->setText(qApp->languageFull(m_history.targetLanguage()));
	} else {
		m_ui->labelLanguageExchange->hide();
		m_ui->labelTargetLanguage->hide();
		m_ui->labelSourceLanguage->setText(tr("Direct call"));
	}
}

} // namespace Qpid
