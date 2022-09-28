#ifndef ORACLEDATABASE_P_H
#define ORACLEDATABASE_P_H

#include "oracledatabase.h"
#include "database_p.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL OracleDatabasePrivate : public DatabasePrivate
{
    Q_DECLARE_PUBLIC(OracleDatabase)

public:
    explicit OracleDatabasePrivate(const QString& connectionName);
    OracleDatabasePrivate(Database::DriverType driverType, const QString& connectionName);
    virtual ~OracleDatabasePrivate();

private:
    Q_DISABLE_COPY(OracleDatabasePrivate)
};

} // namespace ns

#endif // ORACLEDATABASE_P_H
