#ifndef QPID_CONTACTSTREEWIDGET_H
#define QPID_CONTACTSTREEWIDGET_H

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Qpid {

class ContactsTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
	explicit ContactsTreeWidget(QWidget *parent);
	virtual ~ContactsTreeWidget();

public Q_SLOTS:

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event);
	virtual void mousePressEvent(QMouseEvent *event);

Q_SIGNALS:
	void viewProfile(QTreeWidgetItem* item);
	void remove(QTreeWidgetItem* item);

private Q_SLOTS:
	void slotProfileTriggered();
	void slotRemoveTriggered();

private:
	QAction* m_actionProfile;
	QAction* m_actionRemove;
	QMenu* m_menu;

	QTreeWidgetItem* m_itemTriggered;

};

}

#endif // QPID_CONTACTSTREEWIDGET_H
