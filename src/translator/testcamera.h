#ifndef QPID_TESTCAMERA_H
#define QPID_TESTCAMERA_H

#include <QtCore/QScopedPointer>
#include <QPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class CameraService;
class Ui_TestCamera;

class TestCamera : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(CameraService* cameraService READ cameraService WRITE setCameraService)

public:
	enum CameraLevel {
		clNormal = 0,
		clNoCamera = 1
	};

    TestCamera(QWidget* parent = 0);
    ~TestCamera();

	CameraService* cameraService() const;
	void setCameraService(CameraService* cameraService);

	int level() const;

public Q_SLOTS:
	void startTest();
	void finishTest();
	void slotCameraError();
	void slotCameraActive();

Q_SIGNALS:
	void testFinished(int level);

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:

private:
	int m_level;
	int m_state;
	QMovie* m_movie;
	bool m_isOpend;

	QPointer<CameraService> m_cameraService;
	QScopedPointer<Ui_TestCamera> m_ui;
};

} // namespace ns

#endif // QPID_TESTCAMERA_H
