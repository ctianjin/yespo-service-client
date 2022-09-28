#include <QByteArray>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QStringList>
#include "mediaservice.h"
#include "mediaservice_p.h"
#include "servicemanager.h"
#include "downloadthread.h"
#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "MediaService:"
#endif // DEBUG_PREFIX

namespace ns {

const QString MediaServiceOptions::download_ = QLatin1String("download");
const QString MediaServiceOptions::download_file_block_size_ = QLatin1String("download.file_block_size");

MediaServiceOptions::MediaServiceOptions()
{
	download.fileBlockSize = 1024;
}

const QString MediaServicePrivate::tokenSeparator = QLatin1String("|");

MediaServicePrivate::MediaServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
	m_fileHeadQueue(0),
	m_fileDownloadQueue(),
	m_downloadThread()
{
    
}

MediaServicePrivate::~MediaServicePrivate()
{
    cleanup();
}

void MediaServicePrivate::init()
{
    Q_Q(MediaService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

// 	m_downloadThread = new DownloadThread(q);
// 	m_downloadThread->init();
// 
// 	QObject::connect(m_downloadThread, SIGNAL(finished(QString, HeadResult)), q, SLOT(_q_fileHeadFinished(QString, HeadResult)));
// 	QObject::connect(m_downloadThread, SIGNAL(dataReady(QString, qint64)), q, SLOT(_q_fileDownloadDataReady(QString, qint64)));
// 	QObject::connect(m_downloadThread, SIGNAL(finished(QString, QString)), q, SLOT(_q_fileDownloadFinished(QString, QString)));
// 	QObject::connect(m_downloadThread, SIGNAL(failed(QString, QString)), q, SLOT(_q_fileDownloadFailed(QString, QString)));
// 	m_downloadThread->start(QThread::IdlePriority);
#if 1
	m_fileHeadQueue = new HeadQueue(q);
	m_fileDownloadQueue = new DownloadQueue(q);

    QNetworkAccessManager* networkAccessManager = m_manager->networkAccessManager();
    
	m_fileHeadQueue->setNetworkAccessManager(networkAccessManager);
	m_fileDownloadQueue->setNetworkAccessManager(networkAccessManager);   
	m_fileDownloadQueue->setBlockSize(m_opt.download.fileBlockSize);
    
	QObject::connect(m_fileHeadQueue, SIGNAL(finished(QString, HeadResult)), 
		q, SLOT(_q_fileHeadFinished(QString, HeadResult)));
	QObject::connect(m_fileDownloadQueue, SIGNAL(dataReady(QString, QByteArray)),
		q, SLOT(_q_fileDownloadDataReady(QString, QByteArray)));
	QObject::connect(m_fileDownloadQueue, SIGNAL(finished(QString, DownloadResult)),
		q, SLOT(_q_fileDownloadFinished(QString, DownloadResult)));
#endif
}

void MediaServicePrivate::startup()
{
    AbstractServicePrivate::startup();

    Q_Q(MediaService);
}

void MediaServicePrivate::cleanup()
{
    Q_Q(MediaService);

    AbstractServicePrivate::cleanup();
}

void MediaServicePrivate::downloadFile(const QString& link, const QString& localFile)
{
#if 1
	if (!m_fileHeadQueue->add(makeFileToken(link, localFile), QUrl(link))) {
		 setError(Service::DataError, MediaService::tr("Failed to download file"));
		return;
	}
	qDebug() << DEBUG_PREFIX << "Download file " << link << " to " << localFile << " submitted!";
#endif
	//m_downloadThread->downloadFile(link, localFile);
}

void MediaServicePrivate::abortDownloadFile(const QString& link, const QString& localFile)
{
#if 1
	m_fileHeadQueue->abort(makeFileToken(link, localFile));
	m_fileDownloadQueue->abort(makeFileToken(link, localFile));
#endif
	//m_downloadThread->abortDownloadFile(link, localFile);
}

void MediaServicePrivate::setBlockSize(qint64 blocksize)
{
#if 1
	m_fileDownloadQueue->setBlockSize(blocksize);
#endif
	//m_downloadThread->setBlockSize(blocksize);
}

void MediaServicePrivate::_q_fileHeadFinished(const QString& token, const HeadResult& result)
{
	Q_Q(MediaService);

	if (!isActive())
		return;
#if 1
	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		setError(Service::InvalidArgumentError, MediaService::tr("Failed to download file"));
		return;
	}

	if (result.error) {
		setError(result.error, result.errorReason);
		return;
	}

	if (!m_fileDownloadQueue->add(makeFileToken(args[0], args[1]), result.url)) {
		setError(Service::DataError, MediaService::tr("Failed to download file"));
		return;
	}

	Q_EMIT q->fileDownloadLength(args[0], result.contentLength);
#endif
	//Q_EMIT q->fileDownloadLength(token, result.contentLength);
}

void MediaServicePrivate::_q_fileDownloadDataReady(const QString& token, const QByteArray& data)
{
	Q_Q(MediaService);

	if (!isActive())
		return;
#if 1
	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		setError(Service::InvalidArgumentError, MediaService::tr("Failed to download file"));
		return;
	}

	QFile file(args[1]);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		m_fileDownloadQueue->abort(token);
		qWarning() << DEBUG_PREFIX << "Failed to open file for writing" << args[1] << "         " << file.errorString();
		setError(Service::DataError, MediaService::tr("Failed to download file"));
		return;
	}

	if (file.write(data) == -1) {
		m_fileDownloadQueue->abort(token);
		qWarning() << DEBUG_PREFIX << "Failed to write data" << args[1] << "         " << file.errorString();
		setError(Service::DataError, MediaService::tr("Failed to download file"));
		return;
	}
	Q_EMIT q->fileDownloadBytes(args[0], data.length());
#endif
	//Q_EMIT q->fileDownloadBytes(token, length);
}

// void MediaServicePrivate::_q_fileDownloadFinished(const QString& link, const QString& localFile)
// {
// 	Q_Q(MediaService);
// 
// 	if (!isActive())
// 		return;
// 
// 	Q_EMIT q->fileDownloaded(link, localFile);
// }

void MediaServicePrivate::_q_fileDownloadFailed(const QString& link, const QString& reason)
{
	Q_Q(MediaService);

	if (!isActive())
		return;

	Q_EMIT q->fileDownloadFailed(link, reason);
}
#if 1
void MediaServicePrivate::_q_fileDownloadFinished(const QString& token, const DownloadResult& result)
{
	Q_Q(MediaService);

	if (!isActive())
		return;

	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		setError(Service::InvalidArgumentError, MediaService::tr("Failed to download file"));
		return;
	}

	QByteArray data;
	if (result.error) {
		qWarning() << result.errorReason << "\n";
		Q_EMIT q->fileDownloadFailed(args[0], MediaService::tr("Failed to download file"));
		return;
	} else {
		data = result.data;

		QFile file(args[1]);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
			m_fileDownloadQueue->abort(token);
			qWarning() << DEBUG_PREFIX << "Failed to open file for writing" << args[1] << "         " << file.errorString();
			setError(Service::DataError, MediaService::tr("Failed to download file"));
			return;
		}

		if (file.write(data) == -1) {
			m_fileDownloadQueue->abort(token);
			qWarning() << DEBUG_PREFIX << "Failed to write data" << args[1] << "         " << file.errorString();
			setError(Service::DataError, MediaService::tr("Failed to download file"));
			return;
		}

		Q_EMIT q->fileDownloadBytes(args[0], data.length());
	}
	Q_EMIT q->fileDownloaded(args[0], args[1]);
}
#endif

QString MediaServicePrivate::makeFileToken(const QString& link, const QString& localFile)
{
	QString ret;
	ret += link;
	ret += tokenSeparator;
	ret += localFile;
	return ret;
}

QStringList MediaServicePrivate::extractFileToken(const QString& token)
{
	QStringList ret;
	ret = token.split(tokenSeparator);
	return ret;
}

MediaService::MediaService(ServiceManager* manager, QObject* parent) :
    AbstractService(*new MediaServicePrivate(manager, QString()), parent)
{
    Q_D(MediaService);
    d->init();
}

MediaService::MediaService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractService(*new MediaServicePrivate(manager, name), parent)
{
    Q_D(MediaService);
    d->init();
}

MediaService::MediaService(MediaServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
    Q_D(MediaService);
    d->init();
}

MediaService::~MediaService()
{
}

void MediaService::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(MediaService);

    typedef MediaServiceOptions Opt;
    Opt& opt = d->m_opt;
    Preference* pref = preference();

	if (force || paths.contains(Opt::download_file_block_size_)) {
		opt.download.fileBlockSize = pref->getLongLong(Opt::download_file_block_size_);
		d->m_fileDownloadQueue->setBlockSize(opt.download.fileBlockSize);
		//d->m_downloadThread->setBlockSize(opt.download.fileBlockSize);
	}
}

void MediaService::downloadFile(const QString& link, const QString& localFile)
{
	Q_D(MediaService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "downloadFile rejected because of invalid state";
		return;
	}

	d->downloadFile(link, localFile);
}

void MediaService::abortDownloadFile(const QString& link, const QString& localFile)
{
	Q_D(MediaService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "abortDownloadFile rejected because of invalid state";
		return;
	}

	d->abortDownloadFile(link, localFile);
}

void MediaService::setBlockSize(qint64 blocksize)
{
	Q_D(MediaService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "setBlockSize rejected because of invalid state";
		return;
	}

	d->setBlockSize(blocksize);
}

void MediaService::start()
{
    AbstractService::start();
}

void MediaService::stop()
{
    AbstractService::stop();
}

} // namespace ns

#include "moc_mediaservice.cpp"
