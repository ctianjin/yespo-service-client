#ifndef QPID_COLOREDIT_H
#define QPID_COLOREDIT_H

#include <QtGui/QColor>
#include "abstractactionedit.h"
#include "uiglobal.h"

namespace Qpid {

class QPID_UI_DECL ColorEdit : public AbstractActionEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor colorValue READ colorValue WRITE setColorValue)

public:
    explicit ColorEdit(QWidget* parent = 0);
    virtual ~ColorEdit();

    QColor colorValue() const;

public Q_SLOTS:
    void setColorValue(const QColor& colorValue);

Q_SIGNALS:
    void colorValueChanged(const QColor& colorValue);

protected:
    virtual void action();

private:
    QColor m_colorValue;

    Q_DISABLE_COPY(ColorEdit)
};

} // namespace Qpid

#endif // QPID_COLOREDIT_H
