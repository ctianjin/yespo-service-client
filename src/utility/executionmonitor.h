#ifndef EXECUTIONMONITOR_H
#define EXECUTIONMONITOR_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include "executionresult.h"
#include "utilityglobal.h"

QT_BEGIN_NAMESPACE
class QFutureInterfaceBase;
QT_END_NAMESPACE

namespace ns {

class ExecutionMonitorPrivate;
class NullExecutionMonitorPrivate;
class FutureExecutionMonitorPrivate;
class SubExecutionMonitorPrivate;
class ExecutionMonitorStackPrivate;

class BUILDSYS_UTILITY_DECL ExecutionMonitor
{
public:
    ExecutionMonitor();
    virtual ~ExecutionMonitor();

    ExecutionResult result() const;
    void setResult(const ExecutionResult& result);

    bool isError() const;
    void setError(const QString& text = QString());
    void setError(int code, const QString& text = QString());

    virtual bool isCanceled() const = 0;

    virtual QString progressText() const = 0;
    virtual void setProgressText(const QString& text) = 0;

    virtual int progressMinimum() const = 0;
    virtual int progressMaximum() const = 0;
    virtual void setProgressRange(int minimum, int maximum) = 0;

    virtual int progressValue() const = 0;
    virtual void setProgressValue(int value) = 0;
    virtual void progress(int value = 1);

protected:
    ExecutionMonitor(ExecutionMonitorPrivate& dd);

    ExecutionMonitorPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(ExecutionMonitor)
    Q_DISABLE_COPY(ExecutionMonitor)
};

class BUILDSYS_UTILITY_DECL NullExecutionMonitor : public ExecutionMonitor
{
public:
    NullExecutionMonitor();
    virtual ~NullExecutionMonitor();

    virtual bool isCanceled() const;

    virtual QString progressText() const;
    virtual void setProgressText(const QString& text);

    virtual int progressMinimum() const;
    virtual int progressMaximum() const;
    virtual void setProgressRange(int minimum, int maximum);

    virtual int progressValue() const;
    virtual void setProgressValue(int value);

protected:
    NullExecutionMonitor(NullExecutionMonitorPrivate& dd);

private:
    Q_DECLARE_PRIVATE(NullExecutionMonitor)
    Q_DISABLE_COPY(NullExecutionMonitor)
};

class BUILDSYS_UTILITY_DECL FutureExecutionMonitor : public ExecutionMonitor
{
public:
    explicit FutureExecutionMonitor(QFutureInterfaceBase* future);
    virtual ~FutureExecutionMonitor();

    virtual bool isCanceled() const;

    virtual QString progressText() const;
    virtual void setProgressText(const QString& text);

    virtual int progressMinimum() const;
    virtual int progressMaximum() const;
    virtual void setProgressRange(int minimum, int maximum);

    virtual int progressValue() const;
    virtual void setProgressValue(int value);

protected:
    FutureExecutionMonitor(FutureExecutionMonitorPrivate& dd);

private:
    Q_DECLARE_PRIVATE(FutureExecutionMonitor)
    Q_DISABLE_COPY(FutureExecutionMonitor)
};

class BUILDSYS_UTILITY_DECL SubExecutionMonitor : public ExecutionMonitor
{
public:
    SubExecutionMonitor(ExecutionMonitor* parent, int minimum, int maximum);
    virtual ~SubExecutionMonitor();

    virtual bool isCanceled() const;

    virtual QString progressText() const;
    virtual void setProgressText(const QString& text);

    virtual int progressMinimum() const;
    virtual int progressMaximum() const;
    virtual void setProgressRange(int minimum, int maximum);

    virtual int progressValue() const;
    virtual void setProgressValue(int value);

protected:
    SubExecutionMonitor(SubExecutionMonitorPrivate& dd);

private:
    Q_DECLARE_PRIVATE(SubExecutionMonitor)
    Q_DISABLE_COPY(SubExecutionMonitor)
};

class BUILDSYS_UTILITY_DECL ExecutionMonitorStack
{
public:
    ExecutionMonitorStack();
    ExecutionMonitorStack(const ExecutionMonitorStack& rhs);
    ~ExecutionMonitorStack();

    ExecutionMonitorStack& operator=(const ExecutionMonitorStack& rhs);

    ExecutionMonitor* current() const;

    void push(ExecutionMonitor* monitor);
    void pop();

private:
    QSharedDataPointer<ExecutionMonitorStackPrivate> d;
};

class BUILDSYS_UTILITY_DECL ExecutionMonitorStackGuard
{
public:
    ExecutionMonitorStackGuard(ExecutionMonitorStack* stack, ExecutionMonitor* monitor);
    ~ExecutionMonitorStackGuard();

private:
    ExecutionMonitorStack* m_stack;

    Q_DISABLE_COPY(ExecutionMonitorStackGuard)
};

} // namespace ns

#endif // EXECUTIONMONITOR_H
