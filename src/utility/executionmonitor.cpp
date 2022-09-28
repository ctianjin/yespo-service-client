#include <QtCore/QFutureInterfaceBase>
#include <QtCore/QStack>
#include "executionmonitor.h"
#include "executionmonitor_p.h"

namespace ns {

ExecutionMonitorPrivate::ExecutionMonitorPrivate() :
    q_ptr(0)
{
}

ExecutionMonitorPrivate::~ExecutionMonitorPrivate()
{
}

ExecutionMonitor::ExecutionMonitor() :
    d_ptr(new ExecutionMonitorPrivate)
{
    d_ptr->q_ptr = this;
}

ExecutionMonitor::ExecutionMonitor(ExecutionMonitorPrivate& dd) :
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

ExecutionMonitor::~ExecutionMonitor()
{
    delete d_ptr;
}

ExecutionResult ExecutionMonitor::result() const
{
    Q_D(const ExecutionMonitor);
    return d->m_result;
}

void ExecutionMonitor::setResult(const ExecutionResult& result)
{
    Q_D(ExecutionMonitor);
    d->m_result = result;
}

bool ExecutionMonitor::isError() const
{
    Q_D(const ExecutionMonitor);
    return d->m_result.isError();
}

void ExecutionMonitor::setError(const QString& text)
{
    Q_D(ExecutionMonitor);
    d->m_result = ExecutionError(text);
}

void ExecutionMonitor::setError(int code, const QString& text)
{
    Q_D(ExecutionMonitor);
    d->m_result = ExecutionError(code, text);
}

void ExecutionMonitor::progress(int value)
{
    setProgressValue(progressValue() + value);
}

NullExecutionMonitorPrivate::NullExecutionMonitorPrivate()
{
}

NullExecutionMonitorPrivate::~NullExecutionMonitorPrivate()
{
}

NullExecutionMonitor::NullExecutionMonitor() :
    ExecutionMonitor(*new NullExecutionMonitorPrivate)
{
}

NullExecutionMonitor::NullExecutionMonitor(NullExecutionMonitorPrivate& dd) :
    ExecutionMonitor(dd)
{
}

NullExecutionMonitor::~NullExecutionMonitor()
{
}

bool NullExecutionMonitor::isCanceled() const
{
    return false;
}

QString NullExecutionMonitor::progressText() const
{
    return QString();
}

void NullExecutionMonitor::setProgressText(const QString& text)
{
    Q_UNUSED(text);
}

int NullExecutionMonitor::progressMinimum() const
{
    return 0;
}

int NullExecutionMonitor::progressMaximum() const
{
    return 0;
}

void NullExecutionMonitor::setProgressRange(int minimum, int maximum)
{
    Q_UNUSED(minimum);
    Q_UNUSED(maximum);
}

int NullExecutionMonitor::progressValue() const
{
    return 0;
}

void NullExecutionMonitor::setProgressValue(int value)
{
    Q_UNUSED(value);
}

FutureExecutionMonitorPrivate::FutureExecutionMonitorPrivate(QFutureInterfaceBase* future) :
    m_future(future)
{
}

FutureExecutionMonitorPrivate::~FutureExecutionMonitorPrivate()
{
    if (m_future) {
        m_future->setProgressValueAndText(m_future->progressMaximum(), m_future->progressText());
    }
}

FutureExecutionMonitor::FutureExecutionMonitor(QFutureInterfaceBase* future) :
    ExecutionMonitor(*new FutureExecutionMonitorPrivate(future))
{
}

FutureExecutionMonitor::FutureExecutionMonitor(FutureExecutionMonitorPrivate& dd) :
    ExecutionMonitor(dd)
{
}

FutureExecutionMonitor::~FutureExecutionMonitor()
{
}

bool FutureExecutionMonitor::isCanceled() const
{
    Q_D(const FutureExecutionMonitor);
    return d->m_future ? d->m_future->isCanceled() : false;
}

QString FutureExecutionMonitor::progressText() const
{
    Q_D(const FutureExecutionMonitor);
    return d->m_future ? d->m_future->progressText() : QString();
}

void FutureExecutionMonitor::setProgressText(const QString& text)
{
    Q_D(FutureExecutionMonitor);

    if (d->m_future) {
        int value = d->m_future->progressValue() + 1; // XXX Must add one, otherwise has no effect
        d->m_future->setProgressValueAndText(value, text);
    }
}

int FutureExecutionMonitor::progressMinimum() const
{
    Q_D(const FutureExecutionMonitor);
    return d->m_future ? d->m_future->progressMinimum() : 0;
}

int FutureExecutionMonitor::progressMaximum() const
{
    Q_D(const FutureExecutionMonitor);
    return d->m_future ? d->m_future->progressMaximum() : 0;
}

void FutureExecutionMonitor::setProgressRange(int minimum, int maximum)
{
    Q_D(FutureExecutionMonitor);

    if (d->m_future) {
        d->m_future->setProgressRange(minimum, maximum);
    }
}

int FutureExecutionMonitor::progressValue() const
{
    Q_D(const FutureExecutionMonitor);
    return d->m_future ? d->m_future->progressValue() : 0;
}

void FutureExecutionMonitor::setProgressValue(int value)
{
    Q_D(FutureExecutionMonitor);

    if (d->m_future) {
        d->m_future->setProgressValueAndText(value, d->m_future->progressText());
    }
}

SubExecutionMonitorPrivate::SubExecutionMonitorPrivate(ExecutionMonitor* parent, int minimum, int maximum) :
    m_parent(parent),
    m_minimum(minimum),
    m_maximum(maximum),
    m_subMinimum(0),
    m_subMaximum(0),
    m_subValue(0)
{
    Q_ASSERT(m_parent);
}

SubExecutionMonitorPrivate::~SubExecutionMonitorPrivate()
{
    m_parent->setProgressValue(m_maximum);
}

SubExecutionMonitor::SubExecutionMonitor(ExecutionMonitor* parent, int minimum, int maximum) :
    ExecutionMonitor(*new SubExecutionMonitorPrivate(parent, minimum, maximum))
{
}

SubExecutionMonitor::SubExecutionMonitor(SubExecutionMonitorPrivate& dd) :
    ExecutionMonitor(dd)
{
}

SubExecutionMonitor::~SubExecutionMonitor()
{
}

bool SubExecutionMonitor::isCanceled() const
{
    Q_D(const SubExecutionMonitor);
    return d->m_parent->isCanceled();
}

QString SubExecutionMonitor::progressText() const
{
    Q_D(const SubExecutionMonitor);
    return d->m_parent->progressText();
}

void SubExecutionMonitor::setProgressText(const QString& text)
{
    Q_D(SubExecutionMonitor);
    d->m_parent->setProgressText(text);
}

int SubExecutionMonitor::progressMinimum() const
{
    Q_D(const SubExecutionMonitor);
    return d->m_subMinimum;
}

int SubExecutionMonitor::progressMaximum() const
{
    Q_D(const SubExecutionMonitor);
    return d->m_subMaximum;
}

void SubExecutionMonitor::setProgressRange(int minimum, int maximum)
{
    Q_D(SubExecutionMonitor);

    d->m_subMinimum = minimum;
    d->m_subMaximum = maximum;
}

int SubExecutionMonitor::progressValue() const
{
    Q_D(const SubExecutionMonitor);
    return d->m_subValue;
}

void SubExecutionMonitor::setProgressValue(int value)
{
    Q_D(SubExecutionMonitor);

    d->m_subValue = qBound(d->m_subMinimum, value, d->m_subMaximum);

    double ratio = ((double)value - d->m_subMinimum) / (d->m_subMaximum - d->m_subMinimum);
    d->m_parent->setProgressValue(d->m_minimum + (d->m_maximum - d->m_minimum) * ratio);
}

class ExecutionMonitorStackPrivate : public QSharedData
{
public:
    QStack<ExecutionMonitor*> m_stack;

    ExecutionMonitorStackPrivate();
    ExecutionMonitorStackPrivate(const ExecutionMonitorStackPrivate& rhs);
    ~ExecutionMonitorStackPrivate();
};

ExecutionMonitorStackPrivate::ExecutionMonitorStackPrivate() :
    QSharedData(),
    m_stack()
{
}

ExecutionMonitorStackPrivate::ExecutionMonitorStackPrivate(const ExecutionMonitorStackPrivate& rhs) :
    QSharedData(rhs),
    m_stack(rhs.m_stack)
{
}

ExecutionMonitorStackPrivate::~ExecutionMonitorStackPrivate()
{
}

ExecutionMonitorStack::ExecutionMonitorStack() :
    d(new ExecutionMonitorStackPrivate)
{
}

ExecutionMonitorStack::ExecutionMonitorStack(const ExecutionMonitorStack& rhs) :
    d(rhs.d)
{
}

ExecutionMonitorStack::~ExecutionMonitorStack()
{
}

ExecutionMonitorStack& ExecutionMonitorStack::operator=(const ExecutionMonitorStack& rhs)
{
    d = rhs.d;
    return *this;
}

ExecutionMonitor* ExecutionMonitorStack::current() const
{
    return d->m_stack.isEmpty() ? 0 : d->m_stack.top();
}

void ExecutionMonitorStack::push(ExecutionMonitor* monitor)
{
    d->m_stack.push(monitor);
}

void ExecutionMonitorStack::pop()
{
    if (!d->m_stack.isEmpty()) {
        d->m_stack.pop();
    }
}

ExecutionMonitorStackGuard::ExecutionMonitorStackGuard(ExecutionMonitorStack* stack, ExecutionMonitor* monitor) :
    m_stack(stack)
{
    Q_ASSERT(m_stack);
    m_stack->push(monitor);
}

ExecutionMonitorStackGuard::~ExecutionMonitorStackGuard()
{
    m_stack->pop();
}

} // namespace ns
