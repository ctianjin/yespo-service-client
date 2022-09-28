#include "downloadthread.h"
#include "downloadqueue_p.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QMutex>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "DownloadThread:"
#endif // DEBUG_PREFIX
namespace ns {

const char* DownloadThread::tokenProperty = "token";
const QString DownloadThread::tokenSeparator = QLatin1String("|");

DownloadThread::DownloadThread(QObject* parent) :
    QThread(parent),
	m_downloadLink(),
	m_localFile(),
	m_flag(false),
	m_fileHeadQueue(0),
	m_fileDownloadQueue()
{
	
}

DownloadThread::~DownloadThread()
{
	m_flag = true;
	quit();
	wait();
}

void DownloadThread::init()
{
// 	m_fileHeadQueue = new HeadQueue(this);
// 	m_fileDownloadQueue = new DownloadQueue(this);
// 
// 	QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager();
// 
// 	m_fileHeadQueue->setNetworkAccessManager(networkAccessManager);
// 	m_fileDownloadQueue->setNetworkAccessManager(networkAccessManager);   
// 	m_fileDownloadQueue->setBlockSize(1024);
// 
// 	connect(m_fileHeadQueue, SIGNAL(finished(QString, HeadResult)), 
// 		this, SLOT(slotFileHeadFinished(QString, HeadResult)));
// 	connect(m_fileDownloadQueue, SIGNAL(dataReady(QString, QByteArray)),
// 		this, SLOT(slotFileDownloadDataReady(QString, QByteArray)));
// 	connect(m_fileDownloadQueue, SIGNAL(finished(QString, DownloadResult)),
// 		this, SLOT(slotFileDownloadFinished(QString, DownloadResult)));
}

void DownloadThread::slotReplyHeadFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply)
		return;

	const QString token = reply->property(tokenProperty).toString();
	HeadResult result;

	if (reply->error()) {
		

	} else {
		QVariant value;

		result.url = reply->url();

		value = reply->header(QNetworkRequest::ContentLengthHeader);
		if (value.isValid())
			result.contentLength = value.toLongLong();

		value = reply->header(QNetworkRequest::LastModifiedHeader);
		if (value.isValid())
			result.lastModified = value.toDateTime();

		result.etag = reply->rawHeader("ETag");
	}

	reply->disconnect(this);
	reply->close();
	reply->deleteLater();

	slotFileHeadFinished(token, result);
}

void DownloadThread::slotFileHeadFinished(const QString& token, const HeadResult& result)
{
	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		return;
	}

	if (result.error) {
		return;
	}

	if (!m_fileDownloadQueue->add(makeFileToken(args[0], args[1]), result.url)) {
		return;
	}
	
	Q_EMIT finished(args[0], result);
}

void DownloadThread::slotFileDownloadDataReady(const QString& token, const QByteArray& data)
{
	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		return;
	}

	QFile file(args[1]);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		m_fileDownloadQueue->abort(token);
		qWarning() << DEBUG_PREFIX << "Failed to open file for writing" << args[1] << "         " << file.errorString();
		return;
	}

	if (file.write(data) == -1) {
		m_fileDownloadQueue->abort(token);
		qWarning() << DEBUG_PREFIX << "Failed to write data" << args[1] << "         " << file.errorString();
		return;
	}

	Q_EMIT dataReady(args[0], data.length());
}

void DownloadThread::slotFileDownloadFinished(const QString& token, const DownloadResult& result)
{
	QStringList args = extractFileToken(token);

	if (args.size() != 2) {
		return;
	}

	QByteArray data;
	if (result.error) {
		qWarning() << result.errorReason << "\n";
		Q_EMIT failed(args[0], tr("Failed to download file"));
	} else {
		data = result.data;

		QFile file(args[1]);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
			m_fileDownloadQueue->abort(token);
			qWarning() << DEBUG_PREFIX << "Failed to open file for writing" << args[1] << "         " << file.errorString();
			return;
		}

		if (file.write(data) == -1) {
			m_fileDownloadQueue->abort(token);
			qWarning() << DEBUG_PREFIX << "Failed to write data" << args[1] << "         " << file.errorString();
			return;
		}

		Q_EMIT dataReady(args[0], data.length());
	}
	Q_EMIT finished(args[0], args[1]);
}

void DownloadThread::downloadFile(const QString& link, const QString& localFile)
{
	if (!link.isEmpty() && !localFile.isEmpty()) {
		QMutexLocker lock(&m_mutex);
		m_downloadUrls.insert(link, localFile);
	}
}

void DownloadThread::download(const QString& link, const QString& localFile)
{
	if (!m_fileHeadQueue->add(makeFileToken(link, localFile), QUrl(link))) {
		return;
	}
	qDebug() << DEBUG_PREFIX << "Download file " << link << " to " << localFile << " submitted!";
}

void DownloadThread::abortDownloadFile(const QString& link, const QString& localFile)
{
	//m_fileHeadQueue->abort(makeFileToken(link, localFile));
	//m_fileDownloadQueue->abort(makeFileToken(link, localFile));
}

void DownloadThread::setBlockSize(qint64 blocksize)
{
	//m_fileDownloadQueue->setBlockSize(blocksize);
}

void DownloadThread::run()
{
	m_fileHeadQueue = new HeadQueue();
	m_fileDownloadQueue = new DownloadQueue();

	QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager();

	m_fileHeadQueue->setNetworkAccessManager(networkAccessManager);
	m_fileDownloadQueue->setNetworkAccessManager(networkAccessManager);   
	m_fileDownloadQueue->setBlockSize(1024);

	while(!m_flag) {
		m_mutex.lock();
		if (m_downloadUrls.size() > 0) {
			m_downloadLink = m_downloadUrls.begin().key();
			m_localFile = m_downloadUrls.begin().value();
		}
		m_mutex.unlock();
		if (!m_downloadLink.isEmpty() && !m_localFile.isEmpty()) {

			QNetworkReply* reply = networkAccessManager->head(QNetworkRequest(QUrl(m_downloadLink)));
			if (!reply) {
				qWarning() << DEBUG_PREFIX << "Failed to get network head reply";
			}

			reply->setParent(this);
			reply->setProperty(tokenProperty, makeFileToken(m_downloadLink, m_localFile));

			connect(reply, SIGNAL(finished()), this, SLOT(slotReplyHeadFinished()));

			m_networkReplies.insert(makeFileToken(m_downloadLink, m_localFile), reply);

			m_mutex.lock();
			m_downloadUrls.remove(m_downloadLink);
			m_mutex.unlock();

			m_downloadLink = QString();
			m_localFile = QString();
		}
	}
	
	m_fileHeadQueue->deleteLater();
	m_fileDownloadQueue->deleteLater();

}

QString DownloadThread::makeFileToken(const QString& link, const QString& localFile)
{
	QString ret;
	ret += link;
	ret += tokenSeparator;
	ret += localFile;
	return ret;
}

QStringList DownloadThread::extractFileToken(const QString& token)
{
	QStringList ret;
	ret = token.split(tokenSeparator);
	return ret;
}

} // namespace ns
