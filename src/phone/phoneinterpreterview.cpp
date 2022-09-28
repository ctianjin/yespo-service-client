#include <QDebug>
#include <QKeyEvent>
#include "phoneinterpreterview.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_phoneinterpreterview.h"
#include "sipservice.h"
#include "callinfo.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneInterpreterView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneInterpreterViewOptions
{

};

PhoneInterpreterView::PhoneInterpreterView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneInterpreterView),
    m_opt(new PhoneInterpreterViewOptions)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

}

PhoneInterpreterView::~PhoneInterpreterView()
{

}

SipService* PhoneInterpreterView::sipService() const
{
	return m_sipService;
}

void PhoneInterpreterView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

void PhoneInterpreterView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneInterpreterView::signOut()
{
	AbstractPhoneView::signOut();
}

void PhoneInterpreterView::initialSourceLanguage()
{
	m_ui->comboBoxSourceLanguage->clear();
	m_ui->comboBoxSourceLanguage->addItem(tr("- Select -"), QVariant());

	QStringList sourceLanguages = qApp->supportedSource();
	foreach(const QString& l, sourceLanguages) {
		m_ui->comboBoxSourceLanguage->addItem(qApp->languageFull(l), l);
	}

	m_ui->widgetCallHelp->setDisplayText(tr("Are you ready to call?"), 
		tr("Select the languages for interpretation and hit call"),
		tr("Call answered by a human-based interpreter"),
		tr("Turn the speaker on and share the sound with the third party"));
}

void PhoneInterpreterView::changeEvent(QEvent* event)
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

void PhoneInterpreterView::keyPressEvent(QKeyEvent *event)
{
	AbstractPhoneView::keyPressEvent(event);

	if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {
		on_pushButtonCall_clicked();
	}
}

void PhoneInterpreterView::on_comboBoxSourceLanguage_currentIndexChanged(int index)
{
	m_ui->comboBoxTargetLanguage->clear();
	m_ui->comboBoxTargetLanguage->addItem(tr("- Select -"), QVariant());

	QStringList targetLanguages = qApp->supportedTarget(m_ui->comboBoxSourceLanguage->itemData(index).toString());
	foreach(const QString& l, targetLanguages) {
		m_ui->comboBoxTargetLanguage->addItem(qApp->languageFull(l), l);
	}
}

void PhoneInterpreterView::on_pushButtonCall_clicked()
{
	QString sourceLanguage = m_ui->comboBoxSourceLanguage->itemData(m_ui->comboBoxSourceLanguage->currentIndex()).toString();
	QString targetLanguage = m_ui->comboBoxTargetLanguage->itemData(m_ui->comboBoxTargetLanguage->currentIndex()).toString();
	if (sourceLanguage.isEmpty() || targetLanguage.isEmpty()) {
		qDebug() << QPID_DEBUG_PREFIX << "You must select the service language before making a call!";
		return;
	}

	if (m_sipService && qApp->call()->state()==Call::CallClosed) {
		qApp->call()->setServiceMode(ServiceModeOptions::centerFree_);
		qApp->call()->setCalleeNumber(QString());
		qApp->call()->setCallState(Call::CallingPreOut);
		int callID = m_sipService->makeCall(qApp->SIPAccountID(), qApp->makeDestinationUrl(QString(),  true), 
			sourceLanguage, targetLanguage, ServiceModeOptions::centerFree_, QString());
		qApp->call()->setCallID(callID);
		if (callID == -1)
			qApp->call()->setCallState(Call::CallClosed);
	}
}

void PhoneInterpreterView::retranslateTr()
{

}

} // namespace Qpid
