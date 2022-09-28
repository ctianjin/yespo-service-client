#ifndef DOWNLOADQUEUE_P_H
#define DOWNLOADQUEUE_P_H

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QUrl>
#include <QtCore/QMutex>
#include <QtNetwork/QNetworkCookie>
#include "service.h"

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QAuthenticator;
class QStringList;
class QUrl;
class QHttpMultiPart;
class QMutex;
QT_END_NAMESPACE

namespace ns {

struct HeadResult
{
    Service::Error error;
    QString errorReason;

    QUrl url;
    qint64 contentLength;
    QDateTime lastModified;
    QByteArray etag;

    HeadResult();
};

class HeadQueue : public QObject
{
    Q_OBJECT

public:
    explicit HeadQueue(QObject* parent = 0);
    virtual ~HeadQueue();

    QNetworkAccessManager* networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

	void setToken(const QString& token);

	void setSystemId(const QString& id);

    bool add(const QString& token, const QUrl& url);
    bool add(const QString& token, const QNetworkRequest& request);
    void abort(const QString& token);
    void clear();

Q_SIGNALS:
    void finished(const QString& token, const HeadResult& result);

private Q_SLOTS:
    void slotHeadFinished();
	void slotReplyFinished(QNetworkReply* reply);
private:
	bool m_finished;
    static const char* tokenProperty;

	QString m_token;
	QString m_systemId;

    void addReply(const QString& token, QNetworkReply* reply);
	void addCommonRequestHeaders(QNetworkRequest& request);

    QPointer<QNetworkAccessManager> m_networkAccessManager;
    QHash<QString, QNetworkReply*> m_networkReplies;

    Q_DISABLE_COPY(HeadQueue)
};

struct DownloadResult
{
    Service::Error error;
    QString errorReason;

    QByteArray data;

    DownloadResult();
};

class DownloadQueue : public QObject
{
    Q_OBJECT

public:
    explicit DownloadQueue(QObject* parent = 0);
    virtual ~DownloadQueue();

    QNetworkAccessManager* networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

    qint64 blockSize() const;
    void setBlockSize(qint64 blockSize);

	void setToken(const QString& token);

	void setSystemId(const QString& id);

    bool add(const QString& token, const QUrl& url, qint64 from = -1, qint64 to = -1);
    bool add(const QString& token, const QNetworkRequest& request);
    bool add(const QString& token, const QNetworkRequest& request, const QByteArray& data);
	bool add(const QString& token, const QNetworkRequest& request, QHttpMultiPart* multiPart);
    void abort(const QString& token);
    void clear();

	QList<QNetworkCookie> cookies() const;
//	void setCookies(const QList<QNetworkCookie>& cookies, const QUrl& url);
	void setCookies(const QUrl& url);

Q_SIGNALS:
    void finished(const QString& token, const DownloadResult& result);
    void dataReady(const QString& token, const QByteArray& data);

private Q_SLOTS:
    void slotDownloadFinished();
    void slotDownloadReadyRead();

private:
    static const char* tokenProperty;
    static const char* blockSizeProperty;

	QString m_token;
	QString m_systemId;

    void addReply(const QString& token, QNetworkReply* reply);
	void addCommonRequestHeaders(QNetworkRequest& request);

    QPointer<QNetworkAccessManager> m_networkAccessManager;
    qint64 m_blockSize;
    QHash<QString, QNetworkReply*> m_networkReplies;
	QList<QNetworkCookie> m_cookies;

    Q_DISABLE_COPY(DownloadQueue)
};

} // namespace ns

#endif // DOWNLOADQUEUE_P_H
