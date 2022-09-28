#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include "transaction.h"
#include "database.h"
#include "database_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Transaction:"
#endif // DEBUG_PREFIX

namespace ns {

Transaction::Transaction(Database* db) :
    m_db(),
    m_valid(false)
{
    if (db) {
        DatabasePrivate* dbd = db->d_func();
        if (!dbd->transaction()) {
            m_db = db;
            dbd->setTransaction(this);

            QSqlDatabase sqldb = m_db->sqlDatabase();
            if (sqldb.isOpen()) {
                qDebug() << DEBUG_PREFIX << "Begin";
                if (sqldb.transaction()) {
                    qDebug() << DEBUG_PREFIX << "Begin succeeded";
                    m_valid = true;
                } else {
                    m_db->check(sqldb);
                }
            }
        }
    }
}

Transaction::~Transaction()
{
    if (m_db) {
        if (m_valid) {
            qDebug() << DEBUG_PREFIX << "Rollback on destruction";
            m_db->sqlDatabase().rollback(); // XXX Ignores error status
        }

        DatabasePrivate* dbd = m_db->d_func();
        Q_ASSERT(dbd->transaction() == this);
        dbd->setTransaction(0);
    }
}

bool Transaction::commit()
{
    if (!m_db || !m_valid)
        return true;

    qDebug() << DEBUG_PREFIX << "Commit";

    QSqlDatabase sqldb = m_db->sqlDatabase();
    bool ret = sqldb.commit();

    if (ret) {
        qDebug() << DEBUG_PREFIX << "Commit succeeded";
        m_valid = false;
    } else {
        m_db->check(sqldb);
    }

    return ret;
}

bool Transaction::rollback()
{
    if (!m_db || !m_valid)
        return true;

    qDebug() << DEBUG_PREFIX << "Rollback";

    QSqlDatabase sqldb = m_db->sqlDatabase();
    bool ret = sqldb.rollback();

    if (ret) {
        qDebug() << DEBUG_PREFIX << "Rollback succeeded";
        m_valid = false;
    } else {
        m_db->check(sqldb);
    }

    return ret;
}

} // namespace ns
