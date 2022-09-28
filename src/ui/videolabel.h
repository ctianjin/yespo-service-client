#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL VideoLabel : public QLabel
{
    Q_OBJECT

public:
	explicit VideoLabel(QWidget *parent);
	virtual ~VideoLabel();

	void setFullScreen(bool fullScreen);
public Q_SLOTS:

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
Q_SIGNALS:
    void fullScreen(bool full);

private:
	bool m_fullScreen;
};

} // namespace ns

#endif // VIDEOLABEL_H
