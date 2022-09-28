#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QEventLoop>
#include <QDesktopServices>
#ifdef Q_WS_WIN
#include <windows.h>
#include <shellapi.h>
#endif
#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "phoneapplication.h"
#include "commandservice.h"
#include "mediaservice.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "UpdateDialog:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

UpdateDialog::UpdateDialog(QWidget* parent) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_UpdateDialog),
	m_installerLink(),
	m_localFile(),
	m_totalSize(0),
	m_forceUpdate(QLatin1String("2"))
{
    m_ui->setupUi(this);

	reset();
}

UpdateDialog::~UpdateDialog()
{
	 
}

CommandService* UpdateDialog::commandService() const
{
	return m_commandService;
}

void UpdateDialog::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(versionFinished(Command::VersionParam, Command::VersionResult)), 
			this, SLOT(slotVersionFinished(Command::VersionParam, Command::VersionResult)));
	}

	m_commandService = commandService;

	if (m_commandService) {
		connect(m_commandService, SIGNAL(versionFinished(Command::VersionParam, Command::VersionResult)), 
			this, SLOT(slotVersionFinished(Command::VersionParam, Command::VersionResult)));
	}
}

MediaService* UpdateDialog::mediaService() const
{
	return m_mediaService;
}

void UpdateDialog::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
		disconnect(m_mediaService, SIGNAL(fileDownloadLength(const QString&,qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&,qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&,qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&,qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&,const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&,const QString&)));
		disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&,const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&,const QString&)));
	}

	m_mediaService = mediaService;

	if (m_mediaService) {
		connect(m_mediaService, SIGNAL(fileDownloadLength(const QString&,qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&,qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&,qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&,qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&,const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&,const QString&)));
		connect(m_mediaService, SIGNAL(fileDownloaded(const QString&,const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&,const QString&)));
	}
}

void UpdateDialog::reset()
{
	m_ui->progressBar->reset();
	m_ui->progressBar->hide();
	m_ui->textEdit->clear();
	m_installerLink.clear();
	m_localFile.clear();

	m_ui->textEdit->setText(tr("Getting version information..."));
	m_ui->updateButton->setEnabled(false);
}

void UpdateDialog::startUpdate()
{
	Q_ASSERT(m_commandService);
	Q_ASSERT(m_mediaService);

 	if (!m_localFile.isEmpty())
 		m_mediaService->abortDownloadFile(m_localFile);
	Command::VersionParam param;
	m_commandService->version(param);
}

bool UpdateDialog::startUpdate(const QString& version, const QString& link, const QString& force)
{
	Q_ASSERT(m_commandService);
	Q_ASSERT(m_mediaService);

	if (!m_localFile.isEmpty())
		m_mediaService->abortDownloadFile(m_localFile);

	QString currentVersion = qApp->applicationVersion();
	QString latestVersion = version;
	QString text;
	if (currentVersion < latestVersion) {
		m_ui->updateButton->setEnabled(true);
		text = tr("Current Version %1").arg(currentVersion);
		text += QLatin1String("\r\n");
		text += tr("Latest Version %1").arg(latestVersion);
		m_ui->textEdit->setText(text);
		m_installerLink = link;
		if (force == QLatin1String("1")) {
			m_forceUpdate = force;
			on_updateButton_clicked();
		}
		return true;
	} else {
		m_ui->updateButton->setEnabled(false);
		text = tr("Current version is the latest version");
		m_ui->textEdit->setText(text);
		return false;
	}

}

void UpdateDialog::changeEvent(QEvent* event)
{
	FramelessDialog::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void UpdateDialog::hideEvent(QHideEvent * event)
{
	FramelessDialog::hideEvent(event);

	qWarning() << QPID_DEBUG_PREFIX << "Hide update dialog with force update " << m_forceUpdate;
	if (m_forceUpdate == QLatin1String("1")) {
		qApp->quit();
	}
}

void UpdateDialog::slotVersionFinished(const Command::VersionParam& param, const Command::VersionResult& result)
{
	QString currentVersion = qApp->applicationVersion();
	QString latestVersion = result.number();
	QString text;
	if (currentVersion < latestVersion) {
		m_ui->updateButton->setEnabled(true);
		text = tr("Current Version %1").arg(currentVersion);
		text += QLatin1String("\r\n");
		text += tr("Latest Version %1").arg(latestVersion);
		m_ui->textEdit->setText(text);
		m_installerLink = result.link();
	} else {
		m_ui->updateButton->setEnabled(false);
		text = tr("Current version is the latest version");
		m_ui->textEdit->setText(text);
	}
}

void UpdateDialog::slotFileDownloadLength(const QString& link, qint64 length)
{
	if (link != m_installerLink)
		return;
	qDebug() << QString::number(length);
	m_totalSize = length;
	m_ui->progressBar->show();
	m_ui->progressBar->setRange(0, length);
}

void UpdateDialog::slotFileDownloadBytes(const QString& link, qint64 bytes)
{
	if (link != m_installerLink)
		return;
	static qint64 i = 0;
	i += bytes;
	qDebug() << QString::number(bytes);
	qDebug() << "Current size "  << i;
	m_ui->progressBar->setValue(m_ui->progressBar->value()+bytes);
}

void UpdateDialog::slotFileDownloadFailed(const QString& link, const QString& reason)
{
	if (link != m_installerLink)
		return;
 	if (!m_localFile.isEmpty())
 		m_mediaService->abortDownloadFile(m_localFile);
	qApp->information(reason);
	reject();
}

void UpdateDialog::slotFileDownloaded(const QString& link, const QString& localFile)
{
	if (link != m_installerLink)
		return;
	qDebug() << "Start executing update program" << localFile;

	//wait for 3 second to execute the update
	QEventLoop eventLoop;
	QTimer timer;
	timer.setSingleShot(true);
	timer.setInterval(3000);
	connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
	timer.start();
	eventLoop.exec();

	QFile f(localFile);
	if (f.open(QIODevice::ReadOnly)) {
		if (f.size() != m_totalSize) 
			qWarning() << "Wait the file to complete!";
		f.close();
	}

	QString updateFile = localFile;
	updateFile.prepend(QLatin1Char('\"'));
	updateFile.append(QLatin1Char('\"'));

#ifdef Q_WS_WIN
	qWarning() << QPID_DEBUG_PREFIX << "Execute update on Windows";
	int result = (int)::ShellExecuteA(0, "open", updateFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
	if (SE_ERR_ACCESSDENIED == result) {
		// Requesting elevation
		result = (int)::ShellExecuteA(0, "runas", updateFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
	}
	if (result <= 32) {
		qApp->critical(tr("Can't execute the update program!"), this);
	} else {
		qApp->quit();
	}
#else
	qWarning() << QPID_DEBUG_PREFIX << "Execute update on Others";
	if (QProcess::startDetached(updateFile)) {
		setEnabled(false);
		accept();
		qApp->quit();
	} else {
		qApp->critical(tr("Can't execute the update program!"), this);
	}
#endif
}

void UpdateDialog::on_updateButton_clicked()
{
	downloadInstaller(m_installerLink);
	m_ui->updateButton->setEnabled(false);
}

void UpdateDialog::downloadInstaller(const QString& link)
{
	Q_ASSERT(m_mediaService);

	QDir dir(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
	m_localFile = dir.absoluteFilePath(QLatin1String("VoiceEasy_Update.exe"));
	if (QFile::exists(m_localFile)) {
		if (!QFile::remove(m_localFile)) {
			qApp->critical(tr("Update failed, please exit the update program."));
			return;
		}
	}

	m_mediaService->downloadFile(link, m_localFile);
}

} // namespace Qpid
