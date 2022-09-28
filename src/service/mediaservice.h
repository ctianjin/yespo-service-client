#ifndef MEDIASERVICE_H
#define MEDIASERVICE_H

#include "abstractservice.h"
#include "serviceglobal.h"

QT_BEGIN_NAMESPACE
class QByteArray;
QT_END_NAMESPACE

namespace ns {

class MediaServicePrivate;

class BUILDSYS_SERVICE_DECL MediaService : public AbstractService
{
    Q_OBJECT

public:
    explicit MediaService(ServiceManager* manager, QObject* parent = 0);
    MediaService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~MediaService();
   
	void downloadFile(const QString& link, const QString& localFile);
	void abortDownloadFile(const QString& link, const QString& localFile);
	void setBlockSize(qint64 blocksize);

    virtual void start();
    virtual void stop();

Q_SIGNALS:
	void fileDownloadLength(const QString& link, qint64 length);
	void fileDownloadBytes(const QString& link, qint64 bytes);
	void fileDownloadFailed(const QString& link, const QString& reason);
	void fileDownloaded(const QString& link, const QString& localFile);

protected:
    MediaService(MediaServicePrivate& dd, QObject* parent = 0);

    virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
    
	Q_PRIVATE_SLOT(d_func(), void _q_fileHeadFinished(const QString&, const HeadResult&))
	Q_PRIVATE_SLOT(d_func(), void _q_fileDownloadDataReady(const QString&, const QByteArray&))
	Q_PRIVATE_SLOT(d_func(), void _q_fileDownloadFinished(const QString&, const DownloadResult&))
	Q_PRIVATE_SLOT(d_func(), void _q_fileDownloadFailed(const QString&, const QString&))

    Q_DECLARE_PRIVATE(MediaService)
    Q_DISABLE_COPY(MediaService)
};

} // namespace ns

#endif // MEDIASERVICE_H
