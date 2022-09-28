#ifndef GENERICIDENTITYDAO_H
#define GENERICIDENTITYDAO_H

#include "abstractentitydao.h"
#include "identityrecord.h"
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QSqlQuery;
QT_END_NAMESPACE

namespace ns {

class GenericIdentityDaoBasePrivate;
class GenericIdentityDaoPrivate;

class BUILDSYS_DB_DECL GenericIdentityDaoBase : public AbstractEntityDao
{
public:
    explicit GenericIdentityDaoBase(Database* db, const QString& tableName = QString());
    virtual ~GenericIdentityDaoBase();

    virtual int count(qlonglong id);
    virtual int countByExample(const IdentityRecord& record);
    virtual int countAll();

    virtual QVariant findValue(qlonglong id, const QString& field);
    virtual QList<qlonglong> findAllIds();

    virtual bool persist(IdentityRecord& record);
    virtual bool merge(const IdentityRecord& record);
    virtual bool update(const IdentityRecord& record);
    virtual bool updateValue(qlonglong id, const QString& field, const QVariant& value);
    virtual bool remove(const IdentityRecord& record);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(IdentityRecord& record);

protected:
    GenericIdentityDaoBase(GenericIdentityDaoBasePrivate& dd);

    virtual QSqlQuery findQuery(qlonglong id);
    virtual QSqlQuery findByExampleQuery(const IdentityRecord& record);
    virtual QSqlQuery findAllQuery();
    virtual QSqlQuery findAllByExampleQuery(const IdentityRecord& record);

private:
    Q_DECLARE_PRIVATE(GenericIdentityDaoBase)
    Q_DISABLE_COPY(GenericIdentityDaoBase)
};

class BUILDSYS_DB_DECL GenericIdentityDao : public GenericIdentityDaoBase
{
public:
    typedef IdentityRecord Entity;
    typedef IdentityRecordList EntityList;

    explicit GenericIdentityDao(Database* db, const QString& tableName = QString());
    virtual ~GenericIdentityDao();

    virtual QVariant entities();

    virtual IdentityRecord find(qlonglong id);
    virtual IdentityRecord findByExample(const IdentityRecord& record);
    virtual IdentityRecordList findAll();
    virtual IdentityRecordList findAllByExample(const IdentityRecord& record);

protected:
    GenericIdentityDao(GenericIdentityDaoPrivate& dd);

private:
    Q_DECLARE_PRIVATE(GenericIdentityDao)
    Q_DISABLE_COPY(GenericIdentityDao)
};

} // namespace ns

#endif // GENERICIDENTITYDAO_H
