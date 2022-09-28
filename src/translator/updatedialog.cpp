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
#include "downloadthread.h"
#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "phoneapplication.h"
#include "mediaservice.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "UpdateDialog:"
#endif // DEBUG_PREFIX

namespace ns {

UpdateDialog::UpdateDialog(QWidget* parent, int updateMethod) :
    FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_UpdateDialog),
	m_installerLink(),
	m_localFile(),
	m_totalSize(0),
	m_userUpdate(false),
	m_updateMethod(updateMethod)
{
    m_ui->setupUi(this);

	reset();
}

UpdateDialog::~UpdateDialog()
{

}

MediaService* UpdateDialog::mediaService() const
{
	return m_mediaService;
}

void UpdateDialog::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
		disconnect(m_mediaService, SIGNAL(fileDownloadLength(const QString&, qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&, qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&, const QString&)));
	}

	m_mediaService = mediaService;

	if (m_mediaService) {
		connect(m_mediaService, SIGNAL(fileDownloadLength(const QString&, qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&, qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&, const QString&)));
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

bool UpdateDialog::startUpdate(const QString& latestVersion, const QString& downloadLink, bool force)
{
	Q_ASSERT(m_mediaService);

	if (!m_localFile.isEmpty())
		m_mediaService->abortDownloadFile(downloadLink, m_localFile);

	m_latestVersion = latestVersion.trimmed();
	m_installerLink = downloadLink;
	m_force = force;

	QString currentVersion = qApp->applicationVersion();
	QString text;
	if (currentVersion < m_latestVersion) {

		m_ui->updateButton->setEnabled(true);
		text = tr("Current Version %1").arg(currentVersion);
		text += QLatin1String("\r\n");
		text += tr("Latest Version %1").arg(latestVersion);
		m_ui->textEdit->setText(text);

		if (m_updateMethod == 0) {
			on_updateButton_clicked();
			if (m_userUpdate)
				return true;
			return false;
		}

// 		if (m_force) {
// 			on_updateButton_clicked();
// 		} else {
// 			if (m_updateMethod == 0) {
// 				on_updateButton_clicked();
// 				if(m_userUpdate)
// 					return true;
// 				return false;
// 			} 
// 		}
		return true;

	} else {
		m_ui->updateButton->setEnabled(false);
		text = tr("Current version is the latest version");
		m_ui->textEdit->setText(text);
		m_force = false;
		return m_userUpdate;
	}
	return true;
}

void UpdateDialog::setUserUpdate(bool userUpdate)
{
	m_userUpdate = userUpdate;
}

void UpdateDialog::setUpdateMethod(int method)
{
	m_updateMethod = method;
}

int UpdateDialog::updateMethod() const
{
	return m_updateMethod;
}

void UpdateDialog::slotFileDownloadLength(const QString& link, qint64 length)
{
	if (link != m_installerLink)
		return;
	qDebug() << QString::number(length);
	if (length == -1) {
		m_mediaService->abortDownloadFile(link, m_localFile);
		m_ui->textEdit->clear();
		m_ui->textEdit->setText(tr("Download update file failed, please check the download url"));
		return;
	}
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
 		m_mediaService->abortDownloadFile(link, m_localFile);
	qApp->information(reason);
/*	qWarning() << DEBUG_PREFIX << reason;*/
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

#ifdef Q_OS_WIN
	qWarning() << DEBUG_PREFIX << "Execute update on Windows" << updateFile;
//	int result = (int)::ShellExecuteA(0, "open", updateFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
	int result = (int)::ShellExecuteA(0, "runas", updateFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
// 	if (SE_ERR_ACCESSDENIED == result) {
// 		// Requesting elevation
// 		result = (int)::ShellExecuteA(0, "runas", updateFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
// 	}
	if (result == SE_ERR_ACCESSDENIED) {
		reset();	//Restore the UI to original
		reject();	//Close the update dialog
	} else if (result <= 32) {
		qApp->critical(tr("Can't execute the update program!"), this);
	} else {
		qApp->quit();
	}
#else
	qWarning() << DEBUG_PREFIX << "Execute update on Others" << updateFile;
	if (QProcess::startDetached(updateFile)) {
		setEnabled(false);
		accept();
		qApp->quit();
	} else {
		//qApp->critical(tr("Can't execute the update program!"), this);
	}
#endif
}

void UpdateDialog::on_updateButton_clicked()
{
	if(m_installerLink.isEmpty()) {
		qWarning() << DEBUG_PREFIX << "download url is empty";
		m_localFile = QString();
		m_force = false;
		hide();
		return;
	}
	m_installerLink = m_installerLink.trimmed();
	downloadInstaller(m_installerLink);
	m_ui->updateButton->setEnabled(false);
}


void UpdateDialog::downloadInstaller(const QString& link)
{
	Q_ASSERT(m_mediaService);

	int index = link.lastIndexOf(QLatin1String("/"));
	QString fileName = link.right(link.length() - index - 1);
	
	QDir dir(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
	m_localFile = dir.absoluteFilePath(fileName);
	if (QFile::exists(m_localFile)) {
		if (!QFile::remove(m_localFile)) {
			qApp->critical(tr("Update failed, please exit the update program."));
			return;
		}
	}

	m_mediaService->downloadFile(link, m_localFile);
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

	if (m_mediaService && !m_localFile.isEmpty()) {
		m_mediaService->abortDownloadFile(m_installerLink, m_localFile);
		m_localFile = QString();
		m_ui->progressBar->setValue(0);
	}

	qWarning() << DEBUG_PREFIX << "Hide update dialog with force update " << m_force;
	if (m_force) {
		qApp->quit();
	}
}

} // namespace ns
