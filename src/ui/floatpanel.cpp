#include <QDebug>
#include <QHideEvent>
#include "floatpanel.h"

namespace ns {

FloatPanel::FloatPanel(QWidget* parent) :
	QWidget(parent, Qt::FramelessWindowHint | Qt::Popup)
{

}

FloatPanel::~FloatPanel()
{
}

void FloatPanel::hideEvent(QHideEvent *event)
{
	QWidget::hideEvent(event);

	Q_EMIT floatHide();
}

} // namespace ns
