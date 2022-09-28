#ifndef MEMORYDATABASE_P_H
#define MEMORYDATABASE_P_H

#include <QtCore/QString>
#include "memorydatabase.h"
#include "sqlitedatabase_p.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL MemoryDatabasePrivate : public SqliteDatabasePrivate
{
    Q_DECLARE_PUBLIC(MemoryDatabase)

public:
    static const QString memoryDatabaseName;

    explicit MemoryDatabasePrivate(const QString& connectionName);
    MemoryDatabasePrivate(Database::DriverType driverType, const QString& connectionName);
    virtual ~MemoryDatabasePrivate();

private:
    Q_DISABLE_COPY(MemoryDatabasePrivate)
};

} // namespace ns

#endif // MEMORYDATABASE_P_H
