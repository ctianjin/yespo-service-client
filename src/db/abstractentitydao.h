#ifndef ABSTRACTENTITYDAO_H
#define ABSTRACTENTITYDAO_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QVariant;
class QSqlError;
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {

class Database;
class AbstractEntityDaoPrivate;

class BUILDSYS_DB_DECL AbstractEntityDao
{
public:
    explicit AbstractEntityDao(Database* db = 0, const QString& tableName = QString());
    virtual ~AbstractEntityDao();

    bool isValid() const;

    Database* database() const;
    void setDatabase(Database* db);

    bool isError() const;
    QString errorText() const;
    QSqlError lastError() const;

    QString tableName() const;
    void setTableName(const QString& tableName);

    int resultLimit() const;
    void setResultLimit(int resultLimit);

    int resultOffset() const;
    void setResultOffset(int resultOffset);

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities() = 0;

protected:
    AbstractEntityDao(AbstractEntityDaoPrivate& dd);

    virtual QString sqlAddSort(const QString& sql) const;
    virtual QString sqlAddLimits(const QString& sql, int limit, int offset = -1) const;
    virtual QString sqlAddDaoLimits(const QString& sql) const;

    QString sqlAddSort(const char* sql) const;
    QString sqlAddLimits(const char* sql, int limit, int offset = -1) const;
    QString sqlAddDaoLimits(const char* sql) const;
    QString sqlAddLimitOne(const QString& sql) const;
    QString sqlAddLimitOne(const char* sql) const;
    QString sqlDecorate(const QString& sql) const;
    QString sqlDecorate(const char* sql) const;
    QString sqlDecorateOne(const QString& sql) const;
    QString sqlDecorateOne(const char* sql) const;

    static QString sqlQuote(const QString& string);
    static QString sqlInParam(const int count);
    static QString sqlInString(const QList<QString>& strings);
    static QVariantList sqlAppendConditions(QString& sql, const QVariantHash& conds);

    AbstractEntityDaoPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractEntityDao)
    Q_DISABLE_COPY(AbstractEntityDao)
};

inline QString AbstractEntityDao::sqlAddSort(const char* sql) const
{
    return sqlAddSort(QLatin1String(sql));
}

inline QString AbstractEntityDao::sqlAddLimits(const char* sql, int limit, int offset) const
{
    return sqlAddLimits(QLatin1String(sql), limit, offset);
}

inline QString AbstractEntityDao::sqlAddDaoLimits(const char* sql) const
{
    return sqlAddDaoLimits(QLatin1String(sql));
}

inline QString AbstractEntityDao::sqlAddLimitOne(const QString& sql) const
{
    return sqlAddLimits(sql, 1);
}

inline QString AbstractEntityDao::sqlAddLimitOne(const char* sql) const
{
    return sqlAddLimitOne(QLatin1String(sql));
}

inline QString AbstractEntityDao::sqlDecorate(const QString& sql) const
{
    return sqlAddDaoLimits(sqlAddSort(sql));
}

inline QString AbstractEntityDao::sqlDecorate(const char* sql) const
{
    return sqlAddDaoLimits(sqlAddSort(sql));
}

inline QString AbstractEntityDao::sqlDecorateOne(const QString& sql) const
{
    return sqlAddLimitOne(sqlAddSort(sql));
}

inline QString AbstractEntityDao::sqlDecorateOne(const char* sql) const
{
    return sqlAddLimitOne(sqlAddSort(sql));
}

} // namespace ns

#endif // ABSTRACTENTITYDAO_H
