#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtSql/QSqlQuery>
#include "genericidentitydao.h"
#include "genericidentitydao_p.h"
#include "database.h"
#include "transaction.h"
#include "identityrecord_p.h"
#include "queryresult.h"
#include "stringseparator.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "GenericIdentityDao:"
#endif // DEBUG_PREFIX

namespace ns {

typedef QueryResult<IdentityRecord> IdentityRecordResult;
typedef QueryResult<IdentityRecord, QueryResultCreatorByRecord<IdentityRecord> > IdentityRecordResultByRecord;
typedef QueryResults<IdentityRecord> IdentityRecordResults;
typedef QueryResults<IdentityRecord, QueryResultCreatorByRecord<IdentityRecord> > IdentityRecordResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

GenericIdentityDaoBasePrivate::GenericIdentityDaoBasePrivate(Database* db, const QString& tableName) :
    AbstractEntityDaoPrivate(db, tableName)
{
}

GenericIdentityDaoBasePrivate::~GenericIdentityDaoBasePrivate()
{
}

GenericIdentityDaoBase::GenericIdentityDaoBase(Database* db, const QString& tableName) :
    AbstractEntityDao(*new GenericIdentityDaoBasePrivate(db, tableName))
{
}

GenericIdentityDaoBase::GenericIdentityDaoBase(GenericIdentityDaoBasePrivate& dd) :
    AbstractEntityDao(dd)
{
}

GenericIdentityDaoBase::~GenericIdentityDaoBase()
{
}

int GenericIdentityDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM \"%1\" "
         "WHERE \"%2\"=?"
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "count failed because database is not available";
        return -1;
    }

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField));
    query.addBindValue(id);

    const QVariant v = QueryValue<QVariant>(d->m_db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int GenericIdentityDaoBase::countByExample(const IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "countByExample failed because database is not available";
        return -1;
    }

    const IdentityRecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT COUNT(*) FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;

    if (e->m_id.isValid()) {
        (sqlw += a) += IdentityRecord::idField;
        sqlw += QLatin1String("=?");
        values << e->m_id;
    }

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

int GenericIdentityDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM \"%1\""
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "countAll failed because database is not available";
        return -1;
    }

    const QVariant v = QueryValue<QVariant>(d->m_db)(QString::fromLatin1(sql).arg(d->m_tableName));
    return v.isValid() ? v.toInt() : -1;
}

QVariant GenericIdentityDaoBase::findValue(qlonglong id, const QString& field)
{
    static const char sql[] =
        "SELECT \"%3\" FROM \"%1\" "
         "WHERE \"%2\"=?"
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findValue failed because database is not available";
        return QVariant();
    }

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField, field));
    query.addBindValue(id);

    return QueryValue<QVariant>(d->m_db)(query);
}

QList<qlonglong> GenericIdentityDaoBase::findAllIds()
{
    static const char sql[] =
        "SELECT \"%2\" FROM \"%1\""
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findAllIds failed because database is not available";
        return QList<qlonglong>();
    }

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(
        QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField)));

    return QueryValues<qlonglong>(d->m_db)(query);
}

bool GenericIdentityDaoBase::persist(IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "persist failed because database is not available";
        return false;
    }

    const IdentityRecordPrivate* e = record.d;

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

    if (d->m_db->exec(query)) {
        const QVariant id = query.lastInsertId();
        record.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool GenericIdentityDaoBase::merge(const IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "merge failed because database is not available";
        return false;
    }

    const IdentityRecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("INSERT OR REPLACE INTO \"%1\" (").arg(d->m_tableName);
    QString sqlv = QLatin1String("VALUES (");
    SqlComma c, cv;

    QVariantList values;

    if (e->m_id.isValid()) {
        (sql += c) += IdentityRecord::idField;
        (sqlv += cv) += QLatin1String("?");
        values << e->m_id;
    }

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

bool GenericIdentityDaoBase::update(const IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "update failed because database is not available";
        return false;
    }

    const IdentityRecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("UPDATE \"%1\" SET ").arg(d->m_tableName);
    QString sqlv;
    SqlComma c;

    QVariantList values;
    QHashIterator<QString, QVariant> iter(e->m_data);
    while (iter.hasNext()) {
        iter.next();

        const QString field = iter.key();
        const QVariant value = iter.value();
        if (!field.isEmpty() && value.isValid()) {
            (sqlv += c) += sqlQuote(field);
            sqlv += QLatin1String("=?");
            values << value;
        }
    }

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QString::fromLatin1(" WHERE \"%1\"=?").arg(IdentityRecord::idField);

    QSqlQuery query = d->m_db->query();
    query.prepare(sql);
    foreach (const QVariant& value, values) {
        query.addBindValue(value);
    }
    query.addBindValue(e->m_id);

    return d->m_db->exec(query);
}

bool GenericIdentityDaoBase::updateValue(qlonglong id, const QString& field, const QVariant& value)
{
    static const char sql[] =
        "UPDATE \"%1\" SET \"%3\"=? "
         "WHERE \"%2\"=?"
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "updateValue failed because database is not available";
        return false;
    }

    QSqlQuery query = d->m_db->query();
    query.prepare(QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField, field));
    query.addBindValue(value);
    query.addBindValue(id);

    return d->m_db->exec(query);
}

bool GenericIdentityDaoBase::remove(const IdentityRecord& record)
{
    static const char sql[] =
        "DELETE FROM \"%1\" "
         "WHERE \"%2\"=?"
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "remove failed because database is not available";
        return false;
    }

    QSqlQuery query = d->m_db->query();
    query.prepare(QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField));
    query.addBindValue(record.d->m_id);

    return d->m_db->exec(query);
}

bool GenericIdentityDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM \"%1\""
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name=?"
        ;

    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "removeAll failed because database is not available";
        return false;
    }

    if (resetIdentity) {
        Transaction trans(d->m_db);
        if (!d->m_db->exec(QString::fromLatin1(sql).arg(d->m_tableName)))
            return false;
        {
            QSqlQuery query = d->m_db->query();
            query.prepare(QLatin1String(sqli));
            query.addBindValue(d->m_tableName);
            if (!d->m_db->exec(query))
                return false;
        }
        return trans.commit();
    } else {
        return d->m_db->exec(QString::fromLatin1(sql).arg(d->m_tableName));
    }
}

bool GenericIdentityDaoBase::refresh(IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "refresh failed because database is not available";
        return false;
    }

    QSqlQuery query = findQuery(record.id());

    if (!d->m_db->exec(query))
        return false;
    if (!query.next())
        return false;

    record = IdentityRecord(query);
    return true;
}

QSqlQuery GenericIdentityDaoBase::findQuery(qlonglong id)
{
    static const char sql[] =
        "SELECT * FROM \"%1\" "
         "WHERE \"%2\"=?"
        ;

    Q_D(GenericIdentityDaoBase);
    Q_ASSERT(isValid());

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(QString::fromLatin1(sql).arg(d->m_tableName, IdentityRecord::idField));
    query.addBindValue(id);

    return query;
}

QSqlQuery GenericIdentityDaoBase::findByExampleQuery(const IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);
    Q_ASSERT(isValid());

    const IdentityRecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT * FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;

    if (e->m_id.isValid()) {
        (sqlw += a) += IdentityRecord::idField;
        sqlw += QLatin1String("=?");
        values << e->m_id;
    }

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

QSqlQuery GenericIdentityDaoBase::findAllQuery()
{
    static const char sql[] =
        "SELECT * FROM \"%1\""
        ;

    Q_D(GenericIdentityDaoBase);
    Q_ASSERT(isValid());

    QSqlQuery query = d->m_db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(QString::fromLatin1(sql).arg(d->m_tableName)));

    return query;
}

QSqlQuery GenericIdentityDaoBase::findAllByExampleQuery(const IdentityRecord& record)
{
    Q_D(GenericIdentityDaoBase);
    Q_ASSERT(isValid());

    const IdentityRecordPrivate* e = record.d;

    QString sql = QString::fromLatin1("SELECT * FROM \"%1\" WHERE ").arg(d->m_tableName);
    QString sqlw;
    SqlAnd a;

    QVariantList values;

    if (e->m_id.isValid()) {
        (sqlw += a) += IdentityRecord::idField;
        sqlw += QLatin1String("=?");
        values << e->m_id;
    }

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

GenericIdentityDaoPrivate::GenericIdentityDaoPrivate(Database* db, const QString& tableName) :
    GenericIdentityDaoBasePrivate(db, tableName)
{
}

GenericIdentityDaoPrivate::~GenericIdentityDaoPrivate()
{
}

GenericIdentityDao::GenericIdentityDao(Database* db, const QString& tableName) :
    GenericIdentityDaoBase(*new GenericIdentityDaoPrivate(db, tableName))
{
}

GenericIdentityDao::GenericIdentityDao(GenericIdentityDaoPrivate& dd) :
    GenericIdentityDaoBase(dd)
{
}

GenericIdentityDao::~GenericIdentityDao()
{
}

QVariant GenericIdentityDao::entities()
{
    return QVariant::fromValue(findAll());
}

IdentityRecord GenericIdentityDao::find(qlonglong id)
{
    Q_D(GenericIdentityDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "find failed because database is not available";
        return IdentityRecord();
    }

    QSqlQuery query = findQuery(id);
    return IdentityRecordResult(d->m_db)(query);
}

IdentityRecord GenericIdentityDao::findByExample(const IdentityRecord& record)
{
    Q_D(GenericIdentityDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findByExample failed because database is not available";
        return IdentityRecord();
    }

    QSqlQuery query = findByExampleQuery(record);
    return IdentityRecordResult(d->m_db)(query);
}

IdentityRecordList GenericIdentityDao::findAll()
{
    Q_D(GenericIdentityDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findAll failed because database is not available";
        return IdentityRecordList();
    }

    QSqlQuery query = findAllQuery();
    return IdentityRecordResults(d->m_db)(query);
}

IdentityRecordList GenericIdentityDao::findAllByExample(const IdentityRecord& record)
{
    Q_D(GenericIdentityDao);

    if (!isValid()) {
        qWarning() << DEBUG_PREFIX << "findAllByExample failed because database is not available";
        return IdentityRecordList();
    }

    QSqlQuery query = findAllByExampleQuery(record);
    return IdentityRecordResults(d->m_db)(query);
}

} // namespace ns
