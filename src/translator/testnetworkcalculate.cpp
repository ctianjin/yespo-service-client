#include "testnetworkcalculate.h"
#include "ui_testnetworkcalculate.h"
#include <QDebug>
#include <qmath.h>
#include <QPainter>

#define  MARGIN 40

#ifndef DEBUG_PREFIX
#	define DEBUG_PREFIX "TestNetworkCalculate:"
#endif // DEBUG_PREFIX

#define LineLength 400
#define LineHeight 200

#define CalculateValueBad 128
#define CalculateValueOrdinary 256
#define CalculateValueSmooth 512
#define CalculateValueGood 1024
namespace ns {

TestNetworkCalculate::TestNetworkCalculate(QWidget* parent) :
    QWidget(parent),
	m_incSize(0),
	m_location(1),
	m_calResult(Cal_Good),
	m_speedLow(0),
	m_speedNormal(0),
	m_speedSmooth(0),
	m_speedGood(0),
    m_ui(new Ui_TestNetworkCalculate)
{
    m_ui->setupUi(this);
	
 	m_pointList.append(QPoint(MARGIN, LineHeight + MARGIN));
// 	m_pointList.append(QPoint(MARGIN * 4, m_size.height() - MARGIN * 4));
// 	m_pointList.append(QPoint(MARGIN * 3, m_size.height() - MARGIN * 3));
}

TestNetworkCalculate::~TestNetworkCalculate()
{
	 
}

QColor TestNetworkCalculate::getColor(int index)
{
	QColor color(Qt::transparent);
	switch(index) {
		case 1:
			color.setRgb(29, 150, 15);
			break;
		case 2:
			color.setRgb(161, 173, 17);
			break;
		case 3:
			color.setRgb(254, 151, 16);
			break;
		case 4:
			color.setRgb(253, 3, 2);
			break;
		default:
			break;
	}
	return color;
}

QString TestNetworkCalculate::getText(int index)
{
	QString text = QString();
	switch(index) {
		case 1:
			text = tr("Good");
			break;
		case 2:
			text = tr("Smooth");
			break;
		case 3:
			text = tr("Ordinary");
			break;
		case 4:
			text = tr("Bad");
		default:
			break;
	}
	return text;
}

QPoint TestNetworkCalculate::calculateLocation(int location, qint64 size) 
{
	QPoint pointCalculate = QPoint();

	if (size <= 0) {
		pointCalculate = QPoint(MARGIN * location, LineHeight + MARGIN);
		++m_speedLow;
	} else if (size < 1024 * CalculateValueBad) {
		pointCalculate = QPoint(MARGIN * location, LineHeight + MARGIN / 2);
		++m_speedLow;
	} else if (size < 1024 * CalculateValueOrdinary) {
		pointCalculate = QPoint(MARGIN * location, LineHeight - MARGIN / 2);
		++m_speedNormal;
	} else if (size < 1024 * CalculateValueSmooth) {
		pointCalculate = QPoint(MARGIN * location, LineHeight - MARGIN * 3 / 2);
		++m_speedSmooth;
	} else if (size < 1024 * CalculateValueGood) {
		pointCalculate = QPoint(MARGIN * location, LineHeight  - MARGIN * 5 / 2);
		++m_speedGood;
	} else {
		pointCalculate = QPoint(MARGIN * location, LineHeight - MARGIN * 7 / 2);
		++m_speedGood;
	}
	return pointCalculate;
}

void TestNetworkCalculate::uninit()
{
	m_pointList.clear();
	m_pointList.append(QPoint(MARGIN, LineHeight + MARGIN));
	m_location = 1;
	m_speedLow = 0;
	m_speedNormal = 0;
	m_speedSmooth = 0;
	m_speedGood = 0;
	m_calResult = Cal_Good;
	QPainter painter(this);
	painter.eraseRect(geometry());
}

bool TestNetworkCalculate::networkStability()
{
	if (m_calResult <= Cal_Ordinaryl)
		return true;
	else
		return false;
}

void TestNetworkCalculate::setIncSize(qint64 size)
{
	if (m_pointList.size() >= 10) {
		return;
	}
	++m_location;

	m_incSize = size;

	m_pointList.append(calculateLocation(m_location, m_incSize));

	if (m_pointList.size() >= 10) {
		if (m_speedGood > qMax(m_speedLow, qMax(m_speedNormal, m_speedSmooth))) {
			m_calResult = Cal_Good;
		} else if (m_speedSmooth > qMax(m_speedGood, qMax(m_speedNormal, m_speedLow))) {
			m_calResult = Cal_Smooth;
		} else if (m_speedNormal > qMax(m_speedGood, qMax(m_speedLow, m_speedSmooth))) {
			m_calResult = Cal_Ordinaryl;
		} else if (m_speedLow > qMax(m_speedGood, qMax(m_speedNormal, m_speedSmooth)))
			m_calResult = Cal_bad;
	}
}

void TestNetworkCalculate::changeEvent(QEvent* event)
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

void TestNetworkCalculate::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	qreal widthSpread = LineHeight / 5;
	qreal heightSpread = LineLength / 10;

	for (int i = 0; i < 5; i++) {
		if (i == 0)
			continue;
		else
			painter.setPen(QColor(228, 228, 228));
		painter.drawLine(MARGIN, MARGIN + widthSpread* i, LineLength + MARGIN, MARGIN + widthSpread * i);
		if (i >= 1) {
			painter.setPen(QColor(175, 175, 175));
			if (i == 1)
				painter.drawText(0, MARGIN * (i + 1) + 5, QString(QLatin1String("%1MB")).arg(1));
			if (i == 2)
				painter.drawText(0, MARGIN * (i + 1) + 5, QString(QLatin1String("%1KB")).arg(512));
			if (i == 3)
				painter.drawText(0, MARGIN * (i + 1) + 5, QString(QLatin1String("%1KB")).arg(256));
			if (i == 4)
				painter.drawText(0, MARGIN * (i + 1) + 5, QString(QLatin1String("%1KB")).arg(128));
			painter.setPen(getColor(i));
			painter.drawText(LineLength + MARGIN + 5, MARGIN * (i + 1) + 5, getText(i));
		}
	}

	for(int i = 0; i < 10; i++) {
		if (i == 9)
			continue;
		else {
			painter.setPen(QColor(228, 228, 228));
			painter.drawLine(MARGIN*2 + heightSpread * i, MARGIN, MARGIN*2 + heightSpread * i, LineHeight + MARGIN);
			painter.setPen(QColor(175, 175, 175));
			painter.drawText(QPoint(MARGIN*2 + heightSpread * i - 3, LineHeight + MARGIN + 15), QString::number(i + 1));
		}
	}

	QPen pen;
	pen.setWidth(2);
	pen.setColor(QColor(13, 155, 255));
	painter.setPen(pen);

	painter.drawLine(MARGIN, LineHeight + MARGIN, LineLength + MARGIN,  LineHeight + MARGIN);
	painter.drawLine(MARGIN, MARGIN, MARGIN, LineHeight + MARGIN);

	if (m_pointList.size() > 0) {
		for (int i = 0; i < m_pointList.size(); i++) {
			const QPoint eclipse = m_pointList.at(i);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.drawEllipse(eclipse, 4, 4);
			if (i > 0) {
				QPainterPath path;
				QPen penPath;
				penPath.setWidth(3);
				penPath.setColor(QColor(13, 155, 255));
				path.moveTo(eclipse);
				QPoint pointLineTo = m_pointList.at(i - 1);
				path.lineTo(pointLineTo);
				/*painter.setRenderHint(QPainter::Antialiasing, true);*/
				painter.strokePath(path, penPath);
				painter.setRenderHint(QPainter::Antialiasing, false);
			}
		}
	}
}

void TestNetworkCalculate::retranslateTr()
{
	
}

} // namespace ns
