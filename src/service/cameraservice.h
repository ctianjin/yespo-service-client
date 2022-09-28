#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include "abstractservice.h"
#include "serviceglobal.h"

namespace ns {

class CameraServicePrivate;

class BUILDSYS_SERVICE_DECL CameraService : public AbstractService
{
    Q_OBJECT

public:
    explicit CameraService(ServiceManager* manager, QObject* parent = 0);
    CameraService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~CameraService();

	QList<QByteArray> cameraDevices();

	void start(const QString& deviceName);
	void setVideoWidget(QWidget* videoWidget);
	void stop();
	void setState(int state);
	int state();

	void capture(const QString& fileName);

public Q_SLOTS:

Q_SIGNALS:
	void captureFinished(const QString& fileName);
	void cameraLoaded();
	void cameraActive();

protected:
    CameraService(CameraServicePrivate& dd, QObject* parent = 0);

	virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
	Q_PRIVATE_SLOT(d_func(), void _q_imageCaptured(int, const QString&))
	Q_PRIVATE_SLOT(d_func(), void _q_captureError(int, QCameraImageCapture::Error, const QString&))
	Q_PRIVATE_SLOT(d_func(), void _q_cameraError(QCamera::Error))
	Q_PRIVATE_SLOT(d_func(), void _q_lookupStatusChanged(QCamera::LockStatus status, QCamera::LockChangeReason reason))
	Q_PRIVATE_SLOT(d_func(), void _q_lookupStateFocus())

    Q_DECLARE_PRIVATE(CameraService)
    Q_DISABLE_COPY(CameraService)
};

} // namespace ns

#endif // CAMERASERVICE_H
