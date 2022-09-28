#include "translationlanguage.h"
#include "ui_translationlanguage.h"
#include "phoneapplication.h"

namespace ns {

TranslationLanguage::TranslationLanguage(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TranslationLanguage)
{
    m_ui->setupUi(this);
}

TranslationLanguage::~TranslationLanguage()
{
	 
}

void TranslationLanguage::setLanguage(const QString& source, const QString& target)
{
	m_source = source;
	m_target = target;

	QString srcImage = QLatin1String(":/images/ui/") + source + QLatin1String(".png");
	QString targetImage = QLatin1String(":/images/ui/") + target + QLatin1String(".png");

	m_ui->labelSourceFlag->setStyleSheet(QString(QLatin1String("border-image: url(%1)")).arg(srcImage));
	m_ui->labelTargetFlag->setStyleSheet(QString(QLatin1String("border-image: url(%1)")).arg(targetImage));

	m_ui->labelSource->setText(qApp->languageFull(source));
	m_ui->labelTarget->setText(qApp->languageFull(target));

	m_ui->widgetSource->adjustSize();
	m_ui->widgetTarget->adjustSize();

	adjustSize();
}

void TranslationLanguage::setServiceMode(const QString& mode)
{
// 	m_mode = mode;
// 
// 	if (mode==QLatin1String("a1") || mode==QLatin1String("a2") || mode==QLatin1String("a3") || mode==QLatin1String("c1")) {
// 		m_ui->labelMode->setText(tr("Payment"));
// 		m_ui->labelMode->setStyleSheet(QLatin1String("color: #ff6600; border-color:rgb(255,255,255)"));
// 	} else {
// 		m_ui->labelMode->setText(tr("Free"));
// 		m_ui->labelMode->setStyleSheet(QLatin1String("color: #339933; border-color:rgb(255,255,255)"));
// 	}
// 
// 	m_ui->widgetSource->adjustSize();
// 	m_ui->widgetTarget->adjustSize();
// 
// 	adjustSize();
}

void TranslationLanguage::setLanguageStyle(const QString& style)
{
	m_ui->widgetSource->setStyleSheet(QLatin1String("#widgetSource { ")+style+QLatin1String(" }"));
	m_ui->widgetTarget->setStyleSheet(QLatin1String("#widgetTarget { ")+style+QLatin1String(" }"));

	m_ui->widgetSource->adjustSize();
	m_ui->widgetTarget->adjustSize();

	adjustSize();
}

void TranslationLanguage::changeEvent(QEvent* event)
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

void TranslationLanguage::retranslateTr()
{
	m_ui->labelSource->setText(qApp->languageFull(m_source));
	m_ui->labelTarget->setText(qApp->languageFull(m_target));


// 	if (m_mode==QLatin1String("a1") || m_mode==QLatin1String("a2") || m_mode==QLatin1String("a3") || m_mode==QLatin1String("c1")) {
// 		m_ui->labelMode->setText(tr("Payment"));
// 	} else {
// 		m_ui->labelMode->setText(tr("Free"));
// 	}
}

} // namespace ns
