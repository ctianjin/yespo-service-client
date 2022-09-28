#ifndef QPID_CAMERASERVICE_P_H
#define QPID_CAMERASERVICE_P_H

#include <QtMultimediaKit/QCameraImageCapture>
#include "cameraservice.h"
#include "abstractservice_p.h"

QT_BEGIN_NAMESPACE
class QCamera;
class QCameraImageCapture;
class QVideoWidget;
class QTimer;
QT_END_NAMESPACE

namespace ns {

class CameraServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(CameraService)

public:
    CameraServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~CameraServicePrivate();

	QList<QByteArray> cameraDevices();

	void start(const QString& deviceName);
	void setVideoWidget(QWidget* videoWidget);
	void stop();
	void setState(int state);
	int state();

	void capture(const QString& fileName);

	void _q_imageCaptured(int id, const QString& fileName);
	void _q_captureError(int id, QCameraImageCapture::Error error, const QString &errorString);
	void _q_cameraError(QCamera::Error e);
	void _q_lookupStatusChanged(QCamera::LockStatus status, QCamera::LockChangeReason reason);
	void _q_lookupStateFocus();

protected:
	virtual void startup();
    virtual void cleanup();

private:
    void init();

	QCamera* m_camera;
	QCameraImageCapture* m_cameraImageCapture;
	QVideoWidget* m_videoWidget;
	int m_state;
};

} // namespace ns

#endif // QPID_SIPSERVICE_P_H
