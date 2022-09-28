#include <QDebug>
#include "popupdirectcall.h"
#include "phoneapplication.h"
#include "phoneglobal.h"
#include "ui_popupdirectcall.h"


#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PopupDirectCall:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PopupDirectCallOptions
{

};

PopupDirectCall::PopupDirectCall(QWidget* parent) :
    FloatPanel(parent),
    m_ui(new Ui_PopupDirectCall),
    m_opt(new PopupDirectCallOptions),
	m_editNumber()
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->comboBoxCountry->setIconSize(QSize(19, 19));
	m_ui->textBrowserFreeLink->insertHtml(tr("<a href=\"#\">What is free interpretation service?</a>"));

	connect(m_ui->pushButtonVoiceraID, SIGNAL(clicked()), this, SLOT(slotClickToCall()));
	connect(m_ui->pushButtonNumber_1, SIGNAL(clicked()), this, SLOT(slotClickToCall()));
	connect(m_ui->pushButtonNumber_2, SIGNAL(clicked()), this, SLOT(slotClickToCall()));
	connect(m_ui->pushButtonNumber_3, SIGNAL(clicked()), this, SLOT(slotClickToCall()));
	connect(m_ui->pushButtonNumber_4, SIGNAL(clicked()), this, SLOT(slotClickToCall()));
	connect(m_ui->pushButtonNumber_5, SIGNAL(clicked()), this, SLOT(slotClickToCall()));

	m_ui->frameNumberPanel_1->hide();
	m_ui->frameNumberPanel_2->hide();
	m_ui->frameNumberPanel_3->hide();
	m_ui->frameNumberPanel_4->hide();
	m_ui->frameNumberPanel_5->hide();

	autoResize();
}

PopupDirectCall::~PopupDirectCall()
{

}

void PopupDirectCall::addNumber(const QString& number)
{
	m_listNumbers = number.split(AppOptions::phonesSep_);

	int index = 1;
	foreach(const QString& number, m_listNumbers) {

		QStringList tempNumber = number.split(AppOptions::countryNumberSep_);
		if (tempNumber.size() != 2)
			continue;

		QString display = tempNumber[1];
		display.replace(0, 2, QLatin1String("+"));
		display.prepend(tr("Call phone "));

		switch(index) {
		case 1:
			m_ui->pushButtonNumber_1->setText(display);
			m_ui->frameNumberPanel_1->show();
			break;
		case 2:
			m_ui->pushButtonNumber_2->setText(display);
			m_ui->frameNumberPanel_2->show();
			break;
		case 3:
			m_ui->pushButtonNumber_3->setText(display);
			m_ui->frameNumberPanel_3->show();
			break;
		case 4:
			m_ui->pushButtonNumber_4->setText(display);
			m_ui->frameNumberPanel_4->show();
			break;
		case 5:
			m_ui->pushButtonNumber_5->setText(display);
			m_ui->frameNumberPanel_5->show();
			break;
		}
		++ index;
	}

	if (index > 5)
		m_ui->frameAddNumber->hide();
	else
		m_ui->frameAddNumber->show();

	autoResize();
	hide();
}

void PopupDirectCall::clearNumber()
{
	m_ui->frameNumberPanel_1->hide();
	m_ui->frameNumberPanel_2->hide();
	m_ui->frameNumberPanel_3->hide();
	m_ui->frameNumberPanel_4->hide();
	m_ui->frameNumberPanel_5->hide();
	m_listNumbers.clear();

	autoResize();
}

void PopupDirectCall::showVoicera(bool show)
{
	m_ui->frameVoiceraID->setVisible(show);

	autoResize();
}

void PopupDirectCall::showInterpreterOption(bool show)
{
	if (m_sourceLanguage==QLatin1String("-") || m_targetLanguage==QLatin1String("-")) {
		m_ui->widgetInterpretionPanel->hide();
	} else {
		m_ui->widgetInterpretionPanel->setVisible(show);
	}
	m_ui->widgetDisplay->show();
	m_ui->widgetEdit->hide();
	autoResize();
}

void PopupDirectCall::setPSTNSupported(const QStringList& supported)
{
	m_ui->comboBoxCountry->clear();

	foreach(const QString& s, supported) {
		QStringList country = s.split(QLatin1Char('='));
		if (country.size() != 2)
			continue;
		m_ui->comboBoxCountry->addItem(QIcon(QString(QLatin1String(":/images/ui/flag/%1.png")).arg(country[0].trimmed())), 
			QString(QLatin1String("+%1")).arg(country[1].trimmed()), 
			country[0].trimmed());
	}
}

void PopupDirectCall::changeEvent(QEvent* event)
{
	FloatPanel::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}


void PopupDirectCall::setInterpretation(const QString& sourceLanguage, const QString& targetLanguage)
{
	m_sourceLanguage = sourceLanguage;
	m_targetLanguage = targetLanguage;

	m_ui->labelSourceLanguage->setText(m_sourceLanguage);
	m_ui->labelTargetLanguage->setText(m_targetLanguage);
}

void PopupDirectCall::on_pushButtonAddNumber_clicked()
{
	m_editNumber = QString();
	updateDisplay();
}

void PopupDirectCall::on_pushButtonEdit_1_clicked()
{
	if (m_listNumbers.size() > 0) {
		m_editNumber = m_listNumbers.at(0);
		updateDisplay();
	}
}

void PopupDirectCall::on_pushButtonEdit_2_clicked()
{
	if (m_listNumbers.size() > 1) {
		m_editNumber = m_listNumbers.at(1);
		updateDisplay();
	}
}

void PopupDirectCall::on_pushButtonEdit_3_clicked()
{
	if (m_listNumbers.size() > 2) {
		m_editNumber = m_listNumbers.at(2);
		updateDisplay();
	}
}

void PopupDirectCall::on_pushButtonEdit_4_clicked()
{
	if (m_listNumbers.size() > 3) {
		m_editNumber = m_listNumbers.at(3);
		updateDisplay();
	}
}

void PopupDirectCall::on_pushButtonEdit_5_clicked()
{
	if (m_listNumbers.size() > 4) {
		m_editNumber = m_listNumbers.at(4);
		updateDisplay();
	}
}

void PopupDirectCall::on_pushButtonOK_clicked()
{
	QString oldPhone;
	QString phoneCountry;
	QString phone;
	phoneCountry = m_ui->comboBoxCountry->itemData(m_ui->comboBoxCountry->currentIndex()).toString();
	phone = m_ui->lineEditNumber->text().trimmed();
	if (phone.isEmpty())
		return;
	
	phone.prepend(qApp->countryCode(phoneCountry));
	phone.prepend(QLatin1String("00"));

	if (m_listNumbers.contains(phoneCountry + AppOptions::countryNumberSep_ + phone)) {
		m_ui->labelErrorMsg->setText(tr("The number you added already exists"));
		m_ui->labelErrorMsg->show();
		autoResize();
		return;
	}

	if (!m_editNumber.isEmpty()) {
		QStringList oldNumber = m_editNumber.split(AppOptions::countryNumberSep_);
		if (oldNumber.size()==2)
			oldPhone = oldNumber[1];
	}

	Q_EMIT editNumber(oldPhone, phoneCountry, phone);

}

void PopupDirectCall::on_pushButtonCancel_clicked()
{
	m_ui->widgetDisplay->show();
	m_ui->widgetEdit->hide();
	autoResize();
}

void PopupDirectCall::on_pushButtonRemove_clicked()
{
	QString oldPhone;

	if (!m_editNumber.isEmpty()) {
		QStringList oldNumber = m_editNumber.split(AppOptions::countryNumberSep_);
		if (oldNumber.size()==2)
			oldPhone = oldNumber[1];
	}

	Q_EMIT removeNumber(oldPhone);
}

void PopupDirectCall::on_textBrowserFreeLink_anchorClicked(const QUrl & link)
{
//	http://192.168.8.245/http/index/userfaqs#q4

	qDebug() << QPID_DEBUG_PREFIX << link;
}

void PopupDirectCall::slotClickToCall()
{
	QString number;
	QPushButton* buttonClicked = qobject_cast<QPushButton*>(sender());
	if (buttonClicked) {
		
		if (buttonClicked == m_ui->pushButtonNumber_1) {
			number = m_listNumbers.at(0);
		} else if (buttonClicked == m_ui->pushButtonNumber_2) {
			number = m_listNumbers.at(1);
		} else if (buttonClicked == m_ui->pushButtonNumber_3) {
			number = m_listNumbers.at(2);
		} else if (buttonClicked == m_ui->pushButtonNumber_4) {
			number = m_listNumbers.at(3);
		} else if (buttonClicked == m_ui->pushButtonNumber_5) {
			number = m_listNumbers.at(4);
		}
	}

	Q_EMIT callNumber(number, m_ui->widgetInterpretionPanel->isVisible());

	hide();
}

void PopupDirectCall::autoResize()
{
	m_ui->widgetDisplay->adjustSize();
	m_ui->widgetEditPanel->adjustSize();
	m_ui->widgetEdit->adjustSize();
	m_ui->widget->adjustSize();
	adjustSize();
}

void PopupDirectCall::updateDisplay()
{
	m_ui->widgetDisplay->hide();
	m_ui->widgetEdit->show();

	if (!m_editNumber.isEmpty()) {

		QStringList oldNumber = m_editNumber.split(AppOptions::countryNumberSep_);
		QString oldPhoneCountry = oldNumber[0];
		QString oldPhone = oldNumber[1];

		int index = m_ui->comboBoxCountry->findData(oldPhoneCountry);
		m_ui->comboBoxCountry->setCurrentIndex(index);

		QString display = oldPhone;
		QString countryCode = qApp->countryCode(oldPhoneCountry);
		countryCode.prepend(QLatin1String("00"));
		display.replace(0, countryCode.length(), QString());
		m_ui->lineEditNumber->setText(display);

		m_ui->pushButtonRemove->show();

	} else {
		m_ui->lineEditNumber->setText(QString());

		m_ui->pushButtonRemove->hide();
	}

	m_ui->labelErrorMsg->hide();

	autoResize();
}

void PopupDirectCall::retranslateTr()
{
	int index = 1;
	foreach(const QString& number, m_listNumbers) {

		QStringList tempNumber = number.split(AppOptions::countryNumberSep_);
		if (tempNumber.size() != 2)
			continue;

		QString display = tempNumber[1];
		display.replace(0, 2, QLatin1String("+"));
		display.prepend(tr("Call phone "));

		switch(index) {
		case 1:
			m_ui->pushButtonNumber_1->setText(display);
			m_ui->frameNumberPanel_1->show();
			break;
		case 2:
			m_ui->pushButtonNumber_2->setText(display);
			m_ui->frameNumberPanel_2->show();
			break;
		case 3:
			m_ui->pushButtonNumber_3->setText(display);
			m_ui->frameNumberPanel_3->show();
			break;
		case 4:
			m_ui->pushButtonNumber_4->setText(display);
			m_ui->frameNumberPanel_4->show();
			break;
		case 5:
			m_ui->pushButtonNumber_5->setText(display);
			m_ui->frameNumberPanel_5->show();
			break;
		}
		++ index;
	}

	m_ui->labelSourceLanguage->setText(m_sourceLanguage);
	m_ui->labelTargetLanguage->setText(m_targetLanguage);
}

} // namespace Qpid
