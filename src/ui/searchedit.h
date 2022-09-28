#ifndef QPID_SEARCHEDIT_H
#define QPID_SEARCHEDIT_H

#include "smartedit.h"
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QToolButton;
class QSize;
QT_END_NAMESPACE

namespace Qpid {

class QPID_UI_DECL SearchEdit : public SmartEdit
{
    Q_OBJECT
    
public:
    explicit SearchEdit(QWidget* parent = 0);
    virtual ~SearchEdit(); 

Q_SIGNALS:
    

protected:

private Q_SLOTS:
	void slotSearchClicked();

private:
	QToolButton *m_searchButton;
	QSize *m_iconSize;

    Q_DISABLE_COPY(SearchEdit)
};

} // namespace Qpid

#endif // QPID_SEARCHEDIT_H
