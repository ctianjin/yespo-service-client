#include <QDebug>
#include <QTemporaryFile>
#include <QDir>
#include <QTimer>
#include <QTime>
#include "testnetwork.h"
#include "ui_testnetwork.h"
#include "phoneapplication.h"
#include "mediaservice.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "TestNetwork:"
#endif // DEBUG_PREFIX

namespace ns {

const QString TestNetwork::tempFileTemplate = QLatin1String("networktesting.XXXXXX");
const int TestNetwork::speedInterval = 1;
const int TestNetwork::speedTimeout = 15;

TestNetwork::TestNetwork(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TestNetwork),
	m_level(nlNetNotCompleted),
	m_testingFile(NULL),
	m_speedTimer(NULL),
	m_downloadedBytes(0),
	m_lastDownloadedBytes(0),
	m_maxBytes(0),
	m_minBytes(0),
	m_averageBytes(0)
{
    m_ui->setupUi(this);

	m_ui->widgetTesting->hide();
	m_ui->widgetResult->hide();

	if (!m_mediaService) {
		m_mediaService.reset(qApp->createMediaService());
	}

	m_speedTimer = new QTimer(this);
	m_speedTimer->setInterval(speedInterval*1000);
	connect(m_speedTimer, SIGNAL(timeout()), this, SLOT(slotCalcSpeed()));

	m_ui->progressBarTesting->setMaximum(speedTimeout);
	m_ui->progressBarPrepare->setValue(0);
}

TestNetwork::~TestNetwork()
{
 	if (m_mediaService) {
 		m_mediaService.reset();
 	}
}

int TestNetwork::level() const
{
	return m_level;
}

qint64 TestNetwork::averageSpeed() const 
{
	return m_averageBytes;
}

qint64 TestNetwork::maxSpeed() const 
{
	return m_maxBytes;
}

qint64 TestNetwork::minSpeed() const 
{
	return m_minBytes;
}

void TestNetwork::startTest()
{
	if (m_mediaService) {
		connect(m_mediaService.data(), SIGNAL(error(ns::Service::Error)), this, SLOT(slotServiceError()), Qt::QueuedConnection);
		connect(m_mediaService.data(), SIGNAL(fileDownloadLength(const QString&, qint64)), this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		connect(m_mediaService.data(), SIGNAL(fileDownloadBytes(const QString&, qint64)), this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		connect(m_mediaService.data(), SIGNAL(fileDownloadFailed(const QString&, const QString&)), this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		connect(m_mediaService.data(), SIGNAL(fileDownloaded(const QString&, const QString&)), this, SLOT(slotFileDownloaded(const QString&, const QString&)));
		m_mediaService->setBlockSize(8);
		m_mediaService->start();
	}

	m_ui->progressBarTesting->setValue(0);
	m_ui->labelTestingDescription->setText(QLatin1String(""));
	m_ui->widgetPrepare->show();
	m_ui->widgetTesting->hide();
	m_ui->widgetResult->hide();
}

void TestNetwork::finishTest(bool showResult)
{
	if (m_mediaService) {
		if (m_testingFile) {
			m_mediaService->abortDownloadFile(QLatin1String("http://www.yespo.com/download/ToneGoPartner_beta_1.0-win32.exe"), m_testingFile->fileName());
		}
		m_mediaService->stop();
		disconnect(m_mediaService.data(), SIGNAL(fileDownloadLength(const QString&, qint64)), this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		disconnect(m_mediaService.data(), SIGNAL(fileDownloadBytes(const QString&, qint64)), this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		disconnect(m_mediaService.data(), SIGNAL(fileDownloadFailed(const QString&, const QString&)), this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		disconnect(m_mediaService.data(), SIGNAL(fileDownloaded(const QString&, const QString&)), this, SLOT(slotFileDownloaded(const QString&, const QString&)));
//		m_mediaService.reset();
	}

	if (m_speedTimer && m_speedTimer->isActive()) {
		m_speedTimer->stop();
	}

	if (m_testingFile) {
		m_testingFile->deleteLater();
		m_testingFile = NULL;
	}

	m_ui->progressBarTesting->setValue(0);
	m_downloadedBytes = 0;
	m_lastDownloadedBytes = 0;
	if (m_speedArray.size() > 0) {
		qint64 sizePerSecond = 0;
		for (int i = 0; i < m_speedArray.size(); i++) {
			sizePerSecond += m_speedArray.at(i);
		}
		m_averageBytes = sizePerSecond / m_speedArray.size();
		m_speedArray.clear();
	}

	if (!showResult)
		m_ui->widgetCalculate->uninit();
}

void TestNetwork::testingNetwork()
{
	m_level = nlNetNotCompleted;

	qDebug() << DEBUG_PREFIX << m_averageBytes;

	if (!m_mediaService){
		return;
	}

	QDir dir(QDir::tempPath());
	m_testingFile = new QTemporaryFile(dir.absoluteFilePath(tempFileTemplate), this);
	m_testingFile->setAutoRemove(true);
	m_testingFile->open();

	m_downloadedBytes = 0;
	m_lastDownloadedBytes = 0;
	m_averageBytes = 0;
	m_maxBytes = 0;
	m_minBytes = 0;
	m_speedArray.clear();
	m_mediaService->downloadFile(QLatin1String("http://www.yespo.com/download/ToneGoPartner_beta_1.0-win32.exe"), m_testingFile->fileName());

	Q_EMIT testStarted();

	if (m_speedTimer && !m_speedTimer->isActive()) {
		m_speedTimer->start();
	}

	m_ui->widgetPrepare->hide();
	m_ui->widgetTesting->show();
}

void TestNetwork::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		retranslateTr();
		break;
	default:
		break;
	}
}

void TestNetwork::slotServiceError()
{
	if (m_mediaService)
		qWarning() << DEBUG_PREFIX << m_mediaService->error() << m_mediaService->errorText();
	finishTest(true);
	Q_EMIT testFinished(nlNetSpeedLow);
}

void TestNetwork::slotFileDownloadLength(const QString& link, qint64 length)
{
	qDebug() << DEBUG_PREFIX << link << length;
}

void TestNetwork::slotFileDownloadBytes(const QString& link, qint64 bytes)
{
	m_downloadedBytes += bytes;
}

void TestNetwork::slotFileDownloadFailed(const QString& link, const QString& reason)
{
	
}

void TestNetwork::slotFileDownloaded(const QString& link, const QString& localFile)
{

}

void TestNetwork::slotCalcSpeed()
{
	qint64 nowSize = m_downloadedBytes;
	qint64 incSize = nowSize - m_lastDownloadedBytes;
	if (m_maxBytes == 0)
		m_maxBytes = incSize;
	if (m_minBytes == 0)
		m_minBytes = incSize;
	if (incSize > m_maxBytes)
		m_maxBytes = incSize;
	if (incSize < m_minBytes)
		m_minBytes = incSize;
	m_ui->widgetCalculate->setIncSize(incSize);
	m_lastDownloadedBytes = nowSize;
	m_speedArray.append(incSize);

	int seconds = m_speedArray.size() * speedInterval;
	m_seconds = seconds;
	m_ui->progressBarTesting->setValue(seconds);

	const QTime currentTime((seconds / 3600) % 60, (seconds / 60) % 60, seconds % 60);
	QString rateText;
	if (incSize <= 0) {
		rateText = QLatin1String("0KB/s");
	} else if (incSize < 1024*speedInterval) {
		rateText = QString::number(incSize/speedInterval) + QLatin1String("B/s");
	}else if (incSize<(1024*1024*speedInterval)) {
		rateText = QString::number(incSize/(speedInterval*1024.0), 'f', 2) + QLatin1String("KB/s");
	}else {
		rateText = QString::number(incSize/(speedInterval*1024.0*1024.0), 'f', 2) + QLatin1String("MB/s");
	}
	
	QString displayText = tr("Completing: %1% Duration: %2 Speed: %3").arg(
		QString::number(seconds*100/speedTimeout), 
		currentTime.toString(QLatin1String("mm:ss")),
		rateText
		);
	m_ui->labelTestingDescription->setText(displayText);

	if (seconds >= speedTimeout) {
		if (!m_ui->widgetCalculate->networkStability()) {
			Q_EMIT testResult(nlNetSpeedLow);
			m_level = nlNetSpeedLow;
			m_ui->labelResultDescription->setText(tr("Network unstability,could not meet the requirements of the translation,please check the network or try again"));
			m_ui->labelResultDescription->setStyleSheet(QLatin1String("#labelResultDescription{color:#FF6408}"));
			m_ui->labelResultIcon->setStyleSheet(QLatin1String("#labelResultIcon{background-image: url(:/images/ui/test_abnormal.png);}"));
		} else {
			Q_EMIT testResult(nlNormal);
			m_level = nlNormal;
			m_ui->labelResultDescription->setText(tr("Network stability,to meet the requirements of the translation"));
			m_ui->labelResultDescription->setStyleSheet(QLatin1String("#labelResultDescription{color:#65970D}"));
			m_ui->labelResultIcon->setStyleSheet(QLatin1String("#labelResultIcon{background-image: url(:/images/ui/test_normal.png);}"));
		}

		m_ui->widgetTesting->hide();
		m_ui->widgetResult->show();
		finishTest(true);
	}
}

void TestNetwork::retranslateTr()
{
	if (m_ui->widgetCalculate->networkStability()) {
		m_ui->labelResultDescription->setText(tr("Network stability,to meet the requirements of the translation"));
		m_ui->labelResultDescription->setStyleSheet(QLatin1String("#labelResultDescription{color:#65970D}"));
		m_ui->labelResultIcon->setStyleSheet(QLatin1String("#labelResultIcon{background-image: url(:/images/ui/test_normal.png);}"));
	} else {
		m_ui->labelResultDescription->setText(tr("Network unstability,could not meet the requirements of the translation,please check the network or try again"));
		m_ui->labelResultDescription->setStyleSheet(QLatin1String("#labelResultDescription{color:#FF6408}"));
		m_ui->labelResultIcon->setStyleSheet(QLatin1String("#labelResultIcon{background-image: url(:/images/ui/test_abnormal.png);}"));
	}
}

} // namespace ns
