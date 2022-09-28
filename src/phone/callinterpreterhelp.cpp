#include "callinterpreterhelp.h"
#include "phoneapplication.h"
#include "ui_callinterpreterhelp.h"


#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CallInterpreterHelp:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct CallInterpreterHelpOptions
{

};

CallInterpreterHelp::CallInterpreterHelp(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_CallInterpreterHelp),
    m_opt(new CallInterpreterHelpOptions)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	m_ui->pushButtonCloseDesp->hide();
}

CallInterpreterHelp::~CallInterpreterHelp()
{

}

void CallInterpreterHelp::setDisplayText(const QString& caption, 
			const QString& step1, const QString& step2, const QString& step3)
{
	m_ui->labelReadyToCall->setText(caption);
	m_ui->labelStep_1_desp->setText(step1);
	m_ui->labelStep_3_desp->setText(step2);
	m_ui->labelStep_5_desp->setText(step3);
}

void CallInterpreterHelp::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

} // namespace Qpid
