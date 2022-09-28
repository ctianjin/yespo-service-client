#include <QtCore/QDebug>
#include <QtCore/QString>
#include "oracledatabase.h"
#include "oracledatabase_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "OracleDatabase:"
#endif // DEBUG_PREFIX

namespace ns {

OracleDatabasePrivate::OracleDatabasePrivate(const QString& connectionName) :
    DatabasePrivate(Database::OracleDriver, connectionName)
{
}

OracleDatabasePrivate::OracleDatabasePrivate(Database::DriverType driverType, const QString& connectionName) :
    DatabasePrivate(driverType, connectionName)
{
}

OracleDatabasePrivate::~OracleDatabasePrivate()
{
}

OracleDatabase::OracleDatabase(const QString& connectionName, QObject* parent) :
    Database(*new OracleDatabasePrivate(connectionName), parent)
{
}

OracleDatabase::OracleDatabase(DriverType driverType, const QString& connectionName, QObject* parent) :
    Database(*new OracleDatabasePrivate(driverType, connectionName), parent)
{
}

OracleDatabase::OracleDatabase(OracleDatabasePrivate& dd, QObject* parent) :
    Database(dd, parent)
{
}

OracleDatabase::~OracleDatabase()
{
}

QString OracleDatabase::sqlAddLimits(const QString& sql, int limit, int offset) const
{
    if (limit < 0 && offset < 0)
        return sql;

    QString ret;

    if (limit < 0) {
        Q_ASSERT(offset >= 0);
        ret = QLatin1String("SELECT * FROM (SELECT row_.*, ROWNUM rownum_ FROM (");
        ret += sql;
        ret += QLatin1String(") row_");
        ret += QLatin1String(") WHERE rownum_ > ");
        ret += QString::number(offset);

    } else {
        if (offset >= 0) {
            ret = QLatin1String("SELECT * FROM (SELECT row_.*, ROWNUM rownum_ FROM (");
        } else {
            ret = QLatin1String("SELECT * FROM (");
        }

        ret += sql;

        if (offset >= 0) {
            ret += QLatin1String(") row_ WHERE ROWNUM <= ");
            ret += QString::number(offset + limit);
            ret += QLatin1String(") WHERE rownum_ > ");
            ret += QString::number(offset);
        } else {
            ret += QLatin1String(") WHERE ROWNUM <= ");
            ret += QString::number(limit);
        }
    }

    return ret;
}

} // namespace ns
