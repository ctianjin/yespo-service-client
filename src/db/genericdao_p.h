#ifndef GENERICDAO_P_H
#define GENERICDAO_P_H

#include "genericdao.h"
#include "abstractentitydao_p.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL GenericDaoBasePrivate : public AbstractEntityDaoPrivate
{
    Q_DECLARE_PUBLIC(GenericDaoBase)

public:
    explicit GenericDaoBasePrivate(Database* db = 0, const QString& tableName = QString());
    virtual ~GenericDaoBasePrivate();

private:
    Q_DISABLE_COPY(GenericDaoBasePrivate)
};

class BUILDSYS_DB_DECL GenericDaoPrivate : public GenericDaoBasePrivate
{
    Q_DECLARE_PUBLIC(GenericDao)

public:
    explicit GenericDaoPrivate(Database* db = 0, const QString& tableName = QString());
    virtual ~GenericDaoPrivate();

private:
    Q_DISABLE_COPY(GenericDaoPrivate)
};

} // namespace ns

#endif // GENERICDAO_P_H
