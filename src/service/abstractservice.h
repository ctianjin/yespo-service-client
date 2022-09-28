#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QtCore/QObject>
#include "preferencesupport.h"
#include "service.h"
#include "serviceglobal.h"

namespace ns {

class ServiceManager;
class AbstractServicePrivate;

class BUILDSYS_SERVICE_DECL AbstractService : public QObject, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit AbstractService(ServiceManager* manager, QObject* parent = 0);
    AbstractService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~AbstractService();

    ServiceManager* manager() const;
    QString name() const;

    Service::State state() const;
    QString stateText() const;

    Service::Error error() const;
    QString errorText() const;

    bool isStopped() const;
    bool isActive() const;

public Q_SLOTS:
    virtual void start();
    virtual void stop();

Q_SIGNALS:
    void stateChanged(ns::Service::State state);

    void stopped();
    void active();
    void inactive();

    void error(ns::Service::Error error);
	void httpError(ns::Service::Error error, const QString& token, const QString& reason);

protected:
    AbstractService(AbstractServicePrivate& dd, QObject* parent = 0);

    AbstractServicePrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractService)
    Q_DISABLE_COPY(AbstractService)
};

} // namespace ns

#endif // ABSTRACTSERVICE_H
