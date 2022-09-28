#ifndef FRAMELESSMAINWINDOW_H
#define FRAMELESSMAINWINDOW_H

#include <QtCore/QScopedPointer>
#include "mainwindow.h"
#include "uiglobal.h"

namespace ns {

class FramelessHelper;

class BUILDSYS_UI_DECL FramelessMainWindow : public MainWindow
{
    Q_OBJECT

public:
    explicit FramelessMainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FramelessMainWindow();

protected:
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent (QMouseEvent * event );
	virtual void mouseMoveEvent (QMouseEvent * event );
   // FramelessHelper* framelessHelper() const;
Q_SIGNALS:
	void notifyMove(const QPoint& point);
private:
	QPixmap m_pixMask;
	bool leftButtonPressed;
	QPoint dragPos;
  //  QScopedPointer<FramelessHelper> m_framelessHelper;

    Q_DISABLE_COPY(FramelessMainWindow)
};

} // namespace ns

#endif // FRAMELESSMAINWINDOW_H
