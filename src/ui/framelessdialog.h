#ifndef FRAMELESSDIALOG_H
#define FRAMELESSDIALOG_H

#include <QScopedPointer>
#include <QDialog>
#include <QPoint>
#include "uiglobal.h"

namespace ns {

class FramelessHelper;

class BUILDSYS_UI_DECL FramelessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FramelessDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FramelessDialog();

protected:
	virtual void resizeEvent(QResizeEvent* event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent (QMouseEvent * event );
	virtual void mouseMoveEvent (QMouseEvent * event );

private:
	bool leftButtonPressed;
	QPoint dragPos;

    Q_DISABLE_COPY(FramelessDialog)
};

} // namespace ns

#endif // FRAMELESSDIALOG_H
