#include <QtGui/QPixmap>
#include "aboutdialog.h"
#include "phoneapplication.h"
#include "ui_aboutdialog.h"

namespace Qpid {

AboutDialog::AboutDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_AboutDialog)
{
    m_ui->setupUi(this);
	setApplicationIcon(qApp->windowIcon());

    const QString version = tr("Voice Easy \r\nVersion %1\r\nCopyright 2014 Qpid network")
            .arg(qApp->applicationVersion());
     m_ui->versionsLabel->setText(version);
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::changeEvent(QEvent* event)
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

// QString AboutDialog::applicationTitle() const
// {
//    // return m_ui->appNameLabel->text();
// 	
// }
// 
// void AboutDialog::setApplicationTitle(const QString& applicationTitle)
// {
//    // m_ui->appNameLabel->setText(applicationTitle);
// }
// 
QIcon AboutDialog::applicationIcon() const
{
    return m_applicationIcon;
}

void AboutDialog::setApplicationIcon(const QIcon& applicationIcon)
{
    m_applicationIcon = applicationIcon;

    if (m_applicationIcon.isNull()) {
		m_ui->appIcon->setPixmap(QPixmap());
    } else {
		m_ui->appIcon->setPixmap(m_applicationIcon.pixmap(64));
    }
}

} // namespace Qpid
