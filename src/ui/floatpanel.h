#ifndef FLOATPANEL_H
#define FLOATPANEL_H

#include <QScopedPointer>
#include <QWidget>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL FloatPanel : public QWidget
{
	Q_OBJECT

public:
    explicit FloatPanel(QWidget* parent = 0);
    virtual ~FloatPanel();

Q_SIGNALS:
	void floatHide();

protected:
	virtual void hideEvent(QHideEvent *event);

private:

    Q_DISABLE_COPY(FloatPanel)
};

} // namespace ns

#endif // FLOATPANEL_H
