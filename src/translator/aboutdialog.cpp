#include <QtGui/QPixmap>
#include "boxshadow.h"
#include "aboutdialog.h"
#include "phoneapplication.h"
#include "ui_aboutdialog.h"

namespace ns {

AboutDialog::AboutDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_AboutDialog)
{
    m_ui->setupUi(this);

	BoxShadow *boxShadow = new BoxShadow();
	boxShadow->setBlurRadius(35.0);
	boxShadow->setDistance(12.0);
	boxShadow->setColor(QColor(0, 0, 0, 70));
	setGraphicsEffect(boxShadow);

	QString version = qApp->applicationVersion();
	int index = version.lastIndexOf(QLatin1String("."));
	version = version.left(index);

	QString info = tr("YesPo Service Client");
	info += QLatin1String("\r\n");
#ifdef BUILDSYS_OFFICAL
	info += tr("Version %1").arg(version);
#else
	info += tr("Version %1").arg(qApp->applicationVersion());
#endif
	info += QLatin1String("\r\n");
	info += tr("Copyright 2015 %1").arg(qApp->vendorName());
	m_ui->versionsLabel->setText(info);

	setApplicationIcon(qApp->windowIcon());
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

} // namespace ns
