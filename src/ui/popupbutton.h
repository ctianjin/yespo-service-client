#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

#include <QPushButton>
#include "uiglobal.h"

namespace ns {

class FloatPanel;

class BUILDSYS_UI_DECL PopupButton : public QPushButton
{
    Q_OBJECT
    
public:
    explicit PopupButton(QWidget* parent = 0);
    virtual ~PopupButton(); 

	enum WindowAlign {
		AlignTop = 0,
		AlignBottom = 1
	};

	FloatPanel* popupWindow() const;
	void setPopupWindow(FloatPanel* wnd, WindowAlign align = AlignBottom);
	
Q_SIGNALS:
    

protected:
	

private Q_SLOTS:
	void slotClicked(bool checked);

private:
	FloatPanel* m_popupWindow;
	WindowAlign m_align;

    Q_DISABLE_COPY(PopupButton)
};

} // namespace ns

#endif // POPUPBUTTON_H
