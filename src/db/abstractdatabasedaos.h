#ifndef ABSTRACTDATABASEDAOS_H
#define ABSTRACTDATABASEDAOS_H

#include "dbglobal.h"

namespace ns {

class Database;
class EntityData;
class AbstractDatabaseDaosPrivate;
class ExecutionMonitor;

class BUILDSYS_DB_DECL AbstractDatabaseDaos
{
public:
    AbstractDatabaseDaos();
    virtual ~AbstractDatabaseDaos();

    bool isValid() const;

    Database* database() const;
    void setDatabase(Database* db);

    virtual bool load(EntityData* data, ExecutionMonitor* monitor = 0) = 0;
    virtual bool save(const EntityData& data, ExecutionMonitor* monitor = 0) = 0;

protected:
    AbstractDatabaseDaos(AbstractDatabaseDaosPrivate& dd);

    virtual void updateDatabase();

    AbstractDatabaseDaosPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDatabaseDaos)
    Q_DISABLE_COPY(AbstractDatabaseDaos)
};

} // namespace ns

#endif // ABSTRACTDATABASEDAOS_H
