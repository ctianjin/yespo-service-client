#include "contactprofile.h"
#include "phoneapplication.h"
#include "ui_contactprofile.h"
#include "popupdirectcall.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "ContactProfile:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct ContactProfileOptions
{

};

ContactProfile::ContactProfile(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ContactProfile),
	m_opt(new ContactProfileOptions),
	m_popupDirectCall(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_popupDirectCall = new PopupDirectCall(this);
	m_ui->pushButtonDirectCallSelection->setPopupWindow(m_popupDirectCall);
	m_ui->pushButtonInterpreterCallSelection->setPopupWindow(m_popupDirectCall);

	connect(m_popupDirectCall, SIGNAL(callNumber(const QString&, bool)), this, SIGNAL(callNumber(const QString&, bool)));
	connect(m_popupDirectCall, SIGNAL(editNumber(const QString&, const QString&, const QString&)), 
		this, SIGNAL(editNumber(const QString&, const QString&, const QString&)));
	connect(m_popupDirectCall, SIGNAL(removeNumber(const QString&)), this, SIGNAL(removeNumber(const QString&)));
	connect(m_popupDirectCall, SIGNAL(floatHide()), m_ui->pushButtonDirectCallSelection, SLOT(update()));
	connect(m_popupDirectCall, SIGNAL(floatHide()), m_ui->pushButtonInterpreterCallSelection, SLOT(update()));
}

ContactProfile::~ContactProfile()
{

}

void ContactProfile::setPhoto(const QString& photoFile)
{
	if (!photoFile.isEmpty()) {
		m_ui->labelPhoto->setPixmap(QPixmap(photoFile));
	} else {
		m_ui->labelPhoto->setPixmap(QPixmap(QLatin1String(":/images/ui/default_photo.jpg")));
	}
}

void ContactProfile::setDisplayName(const QString& name)
{
	m_ui->lineEditContactName->setText(name);
}

void ContactProfile::setOnlineStatus(const QString& status)
{
	if (status == QLatin1String("2")) {
		m_ui->labelStatusIcon->setPixmap(QPixmap(QLatin1String(":/images/ui/online_status_offline.png")));
		m_ui->lineEditStatusText->setText(tr("Offline"));
	} else {
		m_ui->labelStatusIcon->setPixmap(QPixmap(QLatin1String(":/images/ui/online_status_online.png")));
		m_ui->lineEditStatusText->setText(tr("Online"));
	}
}

void ContactProfile::setVoiceraID(const QString& ID)
{
	m_popupDirectCall->showVoicera(!ID.isEmpty());
}

void ContactProfile::setPhoneNumbers(const QString& number)
{
	m_popupDirectCall->clearNumber();
	m_popupDirectCall->addNumber(number);
}

void ContactProfile::setInterpretation(const QString& sourceLanguage, const QString& targetLanguage)
{
	m_popupDirectCall->setInterpretation(sourceLanguage, targetLanguage);
}

void ContactProfile::setPSTNSupported(const QStringList& supported)
{
	if (m_popupDirectCall)
		m_popupDirectCall->setPSTNSupported(supported);
}

void ContactProfile::setSimpleShow(bool show)
{
	m_ui->frameSimpleCallPanel->setVisible(show);
	m_ui->frameCallPanel->setVisible(!show);
	m_ui->labelStatusIcon->setVisible(!show);
	m_ui->lineEditStatusText->setVisible(!show);
	m_ui->pushButtonAddContact->setVisible(show);
}

void ContactProfile::setAddToContact(bool show)
{
	m_ui->pushButtonAddContact->setVisible(show);
}

void ContactProfile::changeEvent(QEvent* event)
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

void ContactProfile::on_pushButtonDirectCall_clicked()
{
	Q_EMIT callNumber(QString(), false);
}

void ContactProfile::on_pushButtonInterpreterCall_clicked()
{
	Q_EMIT callNumber(QString(), true);
}

void ContactProfile::on_pushButtonDirectCallSelection_clicked()
{
	if (m_popupDirectCall) {
		m_popupDirectCall->showInterpreterOption(false);
	}
}

void ContactProfile::on_pushButtonInterpreterCallSelection_clicked()
{
	if (m_popupDirectCall) {
		m_popupDirectCall->showInterpreterOption(true);
	}
}

void ContactProfile::on_pushButtonSimpleDirectCall_clicked()
{
	Q_EMIT simpleCall();
}

void ContactProfile::on_pushButtonAddContact_clicked()
{
	Q_EMIT addToContact();
}

void ContactProfile::retranslateTr()
{

}

} // namespace Qpid
