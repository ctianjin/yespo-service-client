#ifndef CUSTOMMOVIEWIDGET_H
#define CUSTOMMOVIEWIDGET_H

#include <QtGui/QWidget>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL CustomMovieWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomMovieWidget(QWidget* parent = 0);
    virtual ~CustomMovieWidget();

	void nextAnimationFrame(int diameter, int width);

protected:
	void paintEvent(QPaintEvent *e);



private:
	int m_frameNo;
	int m_diameter;
	int m_widthToPaint;

    Q_DISABLE_COPY(CustomMovieWidget)
};

} // namespace ns

#endif // CUSTOMMOVIEWIDGET_H
