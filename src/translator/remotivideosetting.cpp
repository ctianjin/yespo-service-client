#include "remotivideosetting.h"
#include "ui_remotivideosetting.h"

#ifndef DEBUG_PREFIX
#	define DEBUG_PREFIX "RemoteVideoSetting:"
#endif // DEBUG_PREFIX
namespace ns {

RemoteVideoSetting::RemoteVideoSetting(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_RemoteVideoSetting)
{
    m_ui->setupUi(this);	

	setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);

	createCommonControls();
}

RemoteVideoSetting::~RemoteVideoSetting()
{
	 
}

void RemoteVideoSetting::createCommonControls()
{
	connect(m_ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSetBrightness(int)));

	connect(m_ui->pushButtonStraight, SIGNAL(clicked()), this, SLOT(slotSetRotationStraight()));
	connect(m_ui->pushButtonRightangle, SIGNAL(clicked()), this, SLOT(slotSetRotationRightAngel()));

	connect(m_ui->radioButtonNormal, SIGNAL(clicked()), this, SLOT(slotSetProportionNormal()));
	connect(m_ui->radioButtonPorprotion1, SIGNAL(clicked()), this, SLOT(slotSetProportion1()));
	connect(m_ui->radioButtonProportion2, SIGNAL(clicked()), this, SLOT(slotSetProportion2()));
	connect(m_ui->radioButtonCovered, SIGNAL(clicked()), this, SLOT(slotSetProportionCovered()));
}

void RemoteVideoSetting::resetCommonControls()
{
	m_ui->horizontalSlider->blockSignals(true);
	m_ui->horizontalSlider->setValue(50);
	m_ui->horizontalSlider->blockSignals(false);

	m_ui->radioButtonNormal->setChecked(true);
}

void RemoteVideoSetting::changeEvent(QEvent* event)
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

void RemoteVideoSetting::slotSetBrightness(int brightness)
{
	m_brightness = brightness;
	Q_EMIT brightnessChanged(m_brightness);
}

void RemoteVideoSetting::slotSetRotationNormal()
{
	m_rotationStyle = RotationNormal;

	Q_EMIT rotationChanged(m_rotationStyle);
}

void RemoteVideoSetting::slotSetRotationStraight()
{
	m_rotationStyle = RotationStraight;

	Q_EMIT rotationChanged(m_rotationStyle);
}

void RemoteVideoSetting::slotSetRotationRightAngel()
{
	m_rotationStyle = RotationRightAngel;

	Q_EMIT rotationChanged(m_rotationStyle);
}

void RemoteVideoSetting::slotSetProportionNormal()
{
	m_proportionStyle = ProportionNormal;

	Q_EMIT proportionChanged(m_proportionStyle);
}

void RemoteVideoSetting::slotSetProportion1()
{
	m_proportionStyle = Proportion1;

	Q_EMIT proportionChanged(m_proportionStyle);
}

void RemoteVideoSetting::slotSetProportion2()
{
	m_proportionStyle = Proportion2;

	Q_EMIT proportionChanged(m_proportionStyle);
}

void RemoteVideoSetting::slotSetProportionCovered()
{
	m_proportionStyle = ProportionCovered;

	Q_EMIT proportionChanged(m_proportionStyle);
}

void RemoteVideoSetting::retranslateTr()
{

}

} // namespace ns
