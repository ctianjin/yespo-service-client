#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QtCore/QList>
#include <QtCore/QVariant>
#ifdef Q_OS_DARWIN
#include <QtSql/QSqlRecord>
#endif
#include <QtSql/QSqlQuery>
#include "database.h"

namespace ns {

template <typename T>
class QueryValueCreator
{
public:
    static T create(const QVariant& v)
    {
        return v.value<T>();
    }
};

template <>
class QueryValueCreator<QVariant>
{
public:
    static QVariant create(const QVariant& v)
    {
        return v;
    }
};

template <typename T, typename Creator = QueryValueCreator<T> >
class QueryValue
{
public:
    explicit QueryValue(Database* db) :
        m_db(db)
    {
        Q_ASSERT(m_db);
    }

    T operator()(const QString& sql, int column = 0) const
    {
        QSqlQuery query = m_db->query();
        query.setForwardOnly(true);
        query.prepare(sql);
        return (*this)(query, column);
    }

    T operator()(QSqlQuery query, int column = 0) const
    {
        if (m_db->exec(query)) {
            if (query.next()) {
                return Creator::create(query.value(column));
            }
        }
        return Creator::create(QVariant());
    }

private:
    Database* m_db;
};

template <typename T, typename Creator = QueryValueCreator<T> >
class QueryValues
{
public:
    explicit QueryValues(Database* db) :
        m_db(db)
    {
        Q_ASSERT(m_db);
    }

    QList<T> operator()(const QString& sql, int column = 0) const
    {
        QSqlQuery query = m_db->query();
        query.setForwardOnly(true);
        query.prepare(sql);
        return (*this)(query, column);
    }

    QList<T> operator()(QSqlQuery query, int column = 0) const
    {
        QList<T> ret;
        if (m_db->exec(query)) {
            while (query.next()) {
                ret << Creator::create(query.value(column));
            }
        }
        return ret;
    }

private:
    Database* m_db;
};

template <typename T>
class QueryResultCreator
{
public:
    static T create(const QSqlQuery& query)
    {
        return T(query);
    }
};

template <typename T>
class QueryResultCreatorByRecord
{
public:
    static T create(const QSqlQuery& query)
    {
        return T(query.record());
    }
};

template <typename T, typename Creator = QueryResultCreator<T> >
class QueryResult
{
public:
    explicit QueryResult(Database* db) :
        m_db(db)
    {
        Q_ASSERT(m_db);
    }

    T operator()(const QString& sql) const
    {
        QSqlQuery query = m_db->query();
        query.setForwardOnly(true);
        query.prepare(sql);
        return (*this)(query);
    }

    T operator()(QSqlQuery query) const
    {
        if (m_db->exec(query)) {
            if (query.next()) {
                return Creator::create(query);
            }
        }
        return T();
    }

private:
    Database* m_db;
};

template <typename T, typename Creator = QueryResultCreator<T> >
class QueryResults
{
public:
    explicit QueryResults(Database* db) :
        m_db(db)
    {
        Q_ASSERT(m_db);
    }

    QList<T> operator()(const QString& sql) const
    {
        QSqlQuery query = m_db->query();
        query.setForwardOnly(true);
        query.prepare(sql);
        return (*this)(query);
    }

    QList<T> operator()(QSqlQuery query) const
    {
        QList<T> ret;
        if (m_db->exec(query)) {
            while (query.next()) {
                ret << Creator::create(query);
            }
        }
        return ret;
    }

private:
    Database* m_db;
};

} // namespace ns

#endif // QUERYRESULT_H
