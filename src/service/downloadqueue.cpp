#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/QMutex>
#include <QtCore/QCryptographicHash>
#include "downloadqueue_p.h"
#include "service_p.h"

#ifdef DEBUG_PREFIX
#  define DEBUG_PREFIX_EXTERNAL DEBUG_PREFIX
#endif // DEBUG_PREFIX

#ifndef DEBUG_PREFIX_EXTERNAL
#  undef DEBUG_PREFIX
#  define DEBUG_PREFIX "HeadQueue:"
#endif // DEBUG_PREFIX_EXTERNAL

namespace ns {

const char* HeadQueue::tokenProperty = "token";
const char* DownloadQueue::tokenProperty = "token";
const char* DownloadQueue::blockSizeProperty = "blockSize";

HeadResult::HeadResult() :
    error(Service::NoError),
    contentLength(-1)
{
}

HeadQueue::HeadQueue(QObject* parent) :
    QObject(parent),
	m_finished(true)
{

}

HeadQueue::~HeadQueue()
{
    clear();
}

QNetworkAccessManager* HeadQueue::networkAccessManager() const
{
    return m_networkAccessManager;
}

void HeadQueue::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    m_networkAccessManager = networkAccessManager;
}

void HeadQueue::setToken(const QString& token)
{
	m_token = token;
}

void HeadQueue::setSystemId(const QString& id)
{
	m_systemId = id;
}

bool HeadQueue::add(const QString& token, const QUrl& url)
{
    QNetworkRequest request(url);
	addCommonRequestHeaders(request);
	request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_systemId).arg(m_token).toAscii()).toBase64());
	//request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
    return add(token, request);
}

void HeadQueue::slotReplyFinished(QNetworkReply* reply)
{
	if (!reply)
		return;
	if (reply->url().isEmpty())
		return;
}

bool HeadQueue::add(const QString& token, const QNetworkRequest& request)
{
    abort(token);

	if (!m_networkAccessManager) {
		qWarning() << DEBUG_PREFIX << "NetworkAccessManager is not available";
		return false;
	}
	connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));

	QNetworkReply* reply = m_networkAccessManager->head(request);
	if (!reply) {
		qWarning() << DEBUG_PREFIX << "Failed to get network head reply";
		return false;
	}

	addReply(token, reply);

    qDebug() << DEBUG_PREFIX << "Added" << request.url();
    return true;
}

void HeadQueue::addReply(const QString& token, QNetworkReply* reply)
{
    Q_ASSERT(reply);

    reply->setParent(this);
    reply->setProperty(tokenProperty, token);

	connect(reply, SIGNAL(finished()), this, SLOT(slotHeadFinished()));

    m_networkReplies.insert(token, reply);
}

void HeadQueue::addCommonRequestHeaders(QNetworkRequest& request)
{
	int timestamp = QDateTime::currentDateTime().toTime_t();
	QString strTimestamp = QString::number(timestamp);
	QByteArray appid  = QByteArray("A61201281860668");
	QByteArray appkey = QByteArray("10990ED5D-92AE-4E43-8470-5FCC66A550B3");
	QByteArray secret = QCryptographicHash::hash(appid + appkey + strTimestamp.toLocal8Bit(), QCryptographicHash::Sha1).toHex().constData();

	QString time = strTimestamp.prepend(QLatin1String("."));
	secret.append(QByteArray(time.toAscii()));

	request.setRawHeader("X-YesPo-AppId", appid);
	request.setRawHeader("X-YesPo-AppSecret", secret);
	request.setRawHeader("X-YesPo-AppClient", "partner");
	request.setRawHeader("X-YesPo-AppVer", "v1");
}

void HeadQueue::abort(const QString& token)
{
    QNetworkReply* reply = m_networkReplies.value(token);
    if (!reply)
        return;

    reply->disconnect(this);
    reply->abort();
    reply->deleteLater();

    m_networkReplies.remove(token);
}

void HeadQueue::clear()
{
    if (m_networkReplies.isEmpty())
        return;

    foreach (QNetworkReply* reply, m_networkReplies.values()) {
        reply->disconnect(this);
        reply->abort();
        reply->deleteLater();
    }

    m_networkReplies.clear();
}

void HeadQueue::slotHeadFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply)
		return;

	const QString token = reply->property(tokenProperty).toString();
	HeadResult result;

	if (reply->error()) {
		result.error = Service::translateNetworkError(reply->error());
		result.errorReason = reply->errorString();

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

	m_networkReplies.remove(token);

	Q_EMIT finished(token, result);
}

} // namespace ns

#ifndef DEBUG_PREFIX_EXTERNAL
#  undef DEBUG_PREFIX
#  define DEBUG_PREFIX "DownloadQueue:"
#endif // DEBUG_PREFIX_EXTERNAL

namespace ns {

DownloadResult::DownloadResult() :
    error(Service::NoError)
{
}

DownloadQueue::DownloadQueue(QObject* parent) :
    QObject(parent),
    m_blockSize(0)
{
}

DownloadQueue::~DownloadQueue()
{
    clear();
}

QNetworkAccessManager* DownloadQueue::networkAccessManager() const
{
    return m_networkAccessManager;
}

void DownloadQueue::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    m_networkAccessManager = networkAccessManager;
}

qint64 DownloadQueue::blockSize() const
{
    return m_blockSize;
}

void DownloadQueue::setBlockSize(qint64 blockSize)
{
    m_blockSize = blockSize;
}

void DownloadQueue::setToken(const QString& token) 
{
	m_token = token;
}

void DownloadQueue::setSystemId(const QString& id)
{
	m_systemId = id;
}

bool DownloadQueue::add(const QString& token, const QUrl& url, qint64 from, qint64 to)
{
    QNetworkRequest request(url);
	addCommonRequestHeaders(request);
	request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_systemId).arg(m_token).toAscii()).toBase64());
	
	/*request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");*/

    if (from >= 0) {
        QByteArray rangeHeaderValue = "bytes=";
        rangeHeaderValue += QByteArray::number(from);
        rangeHeaderValue += "-";
        if (to >= 0) {
            if (to < from) {
                qWarning() << DEBUG_PREFIX << "Bytes range not correct";
                return false;
            }
            rangeHeaderValue += QByteArray::number(to);
        }
        request.setRawHeader("Range", rangeHeaderValue);
    }

    return add(token, request);
}

bool DownloadQueue::add(const QString& token, const QNetworkRequest& request)
{
    abort(token);

	if (!m_networkAccessManager) {
		qWarning() << DEBUG_PREFIX << "NetworkAccessManager is not available";
		return false;
	}

	QNetworkReply* reply = m_networkAccessManager->get(request);
	if (!reply) {
		qWarning() << DEBUG_PREFIX << "Failed to get network reply";
		return false;
	}

    addReply(token, reply);
    qDebug() << DEBUG_PREFIX << "Added" << request.url();
    return true;
}

bool DownloadQueue::add(const QString& token, const QNetworkRequest& request, const QByteArray& data)
{
    abort(token);

	if (!m_networkAccessManager) {
		qWarning() << DEBUG_PREFIX << "NetworkAccessManager is not available";
		return false;
	}

    QNetworkReply* reply = m_networkAccessManager->post(request, data);
    if (!reply) {
        qWarning() << DEBUG_PREFIX << "Failed to get network reply";
        return false;
    }

    addReply(token, reply);

    qDebug() << DEBUG_PREFIX << "Added" << request.url();
    return true;
}

bool DownloadQueue::add(const QString& token, const QNetworkRequest& request, QHttpMultiPart* multiPart)
{
	abort(token);

	if (!m_networkAccessManager) {
		qWarning() << DEBUG_PREFIX << "NetworkAccessManager is not available";
		return false;
	}

	QNetworkReply* reply = m_networkAccessManager->post(request, multiPart);
	if (!reply) {
		qWarning() << DEBUG_PREFIX << "Failed to get network reply";
		delete multiPart;
		return false;
	}

	multiPart->setParent(reply);

	addReply(token, reply);

	qDebug() << DEBUG_PREFIX << "Added" << request.url();
	return true;
}

void DownloadQueue::addReply(const QString& token, QNetworkReply* reply)
{
    Q_ASSERT(reply);

    reply->setParent(this);
    reply->setProperty(tokenProperty, token);
    reply->setProperty(blockSizeProperty, m_blockSize);

    connect(reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotDownloadReadyRead()));

    m_networkReplies.insert(token, reply);
}

void DownloadQueue::addCommonRequestHeaders(QNetworkRequest& request)
{
	int timestamp = QDateTime::currentDateTime().toTime_t();
	QString strTimestamp = QString::number(timestamp);
	QByteArray appid  = QByteArray("A61201281860668");
	QByteArray appkey = QByteArray("10990ED5D-92AE-4E43-8470-5FCC66A550B3");
	QByteArray secret = QCryptographicHash::hash(appid + appkey + strTimestamp.toLocal8Bit(), QCryptographicHash::Sha1).toHex().constData();

	QString time = strTimestamp.prepend(QLatin1String("."));
	secret.append(QByteArray(time.toAscii()));

	request.setRawHeader("X-YesPo-AppId", appid);
	request.setRawHeader("X-YesPo-AppSecret", secret);
	request.setRawHeader("X-YesPo-AppClient", "partner");
	request.setRawHeader("X-YesPo-AppVer", "v1");
}

void DownloadQueue::abort(const QString& token)
{
    QNetworkReply* reply = m_networkReplies.value(token);
    if (!reply)
        return;

    reply->disconnect(this);
    reply->abort();
    reply->deleteLater();

    m_networkReplies.remove(token);

    qDebug() << DEBUG_PREFIX << "Aborted" << reply->url();
}

void DownloadQueue::clear()
{
    if (m_networkReplies.isEmpty())
        return;

    foreach (QNetworkReply* reply, m_networkReplies.values()) {
        reply->disconnect(this);
        reply->abort();
        reply->deleteLater();
    }

    m_networkReplies.clear();
}

QList<QNetworkCookie> DownloadQueue::cookies() const
{
	return m_cookies;
}

// void DownloadQueue::setCookies(const QList<QNetworkCookie>& cookies, const QUrl& url)
// {
// 	m_cookies = cookies;
// 	m_networkAccessManager->cookieJar()->setCookiesFromUrl(m_cookies, url);
// }

void DownloadQueue::setCookies(const QUrl& url)
{
	m_networkAccessManager->cookieJar()->setCookiesFromUrl(m_cookies, url);
}

void DownloadQueue::slotDownloadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        return;

    const QString token = reply->property(tokenProperty).toString();
    DownloadResult result;

    if (reply->error()) {
        result.error = Service::translateNetworkError(reply->error());
        result.errorReason = reply->errorString();
    } else {
        result.data = reply->readAll();
    }

    reply->disconnect(this);
    reply->close();
    reply->deleteLater();

    m_networkReplies.remove(token);

    Q_EMIT finished(token, result);
}

void DownloadQueue::slotDownloadReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        return;

    const QString token = reply->property(tokenProperty).toString();
    const qint64 blockSize = reply->property(blockSizeProperty).toLongLong();

    // blockSize == 0 means want to get the whole data once upon finished
    if (blockSize <= 0) {
        return;
    }

    // Not enough data to read
    if (reply->bytesAvailable() < blockSize) {
        return;
    }

    // Read all data and signal, note that the data size could be greater than block size
    const QByteArray data = reply->readAll();
    if (!data.isEmpty()) {
        Q_EMIT dataReady(token, data);
    }
}

} // namespace ns
