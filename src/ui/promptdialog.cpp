#include <QtCore/QString>
#include <QtGui/QPixmap>
#include "promptdialog.h"
#include "preference.h"
#include "ui_promptdialog.h"

namespace ns {

PromptDialog::PromptDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_PromptDialog)
{
    m_ui->setupUi(this);
	
}

PromptDialog::~PromptDialog()
{
}

int PromptDialog::information(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/information.png"), text, button,showIcon);
}

int PromptDialog::critical(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/critical.png"), text, button,showIcon);
}
int PromptDialog::weep(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/weep.png"), text, button,showIcon);
}

int PromptDialog::question(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/question.png"), text, button,showIcon);
}

int PromptDialog::warning(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/warning.png"), text, button, showIcon);
}

int PromptDialog::however(const QString &text, PromptButton button, bool showIcon)
{
	return prompt(QLatin1String(":/images/however.png"), text, button,showIcon);
}

void PromptDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

int PromptDialog::prompt(const QString &image, const QString &text, PromptButton button, bool showIcon)
{
	(button & ok)?m_ui->pushButtonOK->show():m_ui->pushButtonOK->hide();
	(button & yes)?m_ui->pushButtonYes->show():m_ui->pushButtonYes->hide();
	(button & yes)?m_ui->pushButtonNo->show():m_ui->pushButtonNo->hide();

	QString style = QLatin1String("border-image: url(")+image+QLatin1String(")");
	m_ui->label->setStyleSheet(style);
	m_ui->label->setVisible(showIcon);
	m_ui->labelText->setText(text);
	return exec();
}

} // namespace ns
