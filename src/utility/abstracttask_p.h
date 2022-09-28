#ifndef ABSTRACTTASK_P_H
#define ABSTRACTTASK_P_H

#include <QtCore/QFutureWatcher>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include "abstracttask.h"
#include "executionresult.h"
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL AbstractTaskPrivate
{
    Q_DECLARE_PUBLIC(AbstractTask)

public:
    AbstractTask* q_ptr;

    mutable QMutex m_mutex; // XXX Not necessary if not writing members within run() function

    QString m_name;
    QString m_description;
    int m_priority;
    bool m_cancelable;
    bool m_reportEnabled;

    ExecutionResult m_result;
    QFutureWatcher<ExecutionResult>* m_watcher;

    AbstractTaskPrivate();
    virtual ~AbstractTaskPrivate();

    virtual void onSuccess();
    virtual void onError();
    virtual void onCanceled();

    void _q_started();
    void _q_finished();
    void _q_canceled();

private:
    void init();
};

} // namespace ns

#endif // ABSTRACTTASK_P_H
