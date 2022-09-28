#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "abstractentitydao.h"
#include "abstractentitydao_p.h"
#include "database.h"
#include "stringseparator.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractEntityDao:"
#endif // DEBUG_PREFIX

namespace ns {

AbstractEntityDaoPrivate::AbstractEntityDaoPrivate(Database* db, const QString& tableName) :
    q_ptr(0),
    m_db(db),
    m_tableName(tableName),
    m_resultLimit(-1),
    m_resultOffset(-1)
{
}

AbstractEntityDaoPrivate::~AbstractEntityDaoPrivate()
{
}

AbstractEntityDao::AbstractEntityDao(Database* db, const QString& tableName) :
    d_ptr(new AbstractEntityDaoPrivate(db, tableName))
{
    d_ptr->q_ptr = this;
}

AbstractEntityDao::AbstractEntityDao(AbstractEntityDaoPrivate& dd) :
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractEntityDao::~AbstractEntityDao()
{
    delete d_ptr;
}

bool AbstractEntityDao::isValid() const
{
    Q_D(const AbstractEntityDao);
    return d->m_db && !d->m_tableName.isEmpty();
}

Database* AbstractEntityDao::database() const
{
    Q_D(const AbstractEntityDao);
    return d->m_db;
}

void AbstractEntityDao::setDatabase(Database* db)
{
    Q_D(AbstractEntityDao);
    d->m_db = db;
}

bool AbstractEntityDao::isError() const
{
    Q_D(const AbstractEntityDao);
    return d->m_db ? d->m_db->isError() : true; // XXX Invalid means error
}

QString AbstractEntityDao::errorText() const
{
    Q_D(const AbstractEntityDao);
    return d->m_db ? d->m_db->errorText() : QString();
}

QSqlError AbstractEntityDao::lastError() const
{
    Q_D(const AbstractEntityDao);
    return d->m_db ? d->m_db->lastError() : QSqlError();
}

QString AbstractEntityDao::tableName() const
{
    Q_D(const AbstractEntityDao);
    return d->m_tableName;
}

void AbstractEntityDao::setTableName(const QString& tableName)
{
    Q_D(AbstractEntityDao);
    d->m_tableName = tableName;
}

int AbstractEntityDao::resultLimit() const
{
    Q_D(const AbstractEntityDao);
    return d->m_resultLimit;
}

void AbstractEntityDao::setResultLimit(int resultLimit)
{
    Q_D(AbstractEntityDao);
    d->m_resultLimit = resultLimit;
}

int AbstractEntityDao::resultOffset() const
{
    Q_D(const AbstractEntityDao);
    return d->m_resultOffset;
}

void AbstractEntityDao::setResultOffset(int resultOffset)
{
    Q_D(AbstractEntityDao);
    d->m_resultOffset = resultOffset;
}

bool AbstractEntityDao::createTable()
{
    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "createTable failed because database is not available";
        return false;
    }

    qWarning() << DEBUG_PREFIX << "createTable is not supported";
    return false;
}

bool AbstractEntityDao::dropTable()
{
    static const char sql[] =
        "DROP TABLE \"%1\""
        ;

    Q_D(AbstractEntityDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "dropTable failed because database is not available";
        return false;
    }
    return d->m_db->exec(QString::fromLatin1(sql).arg(d->m_tableName));
}

QString AbstractEntityDao::sqlAddSort(const QString& sql) const
{
    return sql;
}

QString AbstractEntityDao::sqlAddLimits(const QString& sql, int limit, int offset) const
{
    Q_D(const AbstractEntityDao);

    if (d->m_db) {
        return d->m_db->sqlAddLimits(sql, limit, offset);
    } else {
        qWarning() << DEBUG_PREFIX << "sqlAddLimits failed because database is not available";
        return sql;
    }
}

QString AbstractEntityDao::sqlAddDaoLimits(const QString& sql) const
{
    Q_D(const AbstractEntityDao);

    if (d->m_db) {
        return d->m_db->sqlAddLimits(sql, d->m_resultLimit, d->m_resultOffset);
    } else {
        qWarning() << DEBUG_PREFIX << "sqlAddDaoLimits failed because database is not available";
        return sql;
    }
}

QString AbstractEntityDao::sqlQuote(const QString& string)
{
    const QChar q = QLatin1Char('"');

    QString ret(q);
    ret += string;
    ret += q;
    return ret;
}

QString AbstractEntityDao::sqlInParam(const int count)
{
    if (count <= 0) {
        return QString();
    }

    const QChar p = QLatin1Char('?');

    QString ret;
    StringSeparator sep(",");
    for (int i = 0; i < count; ++i) {
        ret += sep;
        ret += p;
    }

    return ret;
}

QString AbstractEntityDao::sqlInString(const QList<QString>& strings)
{
    if (strings.isEmpty()) {
        return QString();
    }

    const QChar q = QLatin1Char('\'');

    QString ret;
    StringSeparator sep(",");
    foreach (const QString& string, strings) {
        ret += sep;
        ret += q;
        ret += string;
        ret += q;
    }

    return ret;
}

QVariantList AbstractEntityDao::sqlAppendConditions(QString& sql, const QVariantHash& conds)
{
    QString sqlw;
    StringSeparator a(" AND ");

    QVariantList values;
    QHashIterator<QString, QVariant> iter(conds);

    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();

        if (!field.isEmpty() && value.isValid()) {
            (sqlw += a) += QLatin1Char('(');
            sqlw += sqlQuote(field);

            if (value.isNull()) {
                sqlw += QLatin1String(" IS NULL");

            } else if (value.type() == QVariant::List) {
                sqlw += QLatin1String(" IN (");

                StringSeparator c(",");
                bool hasNull = false;
                foreach (const QVariant& v, value.toList()) {
                    if (!v.isValid())
                        continue;

                    if (v.isNull()) {
                        hasNull = true;
                    } else {
                        (sqlw += c) += QLatin1String("?");
                        values << v;
                    }
                }

                sqlw += QLatin1String(")");

                if (hasNull) {
                    sqlw += QLatin1String(" OR ");
                    sqlw += sqlQuote(field);
                    sqlw += QLatin1String(" IS NULL");
                }

            } else {
                sqlw += QLatin1String("=?");
                values << value;
            }

            sqlw += QLatin1Char(')');
        }
    }

    if (!sqlw.isEmpty()) {
        sql += QLatin1String(" WHERE ");
        sql += sqlw;
    }

    return values;
}

} // namespace ns
