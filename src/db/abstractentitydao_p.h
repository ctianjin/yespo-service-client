#ifndef ABSTRACTENTITYDAO_P_H
#define ABSTRACTENTITYDAO_P_H

#include <QtCore/QPointer>
#include <QtCore/QString>
#include "abstractentitydao.h"
#include "dbglobal.h"

namespace ns {

class Database;

class BUILDSYS_DB_DECL AbstractEntityDaoPrivate
{
    Q_DECLARE_PUBLIC(AbstractEntityDao)

public:
    AbstractEntityDao* q_ptr;

    QPointer<Database> m_db;
    QString m_tableName;
    int m_resultLimit;
    int m_resultOffset;

    explicit AbstractEntityDaoPrivate(Database* db = 0, const QString& tableName = QString());
    virtual ~AbstractEntityDaoPrivate();

private:
    Q_DISABLE_COPY(AbstractEntityDaoPrivate)
};

} // namespace ns

#endif // ABSTRACTENTITYDAO_P_H
