#ifndef ABSTRACTDATASERVICE_H
#define ABSTRACTDATASERVICE_H

#include "abstractservice.h"
#include "serviceglobal.h"

namespace ns {

class AbstractDataServicePrivate;
class Database;

class BUILDSYS_SERVICE_DECL AbstractDataService : public AbstractService
{
    Q_OBJECT

public:
    explicit AbstractDataService(ServiceManager* manager, QObject* parent = 0);
    AbstractDataService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~AbstractDataService();

    Database* database() const;

    virtual void start();
    virtual void stop();

protected:
    AbstractDataService(AbstractDataServicePrivate& dd, QObject* parent = 0);

private:
    Q_DECLARE_PRIVATE(AbstractDataService)
    Q_DISABLE_COPY(AbstractDataService)
};

} // namespace ns

#endif // ABSTRACTDATASERVICE_H
