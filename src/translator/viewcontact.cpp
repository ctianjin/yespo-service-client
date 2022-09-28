#include <QDebug>
#include "viewcontact.h"
#include "ui_viewcontact.h"
#include "phoneapplication.h"
#include "commandservice.h"
#include "tonegoglobal.h"

namespace ns {

ViewContact::ViewContact(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ViewContact),
	m_VESysId()
{
    m_ui->setupUi(this);
}

ViewContact::~ViewContact()
{
	 
}

CommandService* ViewContact::commandService() const
{
	return m_commandService;
}

void ViewContact::setCommandService(CommandService* commandService)
{
	if (m_commandService) {

	}
	
	m_commandService = commandService;

	if (m_commandService) {

	}
}

void ViewContact::setVESysID(const QString& Id)
{
	m_VESysId = Id;
}

void ViewContact::setMemberInfo(const QString& displayName, const QString& language, const QString& nation, const QString& carrer)
{
	m_displayName = displayName;
	m_language = language;
	m_nation = nation;
	m_major = carrer;

	m_ui->labelUserName->setText(m_displayName);
	m_ui->labeluserNameValue->setText(m_displayName);
	m_ui->labelLanguageValue->setText(qApp->languageFull(m_language));
	m_ui->labelNationValue->setText(m_nation);
	m_ui->labelMajorValue->setText(YesPo::majorText(m_major));
}

void ViewContact::setPhotoFile(const QString& phoFile)
{
	QPixmap pic(phoFile);
	QPixmap mask  = QLatin1String(":/images/ui/view_bg_round_corner.png");
	m_ui->labelPho->setMaskPixmap(pic.scaled(m_ui->labelPho->size()), mask);
	//m_ui->labelPho->setPixmap(pic.scaled(m_ui->labelPho->size(), Qt::IgnoreAspectRatio));
}

void ViewContact::changeEvent(QEvent * event)
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

void ViewContact::on_pushButtonBlacklist_clicked()
{
	if (m_commandService) {
		Command::AddBlacklistParam param;
		param.setVESysId(m_VESysId);
		m_commandService->addblacklist(param);
	}
}

void ViewContact::retranslateTr()
{
	m_ui->labelUserName->setText(m_displayName);
	m_ui->labeluserNameValue->setText(m_displayName);
	m_ui->labelLanguageValue->setText(qApp->languageFull(m_language));
	m_ui->labelNationValue->setText(m_nation);
	m_ui->labelMajorValue->setText(YesPo::majorText(m_major));
}

} // namespace ns
