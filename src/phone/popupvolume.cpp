#include <QProxyStyle>
#include "signalblocker.h"
#include "popupvolume.h"
#include "phoneapplication.h"
#include "ui_popupvolume.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PopupVolume:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

namespace {

	class SliderProxyStyle : public QProxyStyle
	{
	public:
		explicit SliderProxyStyle(QStyle* baseStyle = 0) :
		QProxyStyle(baseStyle)
		{
		}

		virtual int styleHint(QStyle::StyleHint hint, const QStyleOption* option = 0, const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const
		{
			if (hint == QStyle::SH_Slider_AbsoluteSetButtons) {
				return (Qt::LeftButton | Qt::MidButton);
			} else if (hint == QStyle::SH_Slider_PageSetButtons) {
				return Qt::RightButton;
			} else {
				return QProxyStyle::styleHint(hint, option, widget, returnData);
			}
		}
	};

} // Anonymous namespace

struct PopupVolumeOptions
{

};

PopupVolume::PopupVolume(QWidget* parent) :
    FloatPanel(parent),
    m_ui(new Ui_PopupVolume),
    m_opt(new PopupVolumeOptions)
{
//#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
//#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	// XXX Trick for make left click on slider jump to the position of the mouse
	{
		QStyle* style = new SliderProxyStyle(m_ui->verticalSlider->style());
		style->setParent(this);
		m_ui->verticalSlider->setStyle(style);
	}

	setMaximumWidth(17);
}

PopupVolume::~PopupVolume()
{

}

void PopupVolume::setVolume(int volume)
{
	if (!m_ui->verticalSlider->isSliderDown()) {
		SignalBlocker blocker(m_ui->verticalSlider);
		m_ui->verticalSlider->setValue(volume);
	}
}

void PopupVolume::changeEvent(QEvent* event)
{
	FloatPanel::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PopupVolume::on_verticalSlider_valueChanged()
{
	Q_EMIT volumeChanged(m_ui->verticalSlider->value());
}


} // namespace Qpid
