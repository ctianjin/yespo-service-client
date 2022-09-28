#include <QtCore/QFuture>
#include <QtCore/QMutexLocker>
#include "abstracttask.h"
#include "abstracttask_p.h"
#include "taskrunnable_p.h"
#include "executionmonitor.h"

namespace ns {

AbstractTaskPrivate::AbstractTaskPrivate() :
    q_ptr(0),
    m_priority(0),
    m_cancelable(true),
    m_reportEnabled(true),
    m_watcher(0)
{
}

AbstractTaskPrivate::~AbstractTaskPrivate()
{
}

void AbstractTaskPrivate::init()
{
    Q_Q(AbstractTask);
    Q_ASSERT(q);

    m_watcher = new QFutureWatcher<ExecutionResult>(q);

    QObject::connect(m_watcher, SIGNAL(started()), q, SLOT(_q_started()));
    QObject::connect(m_watcher, SIGNAL(finished()), q, SLOT(_q_finished()));
    QObject::connect(m_watcher, SIGNAL(canceled()), q, SLOT(_q_canceled()));

    QObject::connect(m_watcher, SIGNAL(progressRangeChanged(int, int)),
        q, SIGNAL(progressRangeChanged(int, int)));
    QObject::connect(m_watcher, SIGNAL(progressValueChanged(int)),
        q, SIGNAL(progressValueChanged(int)));
    QObject::connect(m_watcher, SIGNAL(progressTextChanged(QString)),
        q, SIGNAL(progressTextChanged(QString)));
}

void AbstractTaskPrivate::onSuccess()
{
    // Noop
}

void AbstractTaskPrivate::onError()
{
    // Noop
}

void AbstractTaskPrivate::onCanceled()
{
    // Noop
}

void AbstractTaskPrivate::_q_started()
{
    Q_Q(AbstractTask);

    {
        QMutexLocker locker(&m_mutex);
        m_result = ExecutionResult();
    }

    Q_EMIT q->started();
}

void AbstractTaskPrivate::_q_finished()
{
    Q_Q(AbstractTask);

    ExecutionResult result = m_watcher->future().result();
    {
        QMutexLocker locker(&m_mutex);
        m_result = result;
    }

    if (result.isCanceled()) {
        onCanceled();
    } if (result.isError()) {
        onError();
    } else {
        onSuccess();
    }

    Q_EMIT q->finished();
}

void AbstractTaskPrivate::_q_canceled()
{
    Q_Q(AbstractTask);

    {
        QMutexLocker locker(&m_mutex);
        m_result = ExecutionCanceled();
    }

    onCanceled();

    Q_EMIT q->canceled();
}

AbstractTask::AbstractTask(QObject* parent) :
    QObject(parent),
    d_ptr(new AbstractTaskPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->init();
}

AbstractTask::AbstractTask(AbstractTaskPrivate& dd, QObject* parent) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
    d_ptr->init();
}

AbstractTask::~AbstractTask()
{
    delete d_ptr;
}

QString AbstractTask::name() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_name;
}

void AbstractTask::setName(const QString& name)
{
    Q_D(AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    d->m_name = name;
}

QString AbstractTask::description() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_description;
}

void AbstractTask::setDescription(const QString& description)
{
    Q_D(AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    d->m_description = description;
}

int AbstractTask::priority() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_priority;
}

void AbstractTask::setPriority(int priority)
{
    Q_D(AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    d->m_priority = priority;
}

bool AbstractTask::isCancelable() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_cancelable;
}

void AbstractTask::setCancelable(bool cancelable)
{
    Q_D(AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    d->m_cancelable = cancelable;
}

bool AbstractTask::isReportEnabled() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_reportEnabled;
}

void AbstractTask::setReportEnabled(bool reportEnabled)
{
    Q_D(AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    d->m_reportEnabled = reportEnabled;
}

int AbstractTask::progressValue() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->progressValue();
}

int AbstractTask::progressMinimum() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->progressMinimum();
}

int AbstractTask::progressMaximum() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->progressMaximum();
}

QString AbstractTask::progressText() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->progressText();
}

bool AbstractTask::isStarted() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->isStarted();
}

bool AbstractTask::isFinished() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->isFinished();
}

bool AbstractTask::isRunning() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->isRunning();
}

bool AbstractTask::isCanceled() const
{
    Q_D(const AbstractTask);
    return d->m_watcher->isCanceled();
}

ExecutionResult AbstractTask::result() const
{
    Q_D(const AbstractTask);

    QMutexLocker locker(&d->m_mutex);
    return d->m_result;
}

QMutex* AbstractTask::mutex() const
{
    Q_D(const AbstractTask);
    return &d->m_mutex;
}

void AbstractTask::exec()
{
    Q_D(AbstractTask);

    QFuture<ExecutionResult> future = (new TaskRunnable(this, priority()))->start();
    d->m_watcher->setFuture(future);
}

void AbstractTask::waitForFinished()
{
    Q_D(AbstractTask);
    d->m_watcher->waitForFinished();
}

void AbstractTask::cancel()
{
    Q_D(AbstractTask);
    d->m_watcher->cancel();
}

} // namespace ns

#include "moc_abstracttask.cpp"
