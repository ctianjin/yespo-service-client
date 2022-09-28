#include <QDebug>
#include <QtMultimediaKit/QCamera>
#include <QtMultimediaKit/QVideoWidget>
#include <QTimer>
#include <QBitmap>
#include "cameraservice.h"
#include "cameraservice_p.h"
#include "servicemanager.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "CameraService:"
#endif // DEBUG_PREFIX

namespace ns {

CameraServicePrivate::CameraServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
	m_camera(0),
	m_cameraImageCapture(0),
	m_videoWidget(0),
	m_state(0)
{
	
}

CameraServicePrivate::~CameraServicePrivate()
{
	cleanup();
}

void CameraServicePrivate::init()
{
    Q_Q(CameraService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

	m_videoWidget = new QVideoWidget();
}

QList<QByteArray> CameraServicePrivate::cameraDevices()
{
	return QCamera::availableDevices();
}

void CameraServicePrivate::start(const QString& deviceName)
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	if (m_camera != NULL) 
		stop();

	++m_state;
	if (m_state > 1)
		return;
	
	m_camera = new QCamera(deviceName.toLocal8Bit());	

	_q_lookupStatusChanged(m_camera->lockStatus(), QCamera::UserRequest);

	if (m_camera) {
		qDebug() << DEBUG_PREFIX << m_camera->state();
		m_cameraImageCapture = new QCameraImageCapture(m_camera);
		m_cameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

		QObject::connect(m_cameraImageCapture, SIGNAL(imageSaved(int, const QString&)), q, SLOT(_q_imageCaptured(int, const QString&)));
		QObject::connect(m_cameraImageCapture, SIGNAL(error(int, QCameraImageCapture::Error, const QString&)), 
			q, SLOT(_q_captureError(int, QCameraImageCapture::Error, const QString&)));
		QObject::connect(m_camera, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)), 
			q, SLOT(_q_lookupStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)));
		QObject::connect(m_camera, SIGNAL(error(QCamera::Error)), q, SLOT(_q_cameraError(QCamera::Error)));

		m_camera->start();
		QTimer::singleShot(1000, q, SLOT(_q_lookupStateFocus()));
	}
}

void CameraServicePrivate::_q_lookupStateFocus()
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	if (!m_camera) {
		qDebug() << DEBUG_PREFIX << "Camera has been stopped!";
		return;
	}

	switch (m_camera->state())
	{
	case QCamera::LoadedState:
		qDebug() << DEBUG_PREFIX << "loaded";
		Q_EMIT q->cameraLoaded();
		break;
	case QCamera::ActiveState:
		qDebug() << DEBUG_PREFIX << "active";
		m_camera->setViewfinder(m_videoWidget);
		Q_EMIT q->cameraActive();
		break;
	case QCamera::UnloadedState:
		qDebug() << DEBUG_PREFIX << "unload";
		break;
	}
}

void CameraServicePrivate::setVideoWidget(QWidget* videoWidget)
{
	Q_ASSERT(videoWidget);

	if (videoWidget != m_videoWidget->parent()) {
 		QSize videoSize = videoWidget->size();
		m_videoWidget->setParent(videoWidget);
		m_videoWidget->setGeometry(0,0,videoSize.width(), videoSize.height());	
	}
}

void CameraServicePrivate::stop()
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	if (m_camera != NULL) {

		QObject::disconnect(m_cameraImageCapture, SIGNAL(imageSaved(int, const QString&)), q, SLOT(_q_imageCaptured(int, const QString&)));
		QObject::disconnect(m_cameraImageCapture, SIGNAL(error(int, QCameraImageCapture::Error, const QString&)), 
			q, SLOT(_q_captureError(int, QCameraImageCapture::Error, const QString&)));
		QObject::disconnect(m_camera, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)), 
			q, SLOT(_q_lookupStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)));
		QObject::disconnect(m_camera, SIGNAL(error(QCamera::Error)), q, SLOT(_q_cameraError(QCamera::Error)));

		m_camera->stop();

		m_cameraImageCapture->deleteLater();
		m_cameraImageCapture = NULL;

		m_camera->deleteLater();
		m_camera = NULL;
	}
}

void CameraServicePrivate::setState(int state)
{
	m_state = state;
}

int CameraServicePrivate::state() 
{
	return m_state;
}

void CameraServicePrivate::capture(const QString& fileName)
{
	if (m_cameraImageCapture)
		m_cameraImageCapture->capture(fileName);
}

void CameraServicePrivate::_q_imageCaptured(int id, const QString& fileName)
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	Q_EMIT q->captureFinished(fileName);
}

void CameraServicePrivate::_q_captureError(int id, QCameraImageCapture::Error error, const QString &errorString)
{
	qDebug() << DEBUG_PREFIX << id << error << errorString;
}

void CameraServicePrivate::_q_cameraError(QCamera::Error e)
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	switch (e) {
	case QCamera::NoError:
		{
			break;
		}
	case QCamera::CameraError:
		{
			qDebug() <<"QCameraExample", "General Camera error";
			break;
		}
	case QCamera::InvalidRequestError:
		{
			qDebug() << "QCameraExample", "Camera invalid request error";
			break;
		}
	case QCamera::ServiceMissingError:
		{
			qDebug() << "QCameraExample", "Camera service missing error";
			break;
		}
	case QCamera::NotSupportedFeatureError :
		{
			qDebug() << "QCameraExample", "Camera not supported error";
			break;
		}
	};
}

void CameraServicePrivate::_q_lookupStatusChanged(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	switch (status)
	{
	case QCamera::Searching:
		qDebug() << DEBUG_PREFIX << "updateLockStatus Searching";
		break;
	case QCamera::Locked:
		qDebug() << DEBUG_PREFIX << "updateLockStatus Locked" << m_camera->isAvailable();
		break;
	case QCamera::Unlocked:
		qDebug() << DEBUG_PREFIX << "updateLockStatus Unlocked";
	}
}

void CameraServicePrivate::startup()
{
	Q_Q(CameraService);
	Q_ASSERT(q);

}

void CameraServicePrivate::cleanup()
{
	Q_Q(CameraService);
	Q_ASSERT(q);

	stop();

	setState(Service::StoppedState);

    AbstractServicePrivate::cleanup();
}


CameraService::CameraService(ServiceManager* manager, QObject* parent) :
    AbstractService(*new CameraServicePrivate(manager, QString()), parent)
{
    Q_D(CameraService);

    d->init();
}

CameraService::CameraService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractService(*new CameraServicePrivate(manager, name), parent)
{
    Q_D(CameraService);

    d->init();
}

CameraService::CameraService(CameraServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
    Q_D(CameraService);

    d->init();
}

QList<QByteArray> CameraService::cameraDevices()
{
	Q_D(CameraService);

	return d->cameraDevices();
}

void CameraService::start(const QString& deviceName)
{
	Q_D(CameraService);

	d->start(deviceName);
}

void CameraService::setVideoWidget(QWidget* videoWidget)
{
	Q_D(CameraService);

	d->setVideoWidget(videoWidget);
}

void CameraService::stop()
{
	Q_D(CameraService);

	d->stop();
}

void CameraService::setState(int state)
{
	Q_D(CameraService);

	d->setState(state);
}

int CameraService::state()
{
	Q_D(CameraService);

	return d->state();
}

void CameraService::capture(const QString& fileName)
{
	Q_D(CameraService);

	d->capture(fileName);
}

CameraService::~CameraService()
{
}

void CameraService::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(CameraService);

//     typedef CameraServiceOptions Opt;
//     Opt& opt = d->m_opt;
    Preference* pref = preference();
}

} // namespace ns

#include "moc_cameraservice.cpp"
