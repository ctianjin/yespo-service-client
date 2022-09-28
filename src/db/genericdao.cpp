#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtSql/QSqlQuery>
#include "genericdao.h"
#include "genericdao_p.h"
#include "database.h"
#include "record_p.h"
#include "queryresult.h"
#include "stringseparator.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "GenericDao:"
#endif // DEBUG_PREFIX

namespace ns {

typedef QueryResult<Record> RecordResult;
typedef QueryResult<Record, QueryResultCreatorByRecord<Record> > RecordResultByRecord;
typedef QueryResults<Record> RecordResults;
typedef QueryResults<Record, QueryResultCreatorByRecord<Record> > RecordResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

GenericDaoBasePrivate::GenericDaoBasePrivate(Database* db, const QString& tableName) :
    AbstractEntityDaoPrivate(db, tableName)
{
}

GenericDaoBasePrivate::~GenericDaoBasePrivate()
{
}

GenericDaoBase::GenericDaoBase(Database* db, const QString& tableName) :
    AbstractEntityDao(*new GenericDaoBasePrivate(db, tableName))
{
}

GenericDaoBase::GenericDaoBase(GenericDaoBasePrivate& dd) :
    AbstractEntityDao(dd)
{
}

GenericDaoBase::~GenericDaoBase()
{
}

int GenericDaoBase::countByExample(const Record& record)
{
    Q_D(GenericDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "countByExample failed because database is not available";
        return -1;
    }

    const RecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT COUNT(*) FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sqlw += a) += sqlQuote(field);
            sqlw += QLatin1String("=?");
            values << value;
        }
    }

    sql += sqlw;

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }

    const QVariant v = QueryValue<QVariant>(d->m_db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int GenericDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM \"%1\""
        ;

    Q_D(GenericDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "countAll failed because database is not available";
        return -1;
    }

    const QVariant v = QueryValue<QVariant>(d->m_db)(QString::fromLatin1(sql).arg(d->m_tableName));
    return v.isValid() ? v.toInt() : -1;
}

bool GenericDaoBase::persist(const Record& record)
{
    Q_D(GenericDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "persist failed because database is not available";
        return false;
    }

    const RecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("INSERT INTO \"%1\" (").arg(d->m_tableName);
    QString sqlv = QLatin1String("VALUES (");
    SqlComma c, cv;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sql += c) += sqlQuote(field);
            (sqlv += cv) += QLatin1String("?");
            values << value;
        }
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = d->m_db->query();
    query.prepare(sql);
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }

    return d->m_db->exec(query);
}

bool GenericDaoBase::merge(const Record& record)
{
    Q_D(GenericDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "merge failed because database is not available";
        return false;
    }

    const RecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("INSERT OR REPLACE INTO \"%1\" (").arg(d->m_tableName);
    QString sqlv = QLatin1String("VALUES (");
    SqlComma c, cv;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sql += c) += sqlQuote(field);
            (sqlv += cv) += QLatin1String("?");
            values << value;
        }
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = d->m_db->query();;
    query.prepare(sql);
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }

    return d->m_db->exec(query);
}

bool GenericDaoBase::removeAll()
{
    static const char sql[] =
        "DELETE FROM \"%1\""
        ;

    Q_D(GenericDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "removeAll failed because database is not available";
        return false;
    }

    return d->m_db->exec(QString::fromLatin1(sql).arg(d->m_tableName));
}

QSqlQuery GenericDaoBase::findByExampleQuery(const Record& record)
{
    Q_D(GenericDaoBase);
    Q_ASSERT(isValid());

    const RecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT * FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sqlw += a) += sqlQuote(field);
            sqlw += QLatin1String("=?");
            values << value;
        }
    }

    sql += sqlw;

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }

    return query;
}

QSqlQuery GenericDaoBase::findAllQuery()
{
    static const char sql[] =
        "SELECT * FROM \"%1\""
        ;

    Q_D(GenericDaoBase);
    Q_ASSERT(isValid());

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(QString::fromLatin1(sql).arg(d->m_tableName)));

    return query;
}

QSqlQuery GenericDaoBase::findAllByExampleQuery(const Record& record)
{
    Q_D(GenericDaoBase);
    Q_ASSERT(isValid());

    const RecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT * FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sqlw += a) += sqlQuote(field);
            sqlw += QLatin1String("=?");
            values << value;
        }
    }

    sql += sqlw;

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }

    return query;
}

GenericDaoPrivate::GenericDaoPrivate(Database* db, const QString& tableName) :
    GenericDaoBasePrivate(db, tableName)
{
}

GenericDaoPrivate::~GenericDaoPrivate()
{
}

GenericDao::GenericDao(Database* db, const QString& tableName) :
    GenericDaoBase(*new GenericDaoPrivate(db, tableName))
{
}

GenericDao::GenericDao(GenericDaoPrivate& dd) :
    GenericDaoBase(dd)
{
}

GenericDao::~GenericDao()
{
}

QVariant GenericDao::entities()
{
    return QVariant::fromValue(findAll());
}

Record GenericDao::findByExample(const Record& record)
{
    Q_D(GenericDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findByExample failed because database is not available";
        return Record();
    }

    QSqlQuery query = findByExampleQuery(record);
    return RecordResult(d->m_db)(query);
}

RecordList GenericDao::findAll()
{
    Q_D(GenericDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findAll failed because database is not available";
        return RecordList();
    }

    QSqlQuery query = findAllQuery();
    return RecordResults(d->m_db)(query);
}

RecordList GenericDao::findAllByExample(const Record& record)
{
    Q_D(GenericDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findAllByExample failed because database is not available";
        return RecordList();
    }

    QSqlQuery query = findAllByExampleQuery(record);
    return RecordResults(d->m_db)(query);
}

} // namespace ns
