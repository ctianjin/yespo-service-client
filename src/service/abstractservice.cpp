#include <QtCore/QDebug>
#include <QtNetwork/QNetworkReply>
#include "abstractservice.h"
#include "abstractservice_p.h"
#include "service.h"
#include "service_p.h"
#include "servicemanager.h"
#include "servicemanager_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractService:"
#endif // DEBUG_PREFIX

namespace ns {

AbstractServicePrivate::AbstractServicePrivate(ServiceManager* manager, const QString& name) :
    q_ptr(0),
    m_manager(manager),
    m_name(name),
    m_state(Service::StoppedState),
    m_error(Service::NoError)
{
}

AbstractServicePrivate::~AbstractServicePrivate()
{
}

QString AbstractServicePrivate::dataLocation() const
{
    if (!m_manager)
        return QString();
    return m_manager->dataLocation();
}

DatabaseRegistrar* AbstractServicePrivate::databaseRegistrar() const
{
    if (!m_manager)
        return 0;
    return m_manager->databaseRegistrar();
}

void AbstractServicePrivate::setState(Service::State state)
{
    Q_Q(AbstractService);

    if (m_state != state) {
#ifndef QT_NO_DEBUG_OUTPUT
        qDebug() << DEBUG_PREFIX << m_name << "State changed to" << Service::stateText(state);
#endif // QT_NO_DEBUG_OUTPUT

        Service::State oldState = m_state;
        m_state = state;
        Q_EMIT q->stateChanged(m_state);

        if (m_state > oldState) {
            if (m_state >= Service::ActiveState && oldState < Service::ActiveState) {
                Q_EMIT q->active();
            }
        } else {
            if (m_state < Service::ActiveState && oldState >= Service::ActiveState) {
                Q_EMIT q->inactive();
            }
            if (m_state <= Service::StoppedState && oldState > Service::StoppedState) {
                Q_EMIT q->stopped();
            }
        }
    }
}

void AbstractServicePrivate::setError(Service::Error e)
{
    Q_Q(AbstractService);

    m_error = e;
    m_errorReason.clear();
    if (m_error) {
        Q_EMIT q->error(m_error);
    }
}

void AbstractServicePrivate::setHttpError(Service::Error e, const QString& token, const QString& reason)
{
	Q_Q(AbstractService);

	m_error = e;
	if (m_error) {
		Q_EMIT q->httpError(m_error, token, reason);
	}
}

void AbstractServicePrivate::setError(Service::Error e, const QString& reason)
{
    Q_Q(AbstractService);

    m_error = e;
    m_errorReason = reason;
    if (m_error) {
        Q_EMIT q->error(m_error);
    }
}

void AbstractServicePrivate::setError(AbstractService* service)
{
    Q_ASSERT(service);

    AbstractServicePrivate* serviced = service->d_func();
    setError(serviced->m_error, serviced->m_errorReason);
}

void AbstractServicePrivate::setError(QNetworkReply* reply)
{
    Q_ASSERT(reply);

    QNetworkReply::NetworkError e = reply->error();
    if (e == QNetworkReply::NoError) {
        setError(Service::NoError);
    } else {
        setError(Service::translateNetworkError(e), reply->errorString());
    }
}

void AbstractServicePrivate::stopForNoError()
{
    setState(Service::StoppedState);
    setError(Service::NoError);
    cleanup();
}

void AbstractServicePrivate::stopForError(AbstractService* service)
{
    Q_ASSERT(service);

    setState(Service::StoppedState);

    AbstractServicePrivate* serviced = service->d_func();
    setError(serviced->m_error, serviced->m_errorReason);

    cleanup();
}

void AbstractServicePrivate::startup()
{
    // Noop
}

void AbstractServicePrivate::cleanup()
{
    // Noop
}

AbstractService::AbstractService(ServiceManager* manager, QObject* parent) :
    QObject(parent),
    d_ptr(new AbstractServicePrivate(manager, QString()))
{
    d_ptr->q_ptr = this;
}

AbstractService::AbstractService(ServiceManager* manager, const QString& name, QObject* parent) :
    QObject(parent),
    d_ptr(new AbstractServicePrivate(manager, name))
{
    d_ptr->q_ptr = this;
}

AbstractService::AbstractService(AbstractServicePrivate& dd, QObject* parent) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractService::~AbstractService()
{
    delete d_ptr;
}

ServiceManager* AbstractService::manager() const
{
    Q_D(const AbstractService);
    return d->m_manager;
}

QString AbstractService::name() const
{
    Q_D(const AbstractService);
    return d->m_name;
}

Service::State AbstractService::state() const
{
    Q_D(const AbstractService);
    return d->m_state;
}

QString AbstractService::stateText() const
{
    Q_D(const AbstractService);
    return Service::stateText(d->m_state);
}

Service::Error AbstractService::error() const
{
    Q_D(const AbstractService);
    return d->m_error;
}

QString AbstractService::errorText() const
{
    Q_D(const AbstractService);
    return Service::errorText(d->m_error, d->m_errorReason);
}

bool AbstractService::isStopped() const
{
    Q_D(const AbstractService);
    return d->isStopped();
}

bool AbstractService::isActive() const
{
    Q_D(const AbstractService);
    return d->isActive();
}

void AbstractService::start()
{
    Q_D(AbstractService);

    if (!isStopped()) {
        qDebug() << DEBUG_PREFIX << d->m_name << "start rejected because not stopped";
        return;
    }

    d->setError(Service::NoError);
    d->setState(Service::InitState);

    d->startup();
    if (d->m_error) {
        d->setState(Service::StoppedState);
    } else {
        d->setState(Service::ActiveState);
    }
}

void AbstractService::stop()
{
    Q_D(AbstractService);

    if (isStopped()) {
        qDebug() << DEBUG_PREFIX << d->m_name << "stop rejected because already stopped";
        return;
    }

    d->stopForNoError();
}

} // namespace ns
