#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include "downloadqueue_p.h"
namespace ns {

class HeadQueue;
class DownloadQueue;
class DownloadThread : public QThread
{
    Q_OBJECT

public:
	static const QString tokenSeparator;
	static const char* tokenProperty;
	explicit DownloadThread(QObject* parent = 0);
	virtual ~DownloadThread();

	void init();
	void uninit();

	HeadQueue* m_fileHeadQueue;
	DownloadQueue* m_fileDownloadQueue;

	void downloadFile(const QString& link, const QString& localFile);
	void download(const QString& link, const QString& localFile);
	void abortDownloadFile(const QString& link, const QString& localFile);
	void setBlockSize(qint64 blocksize);
	void run();
private Q_SLOTS:
	void slotReplyHeadFinished();
	void slotFileHeadFinished(const QString& token, const HeadResult& result);
	void slotFileDownloadDataReady(const QString& token, const QByteArray& data);
	void slotFileDownloadFinished(const QString& token, const DownloadResult& result);

Q_SIGNALS:
	void finished(const QString& token, const HeadResult& result);
	void dataReady(const QString& token, qint64 length);
	void finished(const QString& link, const QString& localFile);
	void failed(const QString& link, const QString& reason);

private:
	QMutex m_mutex;
	bool m_flag;
	QString m_downloadLink;
	QString m_localFile;
	QHash<QString, QString> m_downloadUrls;
	QHash<QString, QNetworkReply*> m_networkReplies;

	static QString makeFileToken(const QString& link, const QString& localFile);
	static QStringList extractFileToken(const QString& token);
};

} // namespace ns

#endif // DOWNLOADTHREAD_H
