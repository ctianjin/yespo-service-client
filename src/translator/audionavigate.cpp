#include <QDebug>
#include "phoneapplication.h"
#include "audionavigate.h"
#include "audioservice.h"
#include "cameraservice.h"
#include "ui_audionavigate.h"
#include "boxshadow.h"

#ifndef DEBUG_PREFIX
#	define DEBUG_PREFIX "AudioNavigateDialog"
#endif // DEBUG_PREFIX
namespace ns {

AudioNavigateDialog::AudioNavigateDialog(QWidget* parent) :
    FramelessDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_AudioNavigateDialog),
	m_operatingSystem()
{
    m_ui->setupUi(this);   

	BoxShadow *boxShadow = new BoxShadow();
	boxShadow->setBlurRadius(35.0);
	boxShadow->setDistance(12.0);
	boxShadow->setColor(QColor(0, 0, 0, 70));
	setGraphicsEffect(boxShadow);

	m_ui->textBrowserHint->setText(tr("During the test if you have any questions or obstacle, see <a style = \"color: #099AFD\" href=\"#\">User Guide</a>"));
	m_ui->textBrowserHint->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_ui->textBrowserHint->hide();

	m_ui->tabNetQuality->setEnabled(false);
	m_ui->tabAudioStart->setEnabled(false);
	m_ui->tabVoice->setEnabled(false);
	m_ui->tabCamera->setEnabled(false);
	m_ui->tabAudioFinish->setEnabled(false);

	connect(m_ui->testNetwork, SIGNAL(testFinished(int)), this, SLOT(slotNetTestFinished(int)));
	connect(m_ui->testNetwork, SIGNAL(testStarted()), this, SLOT(slotNetTestStarted()));
	connect(m_ui->testNetwork, SIGNAL(testResult(int)), this, SLOT(slotNetTestResult(int)));
	connect(m_ui->testVoice, SIGNAL(testFinished(int)), this, SLOT(slotVoiceTestFinished(int)));
	connect(m_ui->testCamera, SIGNAL(testFinished(int)), this, SLOT(slotCameraTestFinished(int)));
}

AudioNavigateDialog::~AudioNavigateDialog()
{

}

AudioService* AudioNavigateDialog::audioService() const
{
	return m_audioService;
}

void AudioNavigateDialog::setAudioService(AudioService* audioService)
{
	if (m_audioService) {
	}

	m_audioService = audioService;
	m_ui->testVoice->setAudioService(m_audioService);

	if (m_audioService) {
	}
}

CameraService* AudioNavigateDialog::cameraService() const
{
	return m_cameraService;
}

void AudioNavigateDialog::setCameraService(CameraService* cameraService)
{
	if (m_cameraService) {
	}

	m_cameraService = cameraService;
	m_ui->testCamera->setCameraService(m_cameraService);

	if (m_cameraService) {
	}
}

void AudioNavigateDialog::setNetworkAddress(const QString& address)
{
	m_networkAddress = address;
}

void AudioNavigateDialog::initTest()
{
	resetDisplayUi();

	m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testNetwork);	
	m_ui->testNetwork->startTest();
}

void AudioNavigateDialog::uninitTest()
{
	m_ui->testNetwork->finishTest(false);
	m_ui->testVoice->finishTest();
	m_ui->testCamera->finishTest();

	resetDisplayUi();
}

void AudioNavigateDialog::changeEvent(QEvent* event)
{
    FramelessDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
			retranslateTr();
            break;
        default:
            break;
    }
}

void AudioNavigateDialog::hideEvent(QHideEvent *event)
{
	uninitTest();

	getMemeryLoad(m_memeryLoad);
	getProcessorLoad(m_processorLoad);
	getSystemVersion(m_operatingSystem);

	qDebug() << DEBUG_PREFIX << m_operatingSystem << m_memeryLoad << m_processorLoad << m_networkAddress << m_ui->testNetwork->level() << m_ui->testCamera->level()
		<< m_ui->testVoice->level() << m_ui->testNetwork->averageSpeed() << m_ui->testNetwork->maxSpeed() << m_ui->testNetwork->minSpeed();

	FramelessDialog::hideEvent(event);
}

void AudioNavigateDialog::on_pushButtonTestStart_clicked()
{
	m_ui->testNetwork->testingNetwork();
}

void AudioNavigateDialog::on_pushButtonStopTest_clicked()
{
	m_ui->testNetwork->finishTest(false);

	slotNetTestFinished(TestNetwork::nlNetNotCompleted);
}

void AudioNavigateDialog::on_pushButtonNext_clicked()
{
	if (m_ui->stackedWidgetTest->currentWidget() == m_ui->testNetwork) {
		m_ui->tabNetQuality->setChecked(false);
		m_ui->tabAudioStart->setChecked(true);

		m_ui->labelTabForwardNetwork->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_right.png)"));
		m_ui->labelTabForwardStart->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_left.png)"));

		m_ui->testNetwork->finishTest(false);
		m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testDeviceStart);
	} else if (m_ui->stackedWidgetTest->currentWidget() == m_ui->testDeviceStart) {
		
		m_ui->tabAudioStart->setChecked(false);
		m_ui->tabVoice->setChecked(true);
		m_ui->labelTabForwardNetwork->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));
		m_ui->labelTabForwardStart->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_right.png)"));
		m_ui->labelTabForwardVoice->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_left.png)"));

		m_ui->testVoice->startTest();
		m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testVoice);

	} else if(m_ui->stackedWidgetTest->currentWidget() == m_ui->testVoice) {

		slotVoiceTestFinished(m_ui->testVoice->level());

		m_ui->tabVoice->setChecked(false);
		m_ui->tabCamera->setChecked(true);
		m_ui->labelTabForwardStart->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));
		m_ui->labelTabForwardVoice->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_right.png)"));
		m_ui->labelTabForwardCamera->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_left.png)"));

		m_ui->testVoice->finishTest();
		m_ui->testCamera->startTest();
		m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testCamera);

	}else if(m_ui->stackedWidgetTest->currentWidget() == m_ui->testCamera) {

		slotCameraTestFinished(m_ui->testCamera->level());

		m_ui->tabCamera->setChecked(false);
		m_ui->tabAudioFinish->setChecked(true);
		m_ui->labelTabForwardVoice->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));
		m_ui->labelTabForwardCamera->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_right.png)"));

		m_ui->testCamera->finishTest();
		m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testDeviceFinished);
		
		m_ui->pushButtonTestStart->hide();
		m_ui->pushButtonPrev->hide();
		m_ui->pushButtonNext->hide();
		m_ui->pushButtonTestFinish->show();
		m_ui->pushButtonReTest->show();

		m_ui->testDeviceFinished->showResult();
	}
}

void AudioNavigateDialog::on_pushButtonTestFinish_clicked()
{
	uninitTest();
	hide();
}

void AudioNavigateDialog::on_pushButtonReTest_clicked()
{
	initTest();
}

void AudioNavigateDialog::slotNetTestStarted()
{
	m_ui->pushButtonTestStart->hide();
	m_ui->pushButtonStopTest->show();
}

void AudioNavigateDialog::slotNetTestResult(int level)
{
	m_ui->testDeviceFinished->setNetLevel(level);

	m_ui->pushButtonStopTest->hide();
	m_ui->pushButtonPrev->show();
	m_ui->pushButtonNext->show();
}

void AudioNavigateDialog::slotNetTestFinished(int level)
{
	m_ui->testDeviceFinished->setNetLevel(level);

	m_ui->pushButtonStopTest->hide();
	m_ui->pushButtonPrev->show();
	m_ui->pushButtonNext->show();

	m_ui->labelTabForwardNetwork->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_right.png)"));
	m_ui->labelTabForwardStart->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_left.png)"));

	m_ui->tabNetQuality->setChecked(false);
	m_ui->tabAudioStart->setChecked(true);

	m_ui->stackedWidgetTest->setCurrentWidget(m_ui->testDeviceStart);
}

void AudioNavigateDialog::slotVoiceTestFinished(int level)
{
	m_ui->testDeviceFinished->setVoiceLevel(level);
}

void AudioNavigateDialog::slotCameraTestFinished(int level)
{
	m_ui->testDeviceFinished->setCameraLevel(level);
}

void AudioNavigateDialog::retranslateTr()
{
	m_ui->textBrowserHint->setText(tr("During the test if you have any questions or obstacle, see <a style = \"color: #099AFD\" href=\"#\">User Guide</a>"));
}

void AudioNavigateDialog::resetDisplayUi()
{
	m_ui->tabNetQuality->setChecked(true);
	m_ui->tabAudioStart->setChecked(false);
	m_ui->tabVoice->setChecked(false);
	m_ui->tabCamera->setChecked(false);
	m_ui->tabAudioFinish->setChecked(false);

	m_ui->labelTabForwardNetwork->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_active_left.png)"));
	m_ui->labelTabForwardStart->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));
	m_ui->labelTabForwardVoice->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));
	m_ui->labelTabForwardCamera->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_forward_inactive.png)"));

	m_ui->pushButtonTestStart->show();
	m_ui->pushButtonStopTest->hide();
	m_ui->pushButtonPrev->hide();
	m_ui->pushButtonNext->hide();
	m_ui->pushButtonTestFinish->hide();
	m_ui->pushButtonReTest->hide();
}

void AudioNavigateDialog::getSystemVersion(QString& systemVersion)  
{
#ifdef Q_OS_WIN
	QSysInfo::WinVersion version;
	version = QSysInfo::WindowsVersion;
	switch(version)
	{
	case QSysInfo::WV_XP:
		systemVersion = QLatin1String("winXP");
		break;
	case QSysInfo::WV_WINDOWS7:
		systemVersion = QLatin1String("win7");
		break;
	case QSysInfo::WV_WINDOWS8:
		systemVersion = QLatin1String("win8");
		break;
	case QSysInfo::WV_WINDOWS8_1:
		systemVersion = QLatin1String("win8.1");
		break;
// 	case QSysInfo::WV_WINDOWS10:
// 		systemVersion = QLatin1String("win10");
// 		break;
	case QSysInfo::WV_VISTA:
		systemVersion = QLatin1String("vista");
		break; 
	case QSysInfo::WV_2000:
		systemVersion = QLatin1String("win2000");
		break;
	case QSysInfo::WV_2003:
		systemVersion = QLatin1String("win2003");
		break;
	default:
		break;
	}
#endif
}

void AudioNavigateDialog::getMemeryLoad(QString& memeryLoad)
{
// #ifdef Q_OS_WIN
// 	SYSTEM_INFO info;
// 	GetSystemInfo(&info);
// 	PROCESS_MEMORY_COUNTERS_EX pmc;
// 	GetProcessMemoryInfo(GetCurrentProcess(), (PPROCESS_MEMORY_COUNTERS)&pmc, sizeof(pmc));
// 	SIZE_T workingSetsize = pmc.WorkingSetSize;
// 	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
// 	SIZE_T physMemUsedByMe = pmc.PagefileUsage;
// 	memeryLoad = QString::number(virtualMemUsedByMe / 1024);
// #endif
}

void AudioNavigateDialog::getProcessorLoad(QString& processorLoad)
{
#ifdef Q_OS_WIN
	winProcessInit();

	HANDLE hEvent;
	hEvent = CreateEvent (NULL,FALSE,FALSE,NULL); 
	WaitForSingleObject( hEvent,10); 

	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	percent *= 100;

	processorLoad = QString::number(percent);
	processorLoad.append(QLatin1String("%"));

	CloseHandle(hEvent);
#endif
}

void AudioNavigateDialog::winProcessInit()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

} // namespace ns
