#ifndef UPDATADIALOG_H
#define UPDATADIALOG_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include "framelessdialog.h"
#include "downloadthread.h"
namespace ns {

class Ui_UpdateDialog;
class MediaService;
class DownloadThread;

class UpdateDialog : public FramelessDialog
{
    Q_OBJECT
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)

public:
    UpdateDialog::UpdateDialog(QWidget* parent = 0, int updateMethod = 0);
    ~UpdateDialog();

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

	void reset();
	bool startUpdate(const QString& latestVersion, const QString& downloadLink, bool force);
	void setUserUpdate(bool state);
	void setUpdateMethod(int method);
	int updateMethod() const;

protected:
	virtual void changeEvent(QEvent* event);
	virtual void hideEvent(QHideEvent *event);

private	Q_SLOTS:
	void slotFileDownloadLength(const QString& link, qint64 length);
	void slotFileDownloadBytes(const QString& link,qint64 bytes);
	void slotFileDownloadFailed(const QString& link, const QString& reason);
	void slotFileDownloaded(const QString& link, const QString& localFile);

	void on_updateButton_clicked();
private:
	bool m_force;
	QString m_latestVersion;
	QString m_installerLink;
	QString m_localFile;
	qint64 m_totalSize;
	bool m_userUpdate;
	int m_updateMethod;
	void downloadInstaller(const QString& link);

	QPointer<MediaService> m_mediaService;

	QScopedPointer<Ui_UpdateDialog> m_ui;
};

} // namespace ns

#endif // UPDATADIALOG_H
