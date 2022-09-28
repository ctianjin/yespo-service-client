#ifndef QPID_SMARTEDIT_H
#define QPID_SMARTEDIT_H

#include <QLineEdit>
#include "uiglobal.h"

namespace Qpid {

class QPID_UI_DECL SmartEdit : public QLineEdit
{
    Q_OBJECT
	Q_PROPERTY(QString defaultText READ defaultText WRITE setDefaultText)
    
public:
    explicit SmartEdit(QWidget* parent = 0);
    virtual ~SmartEdit(); 

	QString defaultText() const;
	void setDefaultText(const QString& text);
	void showAsPassword();

	QString text() const;
	void inputText(const QString& text);

Q_SIGNALS:
    

protected:
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);

private Q_SLOTS:
	void slotTextEdited(const QString& text);

private:
	QString m_defaultText;
	bool m_userInput;
	bool m_showAsPassword;

    Q_DISABLE_COPY(SmartEdit)
};

} // namespace Qpid

#endif // QPID_SMARTEDIT_H
