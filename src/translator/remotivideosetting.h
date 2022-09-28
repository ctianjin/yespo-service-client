#ifndef REMOTEVIDEOSETTING_H
#define REMOTEVIDEOSETTING_H

#include <QtCore/QMetaType>
#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include <QWidget>
#include "framelessdialog.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace ns {

class Ui_RemoteVideoSetting;
class FramelessDialog;
class RemoteVideoSetting : public QWidget
{
    Q_OBJECT

public:
	enum RotationStyle {
		RotationNormal = 0,
		RotationStraight,
		RotationRightAngel,
	};

	enum ProportionStyle {
		ProportionNormal = 0,
		Proportion1,
		Proportion2,
		ProportionCovered,
	};	

    explicit RemoteVideoSetting(QWidget* parent = 0);
    ~RemoteVideoSetting();

	void resetCommonControls();
	void setActiveArea(const QRect& rect) {m_activeRect = rect;}

Q_SIGNALS:
	void rotationChanged(ns::RemoteVideoSetting::RotationStyle style);
	void proportionChanged(ns::RemoteVideoSetting::ProportionStyle style);
	void brightnessChanged(int brightness);
protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:
	void slotSetBrightness(int brightness);

	void slotSetRotationNormal();
	void slotSetRotationStraight();
	void slotSetRotationRightAngel();

	void slotSetProportionNormal();
	void slotSetProportion1();
	void slotSetProportion2();
	void slotSetProportionCovered();

private:
	void retranslateTr();
	void createCommonControls();
	QRect m_activeRect;
	int m_brightness;
	RotationStyle m_rotationStyle;
	ProportionStyle m_proportionStyle;
	QScopedPointer<Ui_RemoteVideoSetting> m_ui;

};

} // namespace ns

Q_DECLARE_METATYPE(ns::RemoteVideoSetting::RotationStyle)
Q_DECLARE_METATYPE(ns::RemoteVideoSetting::ProportionStyle)
#endif // REMOTEVIDEOSETTING_H
