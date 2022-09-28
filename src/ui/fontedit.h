#ifndef QPID_FONTEDIT_H
#define QPID_FONTEDIT_H

#include <QtGui/QFont>
#include "abstractactionedit.h"
#include "uiglobal.h"

namespace Qpid {

class QPID_UI_DECL FontEdit : public AbstractActionEdit
{
    Q_OBJECT
    Q_PROPERTY(QFont fontValue READ fontValue WRITE setFontValue)

public:
    explicit FontEdit(QWidget* parent = 0);
    virtual ~FontEdit();

    QFont fontValue() const;

public Q_SLOTS:
    void setFontValue(const QFont& fontValue);

Q_SIGNALS:
    void fontValueChanged(const QFont& fontValue);

protected:
    virtual void action();

private:
    QFont m_fontValue;

    Q_DISABLE_COPY(FontEdit)
};

} // namespace Qpid

#endif // QPID_FONTEDIT_H
