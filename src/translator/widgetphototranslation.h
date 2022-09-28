#ifndef WIDGETPHOTOTRANSLATION_H
#define WIDGETPHOTOTRANSLATION_H

#include <QtGui/QWidget>
#include <QtCore/QPointer>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace ns {

class Ui_WidgetPhotoTranslation;
class MediaService;
class ImageView;
class CustomLoadingLabel;

class WidgetPhotoTranslation : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)
	
public:
    explicit WidgetPhotoTranslation(QWidget* parent = 0);
    virtual ~WidgetPhotoTranslation();

	void init(const QStringList& photoIdList);
	void init(const QString& photoId);
	void uninit();

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

	void downloadPhoto(const QString& largePhotolink, const QString& photoId);

	void setPhotoPicture(const QString& photoLink, const QString& photoFile);

	void setPhotoPicture(const QString& photoId, const QString& photoLink, const QString& photoFile);

	void setSmallPhotoUrl(const QString& photoId, const QString& photoLink);

	QString photoDirPath() const;

	QStringList photoIdList() const;


Q_SIGNALS:
	void downloadFinished();
protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:
	void slotFileDownloadLength(const QString& link, qint64 length);
	void slotFileDownloadBytes(const QString& link,qint64 bytes);
	void slotFileDownloadFailed(const QString& link, const QString& reason);
	void slotFileDownloaded(const QString& link, const QString& localFile);
	void on_pushButtonLeft_clicked();
	void on_pushButtonRight_clicked();
private:
	bool removePhotoDir(const QString& path);

	QString m_photoDirName;
	QStringList m_photoIdList;
	QList<CustomLoadingLabel*> m_labelPhotoList;
	QPointer<MediaService> m_mediaService;

	QScopedPointer<Ui_WidgetPhotoTranslation> m_ui;
	QScopedPointer<ImageView> m_imageView;

	void startAnimation(const QString& id, const QString& name, const QString& localFile, const QString& localDir);

protected:
    Q_DISABLE_COPY(WidgetPhotoTranslation)
};

} // namespace ns

#endif // WIDGETPHOTOTRANSLATION_H
