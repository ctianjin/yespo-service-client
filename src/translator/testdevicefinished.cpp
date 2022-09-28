#include "testdevicefinished.h"
#include "ui_testdevicefinished.h"
#include "phoneapplication.h"

#include "testnetwork.h"
#include "testvoice.h"
#include "testcamera.h"

namespace ns {

TestDeviceFinished::TestDeviceFinished(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TestDeviceFinished)
{
    m_ui->setupUi(this);
}

TestDeviceFinished::~TestDeviceFinished()
{
	 
}

void TestDeviceFinished::setNetLevel(int level)
{
	m_netLevel = level;

	if (m_netLevel == TestNetwork::nlNetNotCompleted) {

		m_ui->labelNetworkResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
		m_ui->labelNetQualityResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
		m_ui->labelNetworkResultValue->setText(tr("Not completed"));
		m_ui->labelNetQualityResultValue->setText(tr("Not completed"));

	} else if (m_netLevel == TestNetwork::nlNormal) {

		m_ui->labelNetworkResultValue->setStyleSheet(QLatin1String("color: #4EB702;"));
		m_ui->labelNetQualityResultValue->setStyleSheet(QLatin1String("color: #4EB702;"));
		m_ui->labelNetworkResultValue->setText(tr("Good"));
		m_ui->labelNetQualityResultValue->setText(tr("Stable"));

	} else {

		if (m_netLevel & TestNetwork::nlNetSpeedLow) {
			m_ui->labelNetworkResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
			m_ui->labelNetworkResultValue->setText(tr("Low"));
			m_ui->labelNetQualityResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
			m_ui->labelNetQualityResultValue->setText(tr("Unstable"));
		}

// 		if (m_netLevel & TestNetwork::nlNetUnStable) {
// 			m_ui->labelNetQualityResultValue->setStyleSheet(QLatin1String("color: #FF6600;"));
// 			m_ui->labelNetQualityResultValue->setText(tr("Unstable"));
// 		}
	}
}

void TestDeviceFinished::setVoiceLevel(int level)
{
	m_voiceLevel = level;

	m_ui->labelMicResultValue->setStyleSheet(QLatin1String("color: #4EB702;"));
	m_ui->labelSpeakerResultValue->setStyleSheet(QLatin1String("color: #4EB702;"));
	m_ui->labelMicResultValue->setText(tr("Normal"));
	m_ui->labelSpeakerResultValue->setText(tr("Normal"));

	if (m_voiceLevel&TestVoice::vlNoSpeaker) {

		m_ui->labelSpeakerResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
		m_ui->labelSpeakerResultValue->setText(tr("No device"));

	}

	if (m_voiceLevel&TestVoice::vlNoMicrophone) {

		m_ui->labelMicResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
		m_ui->labelMicResultValue->setText(tr("No device"));

	}
}

void TestDeviceFinished::setCameraLevel(int level)
{
	m_cameraLevel = level;

	if (m_cameraLevel == TestCamera::clNormal) {
		m_ui->labelCameraResultValue->setStyleSheet(QLatin1String("color: #4EB702;"));
		m_ui->labelCameraResultValue->setText(tr("Normal"));
	} else {
		m_ui->labelCameraResultValue->setStyleSheet(QLatin1String("color: #E72E2F;"));
		m_ui->labelCameraResultValue->setText(tr("No device"));
	}
	
}

void TestDeviceFinished::showResult()
{
	if (m_cameraLevel==TestCamera::clNormal && m_voiceLevel==TestVoice::vlNormal && m_netLevel==TestNetwork::nlNormal) {
		m_ui->labelTestIcon->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_normal.png);"));
		m_ui->labelTestResultTitle->setStyleSheet(QLatin1String("color: #669801;"));
		m_ui->labelTestResultTitle->setText(tr("The test is completed, your computer can provide translation services to normal"));
	} else {
		m_ui->labelTestIcon->setStyleSheet(QLatin1String("background-image: url(:/images/ui/test_abnormal.png);"));
		m_ui->labelTestResultTitle->setStyleSheet(QLatin1String("color: #FF6600;"));
		m_ui->labelTestResultTitle->setText(tr("The test is completed, your computer can't provide translation services to normal"));
	}
}

void TestDeviceFinished::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}


} // namespace ns
