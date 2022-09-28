#include <QDebug>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegExpValidator>
#include "phonemakecallview.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_phonemakecallview.h"
#include "sipservice.h"
#include "callinfo.h"
#include "interpretationselect.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneMakeCallView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneMakeCallViewOptions
{
	static const int max_number_length_;
};

const int PhoneMakeCallViewOptions::max_number_length_ = 30;

PhoneMakeCallView::PhoneMakeCallView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneMakeCallView),
    m_opt(new PhoneMakeCallViewOptions),
	m_sourceLanguage(),
	m_targetLanguage()
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->frameInterpreterLanguage->hide();

	m_ui->comboBoxCountryCode->setIconSize(QSize(36, 19));
	m_ui->comboBoxNumber->lineEdit()->setMaxLength(PhoneMakeCallViewOptions::max_number_length_);
	m_ui->comboBoxNumber->lineEdit()->setValidator(new QRegExpValidator(QRegExp(QLatin1String("[0-9*#]+$"))));

	connect(m_ui->widgetDTMFPlate, SIGNAL(DTMFClicked(const QChar&)), this, SLOT(slotAppendDigit(const QChar&)));

	m_interpretationSelect = new InterpretationSelect(m_ui->frameMainPanel);
	connect(m_interpretationSelect, SIGNAL(interpreterAdded(const QString&, const QString&)), this, SLOT(slotInterpreterAdded(const QString&, const QString&)));

	m_ui->frameMainPanel->adjustSize();
	QRect rect = m_ui->widgetDTMFPlate->geometry();
	m_ui->widgetDTMFPlate->setDTMFSize(QSize(rect.width()/3-3, rect.height()/4-1));
}

PhoneMakeCallView::~PhoneMakeCallView()
{

}

SipService* PhoneMakeCallView::sipService() const
{
	return m_sipService;
}

void PhoneMakeCallView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;

	if (m_sipService) {}
}

// Call* PhoneMakeCallView::currentCall() const
// {
// 	return m_currentCall;
// }
// 
// void PhoneMakeCallView::setCurrentCall(Call* call)
// {
// 	m_currentCall = call;
// }

void PhoneMakeCallView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneMakeCallView::signOut()
{
	m_ui->comboBoxCountryCode->clear();
	m_ui->comboBoxNumber->clear();
	m_interpretationSelect->close();

	m_ui->comboBoxNumber->clear();

	on_pushButtonRemove_clicked();

	AbstractPhoneView::signOut();
}

void PhoneMakeCallView::setPSTNSupported(const QStringList& supported)
{
	m_ui->comboBoxCountryCode->clear();

	foreach(const QString& s, supported) {
		QStringList country = s.split(QLatin1Char('='));
		if (country.size() != 2)
			continue;
		m_ui->comboBoxCountryCode->addItem(QIcon(QString(QLatin1String(":/images/ui/flag/%1.png")).arg(country[0].trimmed())), 
			QString(QLatin1String("+%1")).arg(country[1].trimmed()), 
			country[0].trimmed());
	}
}

void PhoneMakeCallView::setInterpretationVisible(bool visible)
{
	if (!m_interpretationSelect)
		return;

	if (visible) {
		m_interpretationSelect->initialSourceLanguage();
		m_interpretationSelect->show();
	} else {
		m_interpretationSelect->reject();
	}
	
}

void PhoneMakeCallView::changeEvent(QEvent* event)
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

void PhoneMakeCallView::keyPressEvent(QKeyEvent *event)
{
	AbstractPhoneView::keyPressEvent(event);

	if (event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter) {
			on_pushButtonMakeCall_clicked();
	}
}

void PhoneMakeCallView::on_pushButtonDelete_clicked()
{
	QString number = m_ui->comboBoxNumber->currentText();
	if (!number.isEmpty())
		number = number.left(number.length()-1);
	m_ui->comboBoxNumber->	setEditText(number);
}

void PhoneMakeCallView::on_pushButtonAdd_clicked()
{
	m_interpretationSelect->initialSourceLanguage();
	m_interpretationSelect->show();
}

void PhoneMakeCallView::on_pushButtonChange_clicked()
{
	on_pushButtonAdd_clicked();
}

void PhoneMakeCallView::on_pushButtonRemove_clicked()
{
	m_ui->frameInterpreterLanguage->hide();
	m_ui->frameLanguagePanel->show();

	m_sourceLanguage = QString();
	m_targetLanguage = QString();
}

void PhoneMakeCallView::on_pushButtonMakeCall_clicked()
{
	if (m_sipService && qApp->call()->state()==Call::CallClosed) {
		QString number = m_ui->comboBoxNumber->currentText().trimmed();
		if (number.isEmpty()) {
			qWarning() << QPID_DEBUG_PREFIX << "Please enter the destination number before clicking the call";
			return;
		}

		int exist = m_ui->comboBoxNumber->findText(number);
		if (exist != -1)
			m_ui->comboBoxNumber->removeItem(exist);
		m_ui->comboBoxNumber->insertItem(0, number);
		m_ui->comboBoxNumber->setCurrentIndex(0);

		number.prepend(m_ui->comboBoxCountryCode->currentText());
		number.replace(QLatin1String("+"), QLatin1String("00"));
		qApp->call()->setServiceMode(m_ui->frameInterpreterLanguage->isVisible()?ServiceModeOptions::PSTNFree_:ServiceModeOptions::PSTNDirect_);
		qApp->call()->setCalleeNumber(number);
		qApp->call()->setCallState(Call::CallingPreOut);
		int callID = m_sipService->makeCall(
			qApp->SIPAccountID(), 
			qApp->makeDestinationUrl(number, m_ui->frameInterpreterLanguage->isVisible()), 
			m_sourceLanguage, 
			m_targetLanguage, 
			(m_ui->frameInterpreterLanguage->isVisible()?ServiceModeOptions::PSTNFree_:ServiceModeOptions::PSTNDirect_), 
			m_ui->comboBoxCountryCode->itemData(m_ui->comboBoxCountryCode->currentIndex()).toString());
		qApp->call()->setCallID(callID);
		if (callID == -1)
			qApp->call()->setCallState(Call::CallClosed);
	}
}

void PhoneMakeCallView::slotInterpreterAdded(const QString& source, const QString& target)
{
	m_ui->frameInterpreterLanguage->show();
	m_ui->frameLanguagePanel->hide();

	m_sourceLanguage = source;
	m_targetLanguage = target;

	m_ui->labelSourceLanguage->setText(qApp->languageFull(m_sourceLanguage));
	m_ui->labelTargetLanguage->setText(qApp->languageFull(m_targetLanguage));
}

void PhoneMakeCallView::slotAppendDigit(const QChar& c)
{
	QString number = m_ui->comboBoxNumber->currentText();
	number.append(c);
	m_ui->comboBoxNumber->setEditText(number);
}

void PhoneMakeCallView::retranslateTr()
{
	m_ui->labelSourceLanguage->setText(qApp->languageFull(m_sourceLanguage));
	m_ui->labelTargetLanguage->setText(qApp->languageFull(m_targetLanguage));
}

} // namespace Qpid
