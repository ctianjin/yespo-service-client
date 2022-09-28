#include "skillchannel.h"
#include "ui_skillchannel.h"
#include "phoneapplication.h"
#include <QMovie>
namespace ns {

const QString SkillChannel::m_idleStyle = QLatin1String("#widgetBottom { border-bottom: 1px solid #E4E4E4;} #labelMovie { border-image: url(:/images/ui/waiting.gif);}");
const QString SkillChannel::m_waitingStyle = QLatin1String("color: #000000");
const QString SkillChannel::m_ringingStyle = QLatin1String("color: #FF6600");
const QString SkillChannel::m_confirmingStyle = QLatin1String("color: #339900");

SkillChannel::SkillChannel(QWidget* parent) :
    QWidget(parent),
	m_ui(new Ui_SkillChannel),
	m_recentCallStatus(RecentStatusDisconnect),
	m_online(true),
	m_localLanguage(),
	m_country(),
	m_sourceLang(),
	m_targetLang()
{
	m_ui->setupUi(this);

	m_ui->labelNoService->hide();

	m_movieCalling = new QMovie(QLatin1String(":/images/ui/calling.gif"));
	m_movieConfirming = new QMovie(QLatin1String("images/ui/confirming.gif"));
}

SkillChannel::SkillChannel(QWidget* parent /* = 0 */, bool empty) :
	QWidget(parent),
	m_ui(new Ui_SkillChannel),
	m_empty(empty)
{
	m_ui->setupUi(this);

	setStyleSheet(m_idleStyle);

	if (m_empty) {
		m_ui->widgetLanguage->hide();
		m_ui->widgetState->hide();
	}
}

SkillChannel::~SkillChannel()
{

}

void SkillChannel::changeStatus(bool online)
{
	m_online = online;
	if(online) {
		m_ui->labelMovie->show();
		m_ui->labelState->setStyleSheet(m_waitingStyle);
		m_ui->labelState->setText(tr("Waiting"));
	} else {
		m_ui->labelMovie->hide();
		m_ui->labelState->setStyleSheet(QLatin1String("color: #FF3300"));
		m_ui->labelState->setText(tr("Stop"));
	}
}

void SkillChannel::setTrasnlationLanguage(const QString &source, const QString& target)
{
	m_local = false;
	m_ui->labelLocalTarget->hide();
	m_sourceLang = source;
	m_targetLang = target;
	setStyleSheet(m_idleStyle);
	m_ui->labelSource->setText(qApp->languageFull(m_sourceLang));
	m_ui->labelTarget->setText(qApp->languageFull(m_targetLang));
	m_ui->labelSplitter->show();
	m_ui->labelSource->show();
	m_ui->labelTarget->show();
}

void SkillChannel::setLocalLanguage(const QString& country, const QString& language)
{
	m_local = true;
	m_ui->labelSplitter->hide();
	m_ui->labelSource->hide();
	m_ui->labelTarget->hide();

	m_country = country;
	m_localLanguage = language;

	setStyleSheet(m_idleStyle);
	m_ui->labelLocalTarget->setText(qApp->languageFull(m_localLanguage));
	m_ui->labelLocalTarget->show();
}

void SkillChannel::setStyleCalling()
{
	m_recentCallStatus = RecentStatusCalling;
	m_ui->labelState->setText(tr("Calling..."));
	m_ui->labelMovie->setMovie(m_movieCalling);
	m_movieCalling->start();

	if (m_local) {
		m_ui->labelLocalTarget->setStyleSheet(m_ringingStyle);
	} else {
		m_ui->labelSource->setStyleSheet(m_ringingStyle);
		m_ui->labelTarget->setStyleSheet(m_ringingStyle);
	}
}

void SkillChannel::setStyleConfirming()
{
	m_recentCallStatus = RecentStatusConfirmed;
	if(m_movieCalling) {
		m_movieCalling->stop();
	}

	m_ui->labelState->setText(tr("Confirming..."));
	m_ui->labelState->setStyleSheet(m_confirmingStyle);
	m_ui->labelMovie->setMovie(m_movieConfirming);
	m_movieConfirming->start();

	if (m_local) {
		m_ui->labelLocalTarget->setStyleSheet(m_confirmingStyle);
	} else {
		m_ui->labelSource->setStyleSheet(m_confirmingStyle);
		m_ui->labelTarget->setStyleSheet(m_confirmingStyle);
	}
}

void SkillChannel::restroeState()
{
	m_recentCallStatus = RecentStatusDisconnect;

	if(m_movieCalling) {
		m_movieCalling->stop();
	}

	if(m_movieConfirming) {
		m_movieConfirming->stop();
	}

	m_ui->labelState->setText(tr("Waiting"));
	m_ui->labelState->setStyleSheet(m_waitingStyle);
	m_ui->labelMovie->setMovie(NULL);
	m_ui->labelMovie->update();

	if(m_local) {
		m_ui->labelLocalTarget->setStyleSheet(m_waitingStyle);
	} else {
		m_ui->labelSource->setStyleSheet(m_waitingStyle);
		m_ui->labelTarget->setStyleSheet(m_waitingStyle);
	}
}

void SkillChannel::changeEvent(QEvent* event)
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

void SkillChannel::retranslateTr()
{
	if(m_online) {
		m_ui->labelState->setText(tr("Waiting"));
	} else {
		m_ui->labelState->setText(tr("Stop"));
	}

	if (m_recentCallStatus == RecentStatusCalling){
		m_ui->labelState->setText(tr("Calling..."));
	} else if(m_recentCallStatus == RecentStatusConfirmed) {
		m_ui->labelState->setText(tr("Confirming..."));
	}

	m_ui->labelSource->setText(qApp->languageFull(m_sourceLang));
	m_ui->labelTarget->setText(qApp->languageFull(m_targetLang));

	m_ui->labelLocalTarget->setText(qApp->languageFull(m_localLanguage));
}

} // namespace ns
