#include <QFocusEvent>
#include "smartedit.h"

namespace Qpid {

SmartEdit::SmartEdit(QWidget* parent) :
    QLineEdit(parent),
	m_userInput(false),
	m_showAsPassword(false)
{
	connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(slotTextEdited(const QString&)));
}

SmartEdit::~SmartEdit()
{
	
}

QString SmartEdit::defaultText() const
{
	return m_defaultText;
}

void SmartEdit::setDefaultText(const QString& text)
{
	m_defaultText = text;

	if (this->text().isEmpty()) {
		setText(m_defaultText);
		setStyleSheet(QLatin1String("color: #999999;"));
	}
}

void SmartEdit::showAsPassword()
{
	m_showAsPassword = true;
}

QString SmartEdit::text() const
{
	if (m_userInput) 
		return QLineEdit::text();
	else
		return QString(QLatin1String(""));
}

void SmartEdit::inputText(const QString& text)
{
	m_userInput = true;
	setText(QLatin1String(""));
	if (m_showAsPassword) 
		setEchoMode(QLineEdit::Password);
	setStyleSheet(QLatin1String("color: black; font-style: normal"));
	setText(text);
}

void SmartEdit::focusInEvent(QFocusEvent *event)
{
	if (!m_userInput) {
		setText(QLatin1String(""));
		if (m_showAsPassword) 
			setEchoMode(QLineEdit::Password);
		setStyleSheet(QLatin1String("color: black; font-style: normal"));
	}

	QLineEdit::focusInEvent(event);
}

void SmartEdit::focusOutEvent(QFocusEvent *event)
{
	if (text().isEmpty()) {
		if (m_showAsPassword)
			setEchoMode(QLineEdit::Normal);
		m_userInput = false;
		setText(m_defaultText);
		setStyleSheet(QLatin1String("color: #999999"));
	}

	QLineEdit::focusOutEvent(event);
}

void SmartEdit::keyPressEvent(QKeyEvent *event)
{
	QLineEdit::keyPressEvent(event);
}

void SmartEdit::contextMenuEvent(QContextMenuEvent *event)
{
	QLineEdit::contextMenuEvent(event);
}

void SmartEdit::slotTextEdited(const QString& text)
{
	m_userInput = true;
}

} // namespace Qpid
