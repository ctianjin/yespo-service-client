#include <QtCore/QFuture>
#include <QtCore/QThreadPool>
#include "taskrunnable_p.h"
#include "abstracttask.h"
#include "executionmonitor.h"

namespace ns {

TaskRunnable::TaskRunnable(AbstractTask* task, int priority) :
    m_task(task),
    m_priority(priority)
{
    Q_ASSERT(m_task);
}

TaskRunnable::~TaskRunnable()
{
}

QFuture<ExecutionResult> TaskRunnable::start()
{
    m_future.setRunnable(this);
    m_future.reportStarted();
    QFuture<ExecutionResult> future = m_future.future();
    QThreadPool::globalInstance()->start(this, m_priority);
    return future;
}

void TaskRunnable::run()
{
    if (m_future.isCanceled()) {
        m_future.reportFinished();
        return;
    }

    ExecutionResult result;
    {
        FutureExecutionMonitor monitor(&m_future);
        m_task->run(&monitor);
        result = monitor.result();
    }

    m_future.reportResult(result);
    m_future.reportFinished();
}

} // namespace ns
