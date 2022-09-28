#include <QDebug>
#include "phonecontactview.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_phonecontactview.h"
#include "sipservice.h"
#include "commandservice.h"
#include "callinfo.h"
#include "interpretationselect.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneContactView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneContactViewOptions
{

};

PhoneContactView::PhoneContactView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneContactView),
    m_opt(new PhoneContactViewOptions),
	m_interpretationSelect(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);
	m_ui->frameDetailDelete->hide();

	connect(m_ui->contactProfile, SIGNAL(callNumber(const QString&, bool)), this, SLOT(slotCallNumber(const QString&, bool)));

	connect(m_ui->contactProfile, SIGNAL(editNumber(const QString&, const QString&, const QString&)), 
		this, SLOT(slotEditNumber(const QString&, const QString&, const QString&)));
	connect(m_ui->contactProfile, SIGNAL(removeNumber(const QString&)), this, SLOT(slotRemoveNumber(const QString&)));

	m_interpretationSelect = new InterpretationSelect(this);
	m_interpretationSelect->hideHelp();
	connect(m_interpretationSelect, SIGNAL(interpreterAdded(const QString&, const QString&)), this, SLOT(slotInterpreterAdded(const QString&, const QString&)));

	m_ui->contactProfile->setSimpleShow(false);
}

PhoneContactView::~PhoneContactView()
{

}

void PhoneContactView::showContact(const User::Contact& contact, 
				 const QString& nativeLanguage, 
				 const QString& nickname, 
				 const QString& nationality, 
				 const QString& major)
{
	m_contact = contact;
	m_nativeLanguage = nativeLanguage;
	m_nickname = nickname;
	m_nationality = nationality;
	m_major = major;

	m_ui->labelSourceLanguage->setText(qApp->languageFull(contact.sourceLanguage()));
	m_ui->labelTargetLanguage->setText(qApp->languageFull(contact.targetLanguage()));

	m_ui->lineEditVoiceID->setText(contact.voiceraId());

	m_ui->lineEditLanguage->setText(qApp->languageFull(nativeLanguage));

	m_ui->lineEditNationality->setText(qApp->countryFull(nationality.toUpper()));

	m_ui->lineEditProfession->setText(qApp->majorName(major));

	if (m_contact.addBookId() == QLatin1String("-1"))
		m_ui->pushButtonDeleteContact->hide();
	else
		m_ui->pushButtonDeleteContact->show();	

	QString displayName = m_nickname.isEmpty()?m_contact.contactName():m_nickname;
	if (displayName.isEmpty())
		displayName = m_contact.voiceraId();
	m_ui->contactProfile->setPhoto(m_contact.photofile());
	m_ui->contactProfile->setDisplayName(displayName);
	m_ui->contactProfile->setVoiceraID(m_contact.voiceraId());
	m_ui->contactProfile->setPhoneNumbers(contact.phone());
	m_ui->contactProfile->setOnlineStatus(contact.onlineStatus());
	m_ui->contactProfile->setInterpretation(m_ui->labelSourceLanguage->text(), m_ui->labelTargetLanguage->text());

	m_interpretationSelect->hide();
}

void PhoneContactView::updateContact(const User::Contact& contact)
{
	m_contact = contact;

	m_ui->labelSourceLanguage->setText(qApp->languageFull(contact.sourceLanguage()));
	m_ui->labelTargetLanguage->setText(qApp->languageFull(contact.targetLanguage()));

	QString displayName = m_nickname.isEmpty()?m_contact.contactName():m_nickname;
	if (displayName.isEmpty())
		displayName = m_contact.voiceraId();
	m_ui->contactProfile->setPhoto(m_contact.photofile());
	m_ui->contactProfile->setDisplayName(displayName);
	m_ui->contactProfile->setPhoneNumbers(contact.phone());
	m_ui->contactProfile->setOnlineStatus(contact.onlineStatus());
	m_ui->contactProfile->setInterpretation(m_ui->labelSourceLanguage->text(), m_ui->labelTargetLanguage->text());
}


void PhoneContactView::setPSTNSupported(const QStringList& supported)
{
	m_ui->contactProfile->setPSTNSupported(supported);
}

void PhoneContactView::changeEvent(QEvent* event)
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

SipService* PhoneContactView::sipService() const
{
	return m_sipService;
}

void PhoneContactView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

CommandService* PhoneContactView::commandService() const
{
	return m_commandService;
}

void PhoneContactView::setCommandService(CommandService* commandService)
{
	if (m_commandService) {}

	m_commandService = commandService;

	if (m_commandService) {}
}

void PhoneContactView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneContactView::signOut()
{
	m_interpretationSelect->close();

	AbstractPhoneView::signOut();
}

void PhoneContactView::on_pushButtonLanguageEdit_clicked()
{
	m_interpretationSelect->initialSourceLanguage();
	m_interpretationSelect->show();
}

void PhoneContactView::on_pushButtonDeleteContact_clicked()
{
	if (m_commandService) {
		int result = qApp->question(tr("Delete %1 from contacts?").arg(m_contact.contactName().isEmpty()?m_contact.voiceraId():m_contact.contactName()), 
			this, PromptDialog::yes);
		if (result == 1) {
			Command::PhoneDeleteContactParam param;
			param.setAddBookID(m_contact.addBookId());
			m_commandService->phonedelcontact(param);
		}
	}
}

void PhoneContactView::slotCallNumber(const QString& number, bool interpreter)
{
//	qWarning() << QPID_DEBUG_PREFIX << number << interpreter;

	if (interpreter && 
		(m_contact.sourceLanguage().isEmpty() || m_contact.targetLanguage().isEmpty())) {
			on_pushButtonLanguageEdit_clicked();
			return;
	}

	if (m_sipService && qApp->call()->state()==Call::CallClosed) {
		QString targetNumber;
		QString serviceMode;
		QString callCountry;
		if (number.isEmpty() && !m_contact.sipCallerId().isEmpty()) {
			/* Empty number means to call Voice Easy ID */
			targetNumber = m_contact.sipCallerId();
			if (targetNumber.isEmpty()) {
				qWarning() << QPID_DEBUG_PREFIX << "The extension number is empty!";
				return;
			}
			serviceMode = (interpreter ? ServiceModeOptions::VEFree_ : ServiceModeOptions::VEDirect_);
		} else {
			targetNumber = number;
			if (targetNumber.isEmpty()) {
				targetNumber = m_contact.phone();
				targetNumber = targetNumber.split(QLatin1Char(',')).at(0);
			}
			/* PSTN number has the format as CN|1XXXXXXXXXX */
			QStringList tempNumber = targetNumber.split(QLatin1String("|"));
			if (tempNumber.size() != 2) {
				qWarning() << QPID_DEBUG_PREFIX << "Invalid number format" << targetNumber;
				return;
			}
			targetNumber = tempNumber[1];
			serviceMode = (interpreter ? ServiceModeOptions::PSTNFree_ : ServiceModeOptions::PSTNDirect_);
			callCountry = tempNumber[0];
		}
		qApp->call()->setServiceMode(serviceMode);
		qApp->call()->setCalleeNumber(targetNumber);
		qApp->call()->setCallState(Call::CallingPreOut);
		int callID = m_sipService->makeCall(qApp->SIPAccountID(), qApp->makeDestinationUrl(targetNumber, interpreter), 
			m_contact.sourceLanguage(), m_contact.targetLanguage(), serviceMode, callCountry);
		qApp->call()->setCallID(callID);
		if (callID == -1)
			qApp->call()->setCallState(Call::CallClosed);
	}
}

void PhoneContactView::slotEditNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone)
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

void PhoneContactView::slotRemoveNumber(const QString& oldPhone)
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

void PhoneContactView::slotInterpreterAdded(const QString& source, const QString& target)
{
	qDebug() << QPID_DEBUG_PREFIX << source << target;
	if (m_commandService) {
		Command::PhoneUpdateContactParam param;
		param.setAddBookID(m_contact.addBookId());
		param.setUpdateType(UpdateContactOptions::updateLanguage_);
		param.setSourceLanguage(source);
		param.setTargetLanguage(target);
		m_commandService->phoneupdatecontact(param);
	}
}

void PhoneContactView::retranslateTr()
{
	m_ui->labelSourceLanguage->setText(qApp->languageFull(m_contact.sourceLanguage()));
	m_ui->labelTargetLanguage->setText(qApp->languageFull(m_contact.targetLanguage()));
}

} // namespace Qpid
