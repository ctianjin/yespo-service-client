#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QProgressDialog>
#include "taskexecutor.h"
#include "abstracttask.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "TaskExecutor:"
#endif // DEBUG_PREFIX

namespace ns {

TaskExecutor::TaskExecutor(QWidget* parent) :
    QObject(parent)
{
}

TaskExecutor::~TaskExecutor()
{
}

AbstractTask* TaskExecutor::task() const
{
    return m_task;
}

void TaskExecutor::setTask(AbstractTask* task)
{
    m_task = task;
}

ExecutionResult TaskExecutor::exec()
{
    if (!m_task) {
        qWarning() << DEBUG_PREFIX << "Can not execute because task is null";
        return ExecutionResult();
    }

    QProgressDialog dlg(parentWidget());
    dlg.setWindowTitle(m_task->name());
    dlg.setAutoReset(false);
    if (!m_task->isCancelable()) {
        dlg.setCancelButton(0);
    }

    connect(m_task, SIGNAL(finished()), &dlg, SLOT(reset()));
    connect(&dlg, SIGNAL(canceled()), m_task, SLOT(cancel()));
    connect(m_task, SIGNAL(progressRangeChanged(int, int)), &dlg, SLOT(setRange(int, int)));
    connect(m_task, SIGNAL(progressValueChanged(int)), &dlg, SLOT(setValue(int)));
    connect(m_task, SIGNAL(progressTextChanged(QString)), &dlg, SLOT(setLabelText(QString)));

    m_task->exec();
    dlg.exec();
    m_task->waitForFinished();

    ExecutionResult result = m_task->result();
    if (result.isCanceled()) {
        // Noop
    } else if (result.isError()) {
        reportError();
    } else {
        reportSuccess();
    }

    return result;
}

void TaskExecutor::reportSuccess()
{
    if (!m_task)
        return;

    if (m_task->isReportEnabled()) {
        QString text = m_task->result().text();
        if (!text.isEmpty()) {
			qApp->information(text, parentWidget());
//            QMessageBox::information(parentWidget(), m_task->name(), text);
        }
    }
}

void TaskExecutor::reportError()
{
    if (!m_task)
        return;

    if (m_task->isReportEnabled()) {
        QString text = m_task->result().text();
        if (text.isEmpty()) {
            text = tr("Failed to execute task");
        }
		qApp->critical(text, parentWidget());
//        QMessageBox::critical(parentWidget(), m_task->name(), text);
    }
}

QWidget* TaskExecutor::parentWidget() const
{
    QWidget* w = qobject_cast<QWidget*>(parent());
    if (!w) {
        w = qApp->activeWindow();
    }
    return w;
}

} // namespace ns
