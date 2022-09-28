#include <QDebug>
#include <QMovie>
#include "testcamera.h"
#include "ui_testcamera.h"
#include "phoneapplication.h"
#include "cameraservice.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "CameraService:"
#endif // DEBUG_PREFIX

namespace ns {

TestCamera::TestCamera(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TestCamera),
	m_level(clNormal),
	m_state(-1),
	m_isOpend(false)
{
    m_ui->setupUi(this);
	m_movie = new QMovie(QLatin1String(":/images/ui/loading.gif"));
	m_ui->labelCameraImage->setMovie(m_movie);
}

TestCamera::~TestCamera()
{
	 
}

CameraService* TestCamera::cameraService() const
{
	return m_cameraService;
}

void TestCamera::setCameraService(CameraService* cameraService)
{
	if (m_cameraService) {
		disconnect(m_cameraService, SIGNAL(cameraLoaded()), this, SLOT(slotCameraError()));
		disconnect(m_cameraService, SIGNAL(cameraActive()), this, SLOT(slotCameraActive()));
	}

	m_cameraService = cameraService;

	if (m_cameraService) {
		connect(m_cameraService, SIGNAL(cameraLoaded()), this, SLOT(slotCameraError()));
		connect(m_cameraService, SIGNAL(cameraActive()), this, SLOT(slotCameraActive()));
	}
}

int TestCamera::level() const
{
	return m_level;
}

void TestCamera::startTest()
{
	if(m_movie) {
		m_ui->labelCameraImage->setMovie(m_movie);
		m_movie->start();
	}
	
	m_ui->comboBoxCamera->clear();

	if (m_cameraService) {

		QList<QByteArray> cameraDevices = m_cameraService->cameraDevices();

		qCritical() << DEBUG_PREFIX << cameraDevices;

		if (cameraDevices.size() == 0) {
			m_level = clNoCamera;
			m_ui->labelCameraImage->setText(tr("Your computer does not detect a video device, please try again after installation"));
			return;
		} else {
			m_level = clNormal;
		}

		foreach(const QByteArray& camera, cameraDevices) {
			m_ui->comboBoxCamera->addItem(QString::fromLocal8Bit(camera.constData()), QVariant::fromValue(camera));
		}

		if (cameraDevices.size() > 0) {
			m_isOpend = true;
			m_state = m_cameraService->state();
			if (m_state >= 1) {
				m_movie->stop();
				m_cameraService->setState(++m_state);
				m_ui->labelCameraImage->setText(tr("Unable to open the camera , try closing other programs may use the camera"));
				return ;
			}
			m_cameraService->setVideoWidget(m_ui->labelCameraImage);
			m_cameraService->start(QString::fromLocal8Bit(cameraDevices.at(0).constData()));
		}
	}
}

void TestCamera::finishTest()
{
	if(m_cameraService && m_isOpend) {
		int state = m_cameraService->state();

		if (state <= 1) {
			m_cameraService->stop();
			m_cameraService->setState(0);
		} else if(state > 1) {
			m_cameraService->setState(--state);
		} 

		m_isOpend = false;
	}

	if (m_movie)
		m_movie->stop();
}

void TestCamera::slotCameraError()
{
	m_movie->stop();
	m_ui->labelCameraImage->setText(tr("Unable to open the camera , try closing other programs may use the camera"));
	m_level = clNoCamera;
}

void TestCamera::slotCameraActive()
{
	m_movie->stop();
}

void TestCamera::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

} // namespace ns
