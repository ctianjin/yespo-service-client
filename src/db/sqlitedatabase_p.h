#ifndef SQLITEDATABASE_P_H
#define SQLITEDATABASE_P_H

#include <QtCore/QString>
#include "sqlitedatabase.h"
#include "database_p.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL SqliteDatabasePrivate : public ManagedDatabasePrivate
{
    Q_DECLARE_PUBLIC(SqliteDatabase)

public:
    int m_busyTimeout;
    bool m_openReadonly;
    bool m_sharedCacheEnabled;
    bool m_extensionFunctionsEnabled;

    explicit SqliteDatabasePrivate(const QString& connectionName);
    SqliteDatabasePrivate(Database::DriverType driverType, const QString& connectionName);
    virtual ~SqliteDatabasePrivate();

private:
    void init();

    Q_DISABLE_COPY(SqliteDatabasePrivate)
};

} // namespace ns

#endif // SQLITEDATABASE_P_H
