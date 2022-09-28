#include "testdevicestart.h"
#include "ui_testdevicestart.h"
#include "phoneapplication.h"

namespace ns {

TestDeviceStart::TestDeviceStart(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_TestDeviceStart)
{
    m_ui->setupUi(this);
}

TestDeviceStart::~TestDeviceStart()
{
	 
}

void TestDeviceStart::changeEvent(QEvent* event)
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
