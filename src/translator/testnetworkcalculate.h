#ifndef TESTNETWORKCALCULATE_H
#define TESTNETWORKCALCULATE_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace ns {

class Ui_TestNetworkCalculate;

class TestNetworkCalculate : public QWidget
{
    Q_OBJECT

	enum CalculateResult {
		Cal_Good = 0,
		Cal_Smooth,
		Cal_Ordinaryl,
		Cal_bad
	};

public:
    TestNetworkCalculate(QWidget* parent = 0);
    ~TestNetworkCalculate();

	void setIncSize(qint64 size);
	void uninit();
	bool networkStability();
Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent *event);
	virtual void paintEvent(QPaintEvent *event);
private	Q_SLOTS:

private:
	QColor getColor(int index);
	QString getText(int index);
	QPoint calculateLocation(int location, qint64 size);
	QList <QPoint> m_pointList;
	QString m_calculateText;
	int m_speedLow;
	int m_speedNormal;
	int m_speedSmooth;
	int m_speedGood;
	CalculateResult m_calResult;
	qint64 m_incSize;
	int m_location;
	void retranslateTr();
	QScopedPointer<Ui_TestNetworkCalculate> m_ui;
};

} // namespace ns

#endif // TESTNETWORKCALCULATE_H
