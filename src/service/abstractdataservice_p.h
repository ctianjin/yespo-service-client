#ifndef QPID_ABSTRACTDATASERVICE_P_H
#define QPID_ABSTRACTDATASERVICE_P_H

#include "abstractdataservice.h"
#include "abstractservice_p.h"

namespace ns {

class AbstractDataServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(AbstractDataService)

public:
    Database* m_db;

    AbstractDataServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~AbstractDataServicePrivate();

    void setDatabase(Database* db);

    bool isDatabaseOpen() const;

    virtual bool openDatabase();
    virtual void closeDatabase();
    virtual bool validateDatabase();

    virtual void startup();
    virtual void cleanup();
};

} // namespace ns

#endif // QPID_ABSTRACTDATASERVICE_P_H
