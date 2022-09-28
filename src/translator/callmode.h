#ifndef CALLMODE_H
#define CALLMODE_H

#include <QtCore/QScopedPointer>
#include <QWidget>

namespace ns {

class Ui_CallMode;

class CallMode : public QWidget
{
    Q_OBJECT

public:
    CallMode::CallMode(QWidget* parent = 0);
    ~CallMode();

	void setCallingStatusCode(const QString& code);
	void setCallMode(const QString& mode);
	void setLanguage(const QString& source, const QString& target);
	void setServiceMode(const QString& mode);
	void setLanguageStyle(const QString& style);

Q_SIGNALS:

protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:

private:
	void retranslateTr();

	QString m_source;
	QString m_target;
	QString m_mode;
	QString m_callMode;
	QString m_statusCode;

	QScopedPointer<Ui_CallMode> m_ui;
};

} // namespace ns

#endif // CALLMODE_H
