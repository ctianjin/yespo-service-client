#ifndef GENERICDAO_H
#define GENERICDAO_H

#include "abstractentitydao.h"
#include "record.h"
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QSqlQuery;
QT_END_NAMESPACE

namespace ns {

class GenericDaoBasePrivate;
class GenericDaoPrivate;

class BUILDSYS_DB_DECL GenericDaoBase : public AbstractEntityDao
{
public:
    explicit GenericDaoBase(Database* db, const QString& tableName = QString());
    virtual ~GenericDaoBase();

    virtual int countByExample(const Record& record);
    virtual int countAll();

    virtual bool persist(const Record& record);
    virtual bool merge(const Record& record);
    virtual bool removeAll();

protected:
    GenericDaoBase(GenericDaoBasePrivate& dd);

    virtual QSqlQuery findByExampleQuery(const Record& record);
    virtual QSqlQuery findAllQuery();
    virtual QSqlQuery findAllByExampleQuery(const Record& record);

private:
    Q_DECLARE_PRIVATE(GenericDaoBase)
    Q_DISABLE_COPY(GenericDaoBase)
};

class BUILDSYS_DB_DECL GenericDao : public GenericDaoBase
{
public:
    typedef Record Entity;
    typedef RecordList EntityList;

    explicit GenericDao(Database* db, const QString& tableName = QString());
    virtual ~GenericDao();

    virtual QVariant entities();

    virtual Record findByExample(const Record& record);
    virtual RecordList findAll();
    virtual RecordList findAllByExample(const Record& record);

protected:
    GenericDao(GenericDaoPrivate& dd);

private:
    Q_DECLARE_PRIVATE(GenericDao)
    Q_DISABLE_COPY(GenericDao)
};

} // namespace ns

#endif // GENERICDAO_H
