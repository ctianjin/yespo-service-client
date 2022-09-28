#ifndef EXECUTIONMONITOR_P_H
#define EXECUTIONMONITOR_P_H

#include "executionmonitor.h"
#include "executionresult.h"
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL ExecutionMonitorPrivate
{
    Q_DECLARE_PUBLIC(ExecutionMonitor)

public:
    ExecutionMonitor* q_ptr;

    ExecutionResult m_result;

    ExecutionMonitorPrivate();
    virtual ~ExecutionMonitorPrivate();
};

class BUILDSYS_UTILITY_DECL NullExecutionMonitorPrivate : public ExecutionMonitorPrivate
{
    Q_DECLARE_PUBLIC(NullExecutionMonitor)

public:
    NullExecutionMonitorPrivate();
    virtual ~NullExecutionMonitorPrivate();
};

class BUILDSYS_UTILITY_DECL FutureExecutionMonitorPrivate : public ExecutionMonitorPrivate
{
    Q_DECLARE_PUBLIC(FutureExecutionMonitor)

public:
    QFutureInterfaceBase* m_future;

    explicit FutureExecutionMonitorPrivate(QFutureInterfaceBase* future);
    virtual ~FutureExecutionMonitorPrivate();
};

class BUILDSYS_UTILITY_DECL SubExecutionMonitorPrivate : public ExecutionMonitorPrivate
{
    Q_DECLARE_PUBLIC(SubExecutionMonitor)

public:
    ExecutionMonitor* m_parent;
    int m_minimum;
    int m_maximum;
    int m_subMinimum;
    int m_subMaximum;
    int m_subValue;

    SubExecutionMonitorPrivate(ExecutionMonitor* parent, int minimum, int maximum);
    virtual ~SubExecutionMonitorPrivate();
};

} // namespace ns

#endif // EXECUTIONMONITOR_P_H
