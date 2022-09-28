#include <QtCore/QDebug>
#include "memorydatabase.h"
#include "memorydatabase_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "MemoryDatabase:"
#endif // DEBUG_PREFIX

namespace ns {

const QString MemoryDatabasePrivate::memoryDatabaseName = QLatin1String(":memory:");

MemoryDatabasePrivate::MemoryDatabasePrivate(const QString& connectionName) :
    SqliteDatabasePrivate(connectionName)
{
}

MemoryDatabasePrivate::MemoryDatabasePrivate(Database::DriverType driverType, const QString& connectionName) :
    SqliteDatabasePrivate(driverType, connectionName)
{
}

MemoryDatabasePrivate::~MemoryDatabasePrivate()
{
}

MemoryDatabase::MemoryDatabase(const QString& connectionName, QObject* parent) :
    SqliteDatabase(*new MemoryDatabasePrivate(connectionName), parent)
{
}

MemoryDatabase::MemoryDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    SqliteDatabase(*new MemoryDatabasePrivate(driverType, connectionName), parent)
{
}

MemoryDatabase::MemoryDatabase(MemoryDatabasePrivate& dd, QObject* parent) :
    SqliteDatabase(dd, parent)
{
}

MemoryDatabase::~MemoryDatabase()
{
}

bool MemoryDatabase::openDatabase()
{
    if (!SqliteDatabase::openDatabase()) {
        return false;
    }

    // XXX Create the database at this point
    return createDatabase();
}

Database::ValidateResult MemoryDatabase::validateDatabase()
{
    qDebug() << DEBUG_PREFIX << "Memory database does not need validating";
    return ValidateOk; // XXX or New?
}

QString MemoryDatabase::buildDatabaseName() const
{
    Q_D(const MemoryDatabase);
    return d->memoryDatabaseName;
}

} // namespace ns
