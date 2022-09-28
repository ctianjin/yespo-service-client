#ifndef TESTDEVICEFINISHED_H
#define TESTDEVICEFINISHED_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_TestDeviceFinished;

class TestDeviceFinished : public QWidget
{
    Q_OBJECT

public:
    TestDeviceFinished(QWidget* parent = 0);
    ~TestDeviceFinished();

	void setNetLevel(int level);
	void setVoiceLevel(int level);
	void setCameraLevel(int level);
	void showResult();

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent * event);

private	Q_SLOTS:

private:
	int m_netLevel;
	int m_voiceLevel;
	int m_cameraLevel;

	QScopedPointer<Ui_TestDeviceFinished> m_ui;
};

} // namespace ns

#endif // TESTDEVICEFINISHED_H
