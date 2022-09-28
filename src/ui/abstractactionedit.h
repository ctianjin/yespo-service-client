#ifndef QPID_ABSTRACTACTIONEDIT_H
#define QPID_ABSTRACTACTIONEDIT_H

#include <QWidget>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QToolButton;
QT_END_NAMESPACE

namespace Qpid {

class QPID_UI_DECL AbstractActionEdit : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractActionEdit(QWidget* parent = 0);
    virtual ~AbstractActionEdit();

protected:
    QString text() const;
    void setText(const QString& text);

    virtual void focusInEvent(QFocusEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);

protected Q_SLOTS:
    virtual void action() = 0;

private:
    QLineEdit* m_lineEdit;
    QToolButton* m_actionButton;

    Q_DISABLE_COPY(AbstractActionEdit)
};

} // namespace Qpid

#endif // QPID_ABSTRACTACTIONEDIT_H
