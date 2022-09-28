#include "callmode.h"
#include "ui_callmode.h"
#include "phoneapplication.h"

namespace ns {

CallMode::CallMode(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_CallMode),
	m_statusCode()
{
    m_ui->setupUi(this);
	hide();
}

CallMode::~CallMode()
{
	 
}

void CallMode::setCallingStatusCode(const QString& code)
{
	m_statusCode = code;
	m_ui->labelCallMode->setVisible(!m_statusCode.isEmpty());
	m_ui->labelCallMode->setText(m_statusCode);
}

void CallMode::setCallMode(const QString& mode)
{
	m_callMode = mode;
	if (m_callMode == QLatin1String("a1")) {
		m_ui->labelChannel->setText(tr("Service Channel: On-site Interpretation")); 
	} else {
		m_ui->labelChannel->setText(tr("Service Channel: In-call Interpretation"));
	}
}

void CallMode::setLanguage(const QString& source, const QString& target)
{
	m_source = source;
	m_target = target;

	m_ui->labelSource->setText(qApp->languageFull(source));
	m_ui->labelTarget->setText(qApp->languageFull(target));

// 	m_ui->widgetSource->adjustSize();
// 	m_ui->widgetTarget->adjustSize();

/*	adjustSize();*/
}

void CallMode::setServiceMode(const QString& mode)
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

void CallMode::setLanguageStyle(const QString& style)
{
// 	m_ui->widgetSource->setStyleSheet(QLatin1String("#widgetSource { ")+style+QLatin1String(" }"));
// 	m_ui->widgetTarget->setStyleSheet(QLatin1String("#widgetTarget { ")+style+QLatin1String(" }"));

// 	m_ui->widgetSource->adjustSize();
// 	m_ui->widgetTarget->adjustSize();

	adjustSize();
}

void CallMode::changeEvent(QEvent* event)
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

void CallMode::retranslateTr()
{
	m_ui->labelSource->setText(qApp->languageFull(m_source));
	m_ui->labelTarget->setText(qApp->languageFull(m_target));

	if (m_callMode == QLatin1String("a1")) {
		m_ui->labelChannel->setText(tr("Service Channel: On-site Interpretation"));
	} else {
		m_ui->labelChannel->setText(tr("Service Channel: In-call Interpretation"));
	}

	if (!m_statusCode.isEmpty())
		m_ui->labelCallMode->setText(m_statusCode);

// 	if (m_mode==QLatin1String("a1") || m_mode==QLatin1String("a2") || m_mode==QLatin1String("a3") || m_mode==QLatin1String("c1")) {
// 		m_ui->labelMode->setText(tr("Payment"));
// 	} else {
// 		m_ui->labelMode->setText(tr("Free"));
// 	}
}

} // namespace ns
