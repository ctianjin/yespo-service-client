#ifndef ABSTRACTDATABASEDAOS_P_H
#define ABSTRACTDATABASEDAOS_P_H

#include <QtCore/QPointer>
#include "abstractdatabasedaos.h"
#include "database.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL AbstractDatabaseDaosPrivate
{
    Q_DECLARE_PUBLIC(AbstractDatabaseDaos)

public:
    AbstractDatabaseDaos* q_ptr;

    QPointer<Database> m_db;

    AbstractDatabaseDaosPrivate();
    virtual ~AbstractDatabaseDaosPrivate();

private:
    Q_DISABLE_COPY(AbstractDatabaseDaosPrivate)
};

} // namespace ns

#endif // ABSTRACTDATABASEDAOS_P_H
