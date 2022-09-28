#ifndef GENERICIDENTITYDAO_P_H
#define GENERICIDENTITYDAO_P_H

#include "genericidentitydao.h"
#include "abstractentitydao_p.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL GenericIdentityDaoBasePrivate : public AbstractEntityDaoPrivate
{
    Q_DECLARE_PUBLIC(GenericIdentityDaoBase)

public:
    explicit GenericIdentityDaoBasePrivate(Database* db = 0, const QString& tableName = QString());
    virtual ~GenericIdentityDaoBasePrivate();

private:
    Q_DISABLE_COPY(GenericIdentityDaoBasePrivate)
};

class BUILDSYS_DB_DECL GenericIdentityDaoPrivate : public GenericIdentityDaoBasePrivate
{
    Q_DECLARE_PUBLIC(GenericIdentityDao)

public:
    explicit GenericIdentityDaoPrivate(Database* db = 0, const QString& tableName = QString());
    virtual ~GenericIdentityDaoPrivate();

private:
    Q_DISABLE_COPY(GenericIdentityDaoPrivate)
};

} // namespace ns

#endif // GENERICIDENTITYDAO_P_H
