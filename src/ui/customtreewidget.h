#ifndef CUSTOMTREEWIDGET_H
#define CUSTOMTREEWIDGET_H

#include <QtGui/QTreeWidget>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL CustomTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit CustomTreeWidget(QWidget* parent = 0);
    virtual ~CustomTreeWidget();


public Q_SLOTS:

Q_SIGNALS:
	void itemLeaved(QTreeWidgetItem* item, int column);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *event);

private:

    Q_DISABLE_COPY(CustomTreeWidget)
};

} // namespace ns

#endif // CUSTOMTREEWIDGET_H
