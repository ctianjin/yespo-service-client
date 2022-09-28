#ifndef PHOTOSERVICE_P_H
#define PHOTOSERVICE_P_H

#include <QDir>
#include "abstractdataservice_p.h"
#include "media/mediadaos_p.h"

namespace ns {

class MediaService;

struct PhotoServiceOptions
{
    PhotoServiceOptions();
};

class PhotoServicePrivate : public AbstractDataServicePrivate
{
    Q_DECLARE_PUBLIC(PhotoService)

public:
    PhotoServiceOptions m_opt;
	QPointer<MediaService> m_mediaService;
    Media::MediaDaos m_daos;
	QDir m_photoLocation;

    PhotoServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~PhotoServicePrivate();

    void start();
    void stop();

	void downloadPhoto(const QString& url);
	QString photoFile(const QString& url);
	bool isDownloading(const QString& url);
	bool isReady(const QString& url);

	void _q_photoLength(const QString& link, qint64 length);
	void _q_photoBytes(const QString& link, qint64 bytes);
	void _q_photoDownloadFailed(const QString& link, const QString& reason);
	void _q_photoDownloaded(const QString& link, const QString& fileName);

protected:
    virtual void cleanup();

private:
    void init();
};

} // namespace ns

#endif // PHOTOSERVICE_P_H
