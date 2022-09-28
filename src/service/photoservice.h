#ifndef PHOTOSERVICE_H
#define PHOTOSERVICE_H

#include "abstractdataservice.h"
#include "serviceglobal.h"
#include "media/mediaphoto.h"

namespace ns {

class PhotoServicePrivate;

class BUILDSYS_SERVICE_DECL PhotoService : public AbstractDataService
{
    Q_OBJECT

public:
    explicit PhotoService(ServiceManager* manager, QObject* parent = 0);
    PhotoService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~PhotoService();

    virtual void start();
    virtual void stop();

	void downloadPhoto(const QString& url);
	QString photoFile(const QString& url);
	bool isDownloading(const QString& url);
	bool isReady(const QString& url);

Q_SIGNALS:
	void photoDownloaded(const QString& url, const QString& fileName);
	void photoDownloadFailed(const QString& url);

protected:
    PhotoService(PhotoServicePrivate& dd, QObject* parent = 0);

    virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
	Q_PRIVATE_SLOT(d_func(), void _q_photoLength(const QString&, qint64))
	Q_PRIVATE_SLOT(d_func(), void _q_photoBytes(const QString&, qint64))
	Q_PRIVATE_SLOT(d_func(), void _q_photoDownloadFailed(const QString&, const QString&))
	Q_PRIVATE_SLOT(d_func(), void _q_photoDownloaded(const QString&, const QString&))

    Q_DECLARE_PRIVATE(PhotoService)
    Q_DISABLE_COPY(PhotoService)
};

} // namespace ns

#endif // PHOTOSERVICE_H
