#ifndef QPID_HISTORYTREEWIDGET_H
#define QPID_HISTORYTREEWIDGET_H

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Qpid {

class HistoryTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
	explicit HistoryTreeWidget(QWidget *parent);
	virtual ~HistoryTreeWidget();

public Q_SLOTS:

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event);
	virtual void mousePressEvent(QMouseEvent *event);

Q_SIGNALS:
	void remove(QTreeWidgetItem* item);

private Q_SLOTS:
	void slotRemoveTriggered();

private:
	QAction* m_actionRemove;
	QMenu* m_menu;

	QTreeWidgetItem* m_itemTriggered;

};

}

#endif // QPID_HISTORYTREEWIDGET_H
