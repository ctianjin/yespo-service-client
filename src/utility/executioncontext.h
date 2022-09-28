#ifndef QPID_EXECUTIONCONTEXT_H
#define QPID_EXECUTIONCONTEXT_H

#include <QtCore/QCoreApplication>
#include "executionmonitor.h"

namespace ns {

template <typename Context>
class ExecutionUnit
{
public:
    explicit ExecutionUnit(Context* context) :
        m_context(context)
    {
        Q_ASSERT(m_context);
    }

    virtual ~ExecutionUnit()
    {
    }

    Context* context() const
    {
        return m_context;
    }

protected:
    void checkCanceled(ExecutionMonitor* monitor) const
    {
        Q_ASSERT(monitor);

        if (monitor->isCanceled()) {
            throw ExecutionCanceled();
        }
    }

    template <typename Rep>
    void checkRepository(const Rep& rep) const
    {
        if (rep.isError()) {
            throw ExecutionError(rep.errorText());
        }
    }

    Context* m_context;

private:
    Q_DISABLE_COPY(ExecutionUnit);
};

template <typename Context>
class ExecutionFunctor : public ExecutionUnit<Context>
{
public:
    explicit ExecutionFunctor(Context* context) :
        ExecutionUnit<Context>(context)
    {
    }

    virtual ~ExecutionFunctor()
    {
    }

    void exec(ExecutionMonitor* monitor)
    {
        Q_ASSERT(monitor);

        ExecutionMonitorStackGuard guard(&m_monitors, monitor);

        validate();

        Executor executor(this);
        executor();
    }

protected:
    virtual void validate()
    {
        // Noop
    }

    virtual void doInit()
    {
        // Noop
    }

    virtual void doCleanup()
    {
        // Noop
    }

    virtual void doExec() = 0;

    template <typename Func>
    void execSub(Func func, int minimum, int maximum)
    {
        ExecutionMonitor* monitor = m_monitors.current();
        Q_ASSERT(monitor);

        {
            SubExecutionMonitor subMonitor(monitor, minimum, maximum);
            {
                ExecutionMonitorStackGuard guard(&m_monitors, &subMonitor);
                func();
            }
        }
        checkCanceled();
    }

    void execSubFunctor(ExecutionFunctor* func, int minimum, int maximum)
    {
        ExecutionMonitor* monitor = m_monitors.current();
        Q_ASSERT(monitor);
        Q_ASSERT(func);

        {
            SubExecutionMonitor subMonitor(monitor, minimum, maximum);
            {
                ExecutionMonitorStackGuard guard(&m_monitors, &subMonitor);
                func->exec(m_monitors.current());
            }
        }
        checkCanceled();
    }

    using ExecutionUnit<Context>::checkCanceled;

    void checkCanceled() const
    {
        checkCanceled(m_monitors.current());
    }

    ExecutionMonitorStack m_monitors;

private:
    Q_DISABLE_COPY(ExecutionFunctor);

    class Executor
    {
    public:
        explicit Executor(ExecutionFunctor* functor) :
            m_functor(functor)
        {
            Q_ASSERT(m_functor);
            m_functor->doInit();
        }

        ~Executor()
        {
            m_functor->doCleanup();
        }

        void operator()()
        {
            m_functor->doExec();
        }

    private:
        ExecutionFunctor* m_functor;
    };
};

template <typename Context>
class ExecutionEntrance : public ExecutionUnit<Context>
{
    Q_DECLARE_TR_FUNCTIONS(ns::ExecutionEntrance)

public:
    explicit ExecutionEntrance(Context* context) :
        ExecutionUnit<Context>(context),
        m_monitor(0)
    {
    }

    virtual ~ExecutionEntrance()
    {
    }

    bool exec(ExecutionMonitor* monitor)
    {
        NullExecutionMonitor nullMonitor;
        m_monitor = monitor ? monitor : &nullMonitor;

        try {
            doExec();

        } catch (const ExecutionResult& result) {
            monitor->setResult(result);
            return result.isSuccess();

        } catch (...) {
            monitor->setError(tr("Unknown error"));
            return false;
        }

        return true;
    }

protected:
    virtual void doExec() = 0;

    using ExecutionUnit<Context>::checkCanceled;

    void checkCanceled() const
    {
        checkCanceled(m_monitor);
    }

    ExecutionMonitor* m_monitor;

private:
    Q_DISABLE_COPY(ExecutionEntrance);
};

} // namespace ns

#endif // QPID_EXECUTIONCONTEXT_H
