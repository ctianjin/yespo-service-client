#include <QFocusEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include "abstractactionedit.h"

namespace Qpid {

AbstractActionEdit::AbstractActionEdit(QWidget* parent) :
    QWidget(parent),
    m_lineEdit(0),
    m_actionButton(0)
{
    m_lineEdit = new QLineEdit(this);

    m_actionButton = new QToolButton(this);
    m_actionButton->setFocusPolicy(Qt::NoFocus);
    m_actionButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_actionButton->setText(tr("..."));
	m_actionButton->setStyleSheet(QLatin1String("border-image: url(:/images/setting_Ui/111.png)"));
    m_actionButton->setAutoRaise(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_actionButton);

    m_lineEdit->setReadOnly(true);
    m_lineEdit->setFocusProxy(this);
    setFocusPolicy(m_lineEdit->focusPolicy());
    setAttribute(Qt::WA_InputMethodEnabled);

    connect(m_actionButton, SIGNAL(clicked()), this, SLOT(action()));
}

AbstractActionEdit::~AbstractActionEdit()
{
}

QString AbstractActionEdit::text() const
{
    return m_lineEdit->text();
}

void AbstractActionEdit::setText(const QString& text)
{
    m_lineEdit->setText(text);
}

void AbstractActionEdit::focusInEvent(QFocusEvent* e)
{
    m_lineEdit->event(e);
    m_lineEdit->selectAll();
    QWidget::focusInEvent(e);
}

void AbstractActionEdit::focusOutEvent(QFocusEvent* e)
{
    m_lineEdit->event(e);
    QWidget::focusOutEvent(e);
}

} // namespace Qpid
