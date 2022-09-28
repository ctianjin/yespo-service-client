#ifndef QPID_MEDIASERVICE_P_H
#define QPID_MEDIASERVICE_P_H

#include <QtCore/QPointer>
#include "mediaservice.h"
#include "abstractservice_p.h"
#include "downloadqueue_p.h"

namespace ns {

class CommandService;
class DownloadThread;

struct MediaServiceOptions
{
    static const QString download_;
	static const QString download_file_block_size_;

    struct
    {
		qlonglong fileBlockSize;
    } download;

    MediaServiceOptions();
};

class MediaServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(MediaService)

public:
    static const QString tokenSeparator;

    MediaServiceOptions m_opt;
    QPointer<CommandService> m_commandService;
	HeadQueue* m_fileHeadQueue;
	DownloadQueue* m_fileDownloadQueue;
	DownloadThread* m_downloadThread;

    MediaServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~MediaServicePrivate();

    virtual void startup();
    virtual void cleanup();

	void downloadFile(const QString& link, const QString& localFile);
	void abortDownloadFile(const QString& link, const QString& localFile);
	void setBlockSize(qint64 blocksize);
	
	void _q_fileHeadFinished(const QString& token, const HeadResult& result);
	void _q_fileDownloadDataReady(const QString& token, const QByteArray& data);
	void _q_fileDownloadFinished(const QString& link, const DownloadResult& result);
	void _q_fileDownloadFailed(const QString& link, const QString& reason);

private:
	void init();

	static QString makeFileToken(const QString& link, const QString& localFile);
	static QStringList extractFileToken(const QString& token);
};

} // namespace ns

#endif // QPID_MEDIASERVICE_P_H
