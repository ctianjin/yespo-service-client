#ifndef QPID_TASKRUNNABLE_P_H
#define QPID_TASKRUNNABLE_P_H

#include <QtCore/QFutureInterface>
#include <QtCore/QRunnable>
#include "executionresult.h"

namespace ns {

class AbstractTask;

class TaskRunnable : public QRunnable
{
public:
    explicit TaskRunnable(AbstractTask* task, int priority = 0);
    virtual ~TaskRunnable();

    QFuture<ExecutionResult> start();

    virtual void run();

private:
    AbstractTask* m_task;
    int m_priority;
    QFutureInterface<ExecutionResult> m_future;
};

} // namespace ns

#endif // QPID_TASKRUNNABLE_P_H
