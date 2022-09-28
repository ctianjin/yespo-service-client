#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include <QtCore/QObject>
#include "executionresult.h"
#include "utilityglobal.h"

QT_BEGIN_NAMESPACE
class QMutex;
QT_END_NAMESPACE

namespace ns {

class AbstractTaskPrivate;
class ExecutionMonitor;

class BUILDSYS_UTILITY_DECL AbstractTask : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(int priority READ priority WRITE setPriority)
    Q_PROPERTY(bool cancelable READ isCancelable WRITE setCancelable)
    Q_PROPERTY(bool reportEnabled READ isReportEnabled WRITE setReportEnabled)

public:
    explicit AbstractTask(QObject* parent = 0);
    virtual ~AbstractTask();

    QString name() const;
    void setName(const QString& name);

    QString description() const;
    void setDescription(const QString& description);

    int priority() const;
    void setPriority(int priority);

    bool isCancelable() const;
    void setCancelable(bool cancelable);

    bool isReportEnabled() const;
    void setReportEnabled(bool reportEnabled);

    int progressValue() const;
    int progressMinimum() const;
    int progressMaximum() const;
    QString progressText() const;

    bool isStarted() const;
    bool isFinished() const;
    bool isRunning() const;
    bool isCanceled() const;

    ExecutionResult result() const;

    void exec();
    void waitForFinished();

public Q_SLOTS:
    void cancel();

Q_SIGNALS:
    void started();
    void finished();
    void canceled();
    void progressRangeChanged(int minimum, int maximum);
    void progressValueChanged(int progressValue);
    void progressTextChanged(const QString& progressText);

protected:
    AbstractTask(AbstractTaskPrivate& dd, QObject* parent = 0);

    QMutex* mutex() const;

    virtual void run(ExecutionMonitor* monitor) = 0;

    AbstractTaskPrivate* const d_ptr;

private:
    friend class TaskRunnable;

    Q_PRIVATE_SLOT(d_func(), void _q_started())
    Q_PRIVATE_SLOT(d_func(), void _q_finished())
    Q_PRIVATE_SLOT(d_func(), void _q_canceled())

    Q_DECLARE_PRIVATE(AbstractTask)
    Q_DISABLE_COPY(AbstractTask)
};

} // namespace ns

#endif // ABSTRACTTASK_H
