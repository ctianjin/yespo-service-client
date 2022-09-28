#include <QVariant>
#include <QColorDialog>
#include "coloredit.h"

namespace Qpid {

ColorEdit::ColorEdit(QWidget* parent) :
    AbstractActionEdit(parent)
{
}

ColorEdit::~ColorEdit()
{
}

QColor ColorEdit::colorValue() const
{
    return m_colorValue;
}

void ColorEdit::setColorValue(const QColor& colorValue)
{
    if (m_colorValue == colorValue)
        return;

    m_colorValue = colorValue;
    setText(QVariant(m_colorValue).toString());
}

void ColorEdit::action()
{
    QColor color = QColorDialog::getColor(m_colorValue, this);
    if (m_colorValue != color) {
        setColorValue(color);
        Q_EMIT colorValueChanged(m_colorValue);
    }
}

} // namespace Qpid
