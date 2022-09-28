#ifndef ORACLEDATABASE_H
#define ORACLEDATABASE_H

#include "database.h"
#include "dbglobal.h"

namespace ns {

class OracleDatabasePrivate;

class BUILDSYS_DB_DECL OracleDatabase : public Database
{
    Q_OBJECT

public:
    explicit OracleDatabase(const QString& connectionName, QObject* parent = 0);
    OracleDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~OracleDatabase();

protected:
    OracleDatabase(OracleDatabasePrivate& dd, QObject* parent = 0);

    virtual QString sqlAddLimits(const QString& sql, int limit, int offset = -1) const;

private:
    Q_DECLARE_PRIVATE(OracleDatabase)
    Q_DISABLE_COPY(OracleDatabase)
};

} // namespace ns

#endif // ORACLEDATABASE_H
