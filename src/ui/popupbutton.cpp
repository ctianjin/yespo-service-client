#include <QDebug>
#include "popupbutton.h"
#include "floatpanel.h"

namespace ns {

PopupButton::PopupButton(QWidget* parent) :
    QPushButton(parent),
	m_popupWindow(0),
	m_align(AlignBottom)
{
	connect(this, SIGNAL(clicked(bool)), this, SLOT(slotClicked(bool)));
}

PopupButton::~PopupButton()
{
	
}

FloatPanel* PopupButton::popupWindow() const
{
	return m_popupWindow;
}

void PopupButton::setPopupWindow(FloatPanel* wnd, WindowAlign align)
{
	m_popupWindow = wnd;
	m_align = align;
}

void PopupButton::slotClicked(bool checked)
{
	if (m_popupWindow) {
		if (m_popupWindow->isVisible())
			m_popupWindow->hide();
		else {
			QRect rect = geometry();
			QRect rectPopup = m_popupWindow->geometry();
			QPoint pointCursor = QCursor::pos();

			rect.moveTopLeft(mapToGlobal(rect.topLeft()));

			if (m_align == AlignBottom)
				rect.setTop(rect.top() + rect.height());
			else
				rect.setTop(rect.top() - rectPopup.height());

			rect.setLeft(pointCursor.x()-rectPopup.width()/2);

			rect.setSize(rectPopup.size());

			m_popupWindow->setGeometry(rect);
			m_popupWindow->show();
		}
	}
}

} // namespace ns
