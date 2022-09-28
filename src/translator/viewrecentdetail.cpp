#include <QDatetime>
#include <QTime>
#include "viewrecentdetail.h"
#include "ui_viewrecentdetail.h"
#include "phoneapplication.h"
#include "tonegoglobal.h"

namespace ns {

ViewRecentDetail::ViewRecentDetail(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ViewRecentDetail)
{
    m_ui->setupUi(this);

	m_ui->labelCallerPhoto->setRadius(3.0, 3.0);
	m_ui->labelCalleePhoto->setRadius(3.0, 3.0);
	m_ui->labelMePhoto->setRadius(3.0, 3.0);
}

ViewRecentDetail::~ViewRecentDetail()
{
	 
}

void ViewRecentDetail::setCallerInfo(const QString& displayName, const QString& mobile, const QString& photoUrl)
{
	m_callerDisplayName = displayName;
	m_callerMobile = mobile;

	m_ui->labelCallerName->setText(displayName);
	m_ui->labelCallerInfo->setText(tr("Mobile: %1").arg(mobile));
	m_callerPhotoUrl = photoUrl;
}

void ViewRecentDetail::setCallerPhoto(const QString& url, const QString& photo)
{
	if (m_callerPhotoUrl==url) {
		QPixmap pic(photo);
		QPixmap mask  = QLatin1String(":/images/ui/view_bg_round_corner.png");
		m_ui->labelCallerPhoto->setMaskPixmap(pic.scaled(m_ui->labelCallerPhoto->size()),mask);
		//m_ui->labelCallerPhoto->setPixmap(pic.scaled(m_ui->labelCallerPhoto->size()));
	}
}

void ViewRecentDetail::setPartnerInfo(const QString& photoUrl)
{
	m_partnerPhotoUrl = photoUrl;
}

void ViewRecentDetail::setPartnerPhoto(const QString& url, const QString& photo)
{
	if (url == m_partnerPhotoUrl) {
		QPixmap pic(photo);
		QPixmap mask = QLatin1String(":/images/ui/view_bg_round_corner_me.png");
		m_ui->labelMePhoto->setMaskPixmap(pic.scaled(m_ui->labelMePhoto->size()), mask);
	}
}

void ViewRecentDetail::setCalleeInfo(const QString& displayName, const QString& mobile, const QString& photoUrl)
{
	m_calleeDisplayName = displayName;
	m_calleeMobile = mobile;

	m_ui->labelCalleeName->setText(displayName);
	m_ui->labelCalleeInfo->setText(tr("Mobile: %1").arg(mobile));
	m_calleePhotoUrl = photoUrl;
}

void ViewRecentDetail::setCalleePhoto(const QString& url, const QString& photo)
{
	if (m_calleePhotoUrl==url) {
		QPixmap pic(photo);
		QPixmap mask  = QLatin1String(":/images/ui/view_bg_round_corner.png");
		m_ui->labelCalleePhoto->setMaskPixmap(pic.scaled(m_ui->labelCalleePhoto->size()), mask);
		//m_ui->labelCalleePhoto->setPixmap(pic.scaled(m_ui->labelCalleePhoto->size()));
	}
}

void ViewRecentDetail::setCallDetail(const QString& dateTime, const QString& Id, const QString& duration, const QString& mode)
{
	m_datetime = dateTime;
	m_id = Id;
	m_duration = duration;
	m_mode = mode;

	QDateTime dt = QDateTime::fromTime_t(dateTime.toInt());
	m_ui->labelCallDatetimeValue->setText(dt.toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));

	m_ui->labelCallIDValue->setText(Id);

	QString callDuration;
	int dur = duration.toInt();
	const QTime currentTime((dur / 3600) % 60, (dur / 60) % 60, dur % 60);
	callDuration += QString::number(currentTime.hour()*60+currentTime.minute());
	callDuration += QLatin1Char('\'');
	callDuration += tr("%1").arg(currentTime.second(), 2, 10, QLatin1Char('0'));
	callDuration += QLatin1Char('\"');
	m_ui->labelCallDurationValue->setText(callDuration);

	m_ui->labelCallModeValue->setText(mode);

	if (YesPo::callMode(mode) ==  YesPo::cmTwoWay) {
		m_ui->widgetCallee->hide();
		m_ui->labelCallModeValue->setText(tr("Two way"));
	} else {
		m_ui->widgetCallee->show();
		m_ui->labelCallModeValue->setText(tr("Three way"));
	}

	m_ui->widgetTranslationLanguage->setServiceMode(mode);
}

void ViewRecentDetail::setLanguage(const QString& srcLang, const QString& targetLang)
{
	m_ui->widgetTranslationLanguage->setLanguage(srcLang, targetLang);
}

void ViewRecentDetail::changeEvent(QEvent * event)
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

void ViewRecentDetail::retranslateTr()
{
	m_ui->labelCallerName->setText(m_callerDisplayName);
	m_ui->labelCallerInfo->setText(tr("Mobile: %1").arg(m_callerMobile));

	m_ui->labelCalleeName->setText(m_calleeDisplayName);
	m_ui->labelCalleeInfo->setText(tr("Mobile: %1").arg(m_calleeMobile));

	QDateTime dt = QDateTime::fromTime_t(m_datetime.toInt());
	m_ui->labelCallDatetimeValue->setText(dt.toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));

	m_ui->labelCallIDValue->setText(m_id);

	QString callDuration;
	int dur = m_duration.toInt();
	const QTime currentTime((dur / 3600) % 60, (dur / 60) % 60, dur % 60);
	callDuration += QString::number(currentTime.hour()*60+currentTime.minute());
	callDuration += QLatin1Char('\'');
	callDuration += tr("%1").arg(currentTime.second(), 2, 10, QLatin1Char('0'));
	callDuration += QLatin1Char('\"');
	m_ui->labelCallDurationValue->setText(callDuration);

	m_ui->labelCallModeValue->setText(m_mode);

	if (YesPo::callMode(m_mode) ==  YesPo::cmTwoWay) {
		m_ui->labelCallModeValue->setText(tr("Two way"));
	} else {
		m_ui->labelCallModeValue->setText(tr("Three way"));
	}

}

} // namespace ns
