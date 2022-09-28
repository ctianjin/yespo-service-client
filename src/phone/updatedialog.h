#ifndef QPID_UPDATADIALOG_H
#define QPID_UPDATADIALOG_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include "framelessdialog.h"
#include "command.h"

namespace Qpid {

class Ui_UpdateDialog;
class CommandService;
class MediaService;

class UpdateDialog : public FramelessDialog
{
    Q_OBJECT
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)

public:
    UpdateDialog::UpdateDialog(QWidget* parent = 0);
    ~UpdateDialog();

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

	void reset();
	void startUpdate();
	bool startUpdate(const QString& version, const QString& link, const QString& force);

protected:
	virtual void changeEvent(QEvent* event);
	virtual void hideEvent(QHideEvent * event);

private	Q_SLOTS:
	void slotVersionFinished(const Command::VersionParam& param, const Command::VersionResult& result);
	void slotFileDownloadLength(const QString& link, qint64 length);
	void slotFileDownloadBytes(const QString& link, qint64 bytes);
	void slotFileDownloadFailed(const QString& link, const QString& reason);
	void slotFileDownloaded(const QString& link, const QString& localFile);

	void on_updateButton_clicked();

private:
	QString m_installerLink;
	QString m_localFile;
	qint64 m_totalSize;
	void downloadInstaller(const QString& link);

	QPointer<CommandService> m_commandService;
	QPointer<MediaService> m_mediaService;

	QScopedPointer<Ui_UpdateDialog> m_ui;

	QString m_forceUpdate;
};

} // namespace Qpid

#endif // QPID_UPDATADIALOG_H
