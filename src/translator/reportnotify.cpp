#include "reportnotify.h"
#include "ui_reportnotify.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QMovie>

#ifndef DEBUG_PREFIX
#	define DEBUG_PREFIX "ReportNotify:"
#endif // DEBUG_PREFIX

namespace ns {

ReportNotify::ReportNotify(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_ReportNotify)
{
    m_ui->setupUi(this);
}

ReportNotify::~ReportNotify()
{
	 
}

void ReportNotify::setNoReportNum(const QString& num)
{
	m_num = num;
	m_ui->labelReportNum->setText(tr("You have  <font style = \"color: #FF6600\"; \"text-align: center\">%1</font> report(s) to finish").arg(m_num));
}

void ReportNotify::on_pushButtonCheck_clicked()
{
	Q_EMIT requestReportList();
}

void ReportNotify::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	switch (event->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		retranslateTr();
		break;
	default:
		break;
	}
}

void ReportNotify::retranslateTr()
{
	m_ui->labelReportNum->setText(tr("You have  <font style = \"color: #FF6600\" >%1</font> report(s) to finish").arg(m_num));
}

} // namespace ns
