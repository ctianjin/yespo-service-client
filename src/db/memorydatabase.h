#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include "sqlitedatabase.h"
#include "dbglobal.h"

namespace ns {

class MemoryDatabasePrivate;

class BUILDSYS_DB_DECL MemoryDatabase : public SqliteDatabase
{
    Q_OBJECT

public:
    explicit MemoryDatabase(const QString& connectionName, QObject* parent = 0);
    MemoryDatabase(DriverType driverType, const QString& connectionName, QObject* parent = 0);
    virtual ~MemoryDatabase();

protected:
    MemoryDatabase(MemoryDatabasePrivate& dd, QObject* parent = 0);

    virtual bool openDatabase();
    virtual ValidateResult validateDatabase();

    virtual QString buildDatabaseName() const;

private:
    Q_DECLARE_PRIVATE(MemoryDatabase)
    Q_DISABLE_COPY(MemoryDatabase)
};

} // namespace ns

#endif // MEMORYDATABASE_H
