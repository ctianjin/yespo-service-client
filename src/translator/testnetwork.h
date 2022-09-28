#ifndef TESTNETWORK_H
#define TESTNETWORK_H

#include <QtCore/QScopedPointer>
#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class QTemporaryFile;
class QTimer;
QT_END_NAMESPACE

namespace ns {

class Ui_TestNetwork;
class MediaService;

class TestNetwork : public QWidget
{
    Q_OBJECT

public:
	enum NetWorkLevel {
		nlNetNotCompleted = 0,
		nlNetSpeedLow = 1,
		nlNetUnStable = 2,
		nlNormal = 4
	};

	static const QString tempFileTemplate;
	static const int speedInterval;
	static const int speedTimeout;

    TestNetwork(QWidget* parent = 0);
    ~TestNetwork();

	int level() const;
	qint64 averageSpeed() const;
	qint64 maxSpeed() const;
	qint64 minSpeed() const;

public Q_SLOTS:
	void startTest();
	void finishTest(bool showResult);
	void testingNetwork();

Q_SIGNALS:
	void testStarted();
	void testFinished(int level);
	void testResult(int level);
protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:
	void slotServiceError();
	void slotFileDownloadLength(const QString& link, qint64 length);
	void slotFileDownloadBytes(const QString& link, qint64 bytes);
	void slotFileDownloadFailed(const QString& link, const QString& reason);
	void slotFileDownloaded(const QString& link, const QString& localFile);

	void slotCalcSpeed();

private:
	void retranslateTr();
	int m_level;
	int m_seconds;
	QTemporaryFile* m_testingFile;
	QTimer* m_speedTimer;
	qint64 m_totalBytes;
	qint64 m_downloadedBytes;
	qint64 m_lastDownloadedBytes;
	qint64 m_averageBytes;
	qint64 m_maxBytes;
	qint64 m_minBytes;
	QList<qint64> m_speedArray;

	QScopedPointer<MediaService> m_mediaService;
	QScopedPointer<Ui_TestNetwork> m_ui;
};

} // namespace ns

#endif // TESTNETWORK_H
