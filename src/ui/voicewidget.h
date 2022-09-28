#ifndef VOICEWIDGET_H
#define VOICEWIDGET_H

#include <QWidget>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget* parent = 0);
    virtual ~RenderArea();

	void setLevel(qreal value);

protected:
	void paintEvent(QPaintEvent *event);

private:
	qreal m_level;

    Q_DISABLE_COPY(RenderArea)
};

} // namespace ns

#endif // VOICEWIDGET_H
