#ifndef QPID_CALLINTERPRETERHELP_H
#define QPID_CALLINTERPRETERHELP_H

#include <QWidget>
#include <QScopedPointer>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_CallInterpreterHelp;
struct CallInterpreterHelpOptions;

class CallInterpreterHelp : public QWidget, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit CallInterpreterHelp(QWidget* parent = 0);
    virtual ~CallInterpreterHelp();

	void setDisplayText(const QString& caption, 
		const QString& step1, const QString& step2, const QString& step3);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:

private:
    QScopedPointer<Ui_CallInterpreterHelp> m_ui;
    QScopedPointer<CallInterpreterHelpOptions> m_opt;

    Q_DISABLE_COPY(CallInterpreterHelp)
};

} // namespace Qpid

#endif // QPID_CALLINTERPRETERHELP_H
