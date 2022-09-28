#include <QFontDialog>
#include "fontedit.h"

namespace Qpid {

FontEdit::FontEdit(QWidget* parent) :
    AbstractActionEdit(parent)
{
}

FontEdit::~FontEdit()
{
}

QFont FontEdit::fontValue() const
{
    return m_fontValue;
}

void FontEdit::setFontValue(const QFont& fontValue)
{
    if (m_fontValue == fontValue)
        return;

    m_fontValue = fontValue;
    setText(tr("%1, %2pt").arg(m_fontValue.family(),
        QString::number(m_fontValue.pointSize())));
}

void FontEdit::action()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_fontValue);
    if (ok && m_fontValue != font) {
        setFontValue(font);
        Q_EMIT fontValueChanged(m_fontValue);
    }
}

} // namespace Qpid
