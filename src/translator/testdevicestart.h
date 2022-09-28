#ifndef TESTDEVICESTART_H
#define TESTDEVICESTART_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_TestDeviceStart;

class TestDeviceStart : public QWidget
{
    Q_OBJECT

public:
    TestDeviceStart(QWidget* parent = 0);
    ~TestDeviceStart();

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:

private:

	QScopedPointer<Ui_TestDeviceStart> m_ui;
};

} // namespace ns

#endif // TESTDEVICESTART_H
