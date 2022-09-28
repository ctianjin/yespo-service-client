#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QtCore/QPointer>
#include "dbglobal.h"

namespace ns {

class Database;

class BUILDSYS_DB_DECL Transaction
{
public:
    explicit Transaction(Database* db);
    ~Transaction();

    bool commit();
    bool rollback();

private:
    QPointer<Database> m_db;
    bool m_valid;

    Q_DISABLE_COPY(Transaction)
};

} // namespace ns

#endif // TRANSACTION_H
