#include "viewprofile.h"
#include "ui_viewprofile.h"
#include "phoneapplication.h"

namespace ns {

ViewProfile::ViewProfile(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ViewProfile)
{
    m_ui->setupUi(this);
	m_ui->labelPhoto->setRadius(3.0, 3.0);
	m_ui->pushButtonEdit->hide();
}

ViewProfile::~ViewProfile()
{
	 
}

void ViewProfile::setDisplayName(const QString& name, const QString& Id)
{
	m_displayName = name;
	m_loginId = Id;

#if 0
	QString displayName = tr("%1(ID:%2)").arg(name, Id);
	m_ui->labelDisplay->setText(displayName);
#endif
	QString displayName = name;
	m_ui->labelDisplay->setText(displayName);
	QString displayId = tr("(ID:%1)").arg(Id);
	m_ui->labelDisplayID->setText(displayId);
}

void ViewProfile::setPhotoUrl(const QString& url)
{
	m_photoUrl = url;
}

void ViewProfile::setPhoto(const QString& url, const QString& photo)
{
	if (url != m_photoUrl)
		return;

	QPixmap pic(photo);
	QPixmap mask  = QLatin1String(":/images/ui/view_bg_round_corner.png");
	m_ui->labelPhoto->setMaskPixmap(pic.scaled(m_ui->labelPhoto->size()), mask);
	//m_ui->labelPhoto->setPixmap(pic.scaled(m_ui->labelPhoto->size()));
}

void ViewProfile::setIntroduction(const QString& intro)
{
	m_ui->textEditIntroduction->setText(intro);
}

void ViewProfile::setLanguage(const QString& source, const QString& target)
{
	m_ui->widget->setLanguage(source, target);
}

void ViewProfile::setServiceMode(const QString& mode)
{
	m_ui->widget->setServiceMode(mode);
}

void ViewProfile::changeEvent(QEvent * event)
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

void ViewProfile::retranslateTr()
{
	m_ui->labelDisplay->setText(m_displayName);
	QString displayId = tr("(ID:%1)").arg(m_loginId);
	m_ui->labelDisplayID->setText(displayId);
}

} // namespace ns
