#ifndef FRAMELESSVIDEODIALOG_H
#define FRAMELESSVIDEODIALOG_H

#include <QScopedPointer>
#include <QDialog>
#include <QPoint>
#include <QEvent>
#include "uiglobal.h"

namespace ns {

class FramelessHelper;

class BUILDSYS_UI_DECL FramelessVideoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FramelessVideoDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~FramelessVideoDialog();

protected:
	virtual void paintEvent(QPaintEvent *event);
// 	virtual bool eventFilter(QObject *obj, QEvent *event);
// 	virtual void resizeEvent(QResizeEvent* event);
private:
	void updateSkin();
private:
	QPixmap m_pixMask;
    Q_DISABLE_COPY(FramelessVideoDialog)
};

} // namespace ns

#endif // FRAMELESSVIDEODIALOG_H
