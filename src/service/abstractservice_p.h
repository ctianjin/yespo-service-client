#ifndef QPID_ABSTRACTSERVICE_P_H
#define QPID_ABSTRACTSERVICE_P_H

#include <QtCore/QPointer>
#include <QtCore/QString>
#include "abstractservice.h"

QT_BEGIN_NAMESPACE
class QNetworkReply;
QT_END_NAMESPACE

namespace ns {

class DatabaseRegistrar;

class AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(AbstractService)

public:
    AbstractService* q_ptr;

    const QPointer<ServiceManager> m_manager;
    const QString m_name;

    Service::State m_state;
    Service::Error m_error;
    QString m_errorReason;

    AbstractServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~AbstractServicePrivate();

    QString dataLocation() const;
    DatabaseRegistrar* databaseRegistrar() const;

    bool isStopped() const;
    bool isActive() const;

    void setState(Service::State state);
    void setError(Service::Error e);
	void setHttpError(Service::Error e, const QString& token, const QString& reason);
    void setError(Service::Error e, const QString& reason);
    void setError(AbstractService* service);
    void setError(QNetworkReply* reply);

    void stopForNoError();

    template <typename E>
    void stopForError(const E& e);

    template <typename E, typename Reason>
    void stopForError(const E& e, const Reason& reason);

    void stopForError(AbstractService* service);

    virtual void startup();
    virtual void cleanup();
};

inline bool AbstractServicePrivate::isStopped() const
{
    return m_state <= Service::StoppedState;
}

inline bool AbstractServicePrivate::isActive() const
{
    return m_state >= Service::ActiveState;
}

template <typename E>
void AbstractServicePrivate::stopForError(const E& e)
{
    setState(Service::StoppedState);
    setError(e);
    cleanup();
}

template <typename E, typename Reason>
void AbstractServicePrivate::stopForError(const E& e, const Reason& reason)
{
    setState(Service::StoppedState);
    setError(e, reason);
    cleanup();
}

} // namespace ns

#endif // QPID_ABSTRACTSERVICE_P_H
