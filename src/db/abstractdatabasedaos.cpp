#include <QtCore/QDebug>
#include "abstractdatabasedaos.h"
#include "abstractdatabasedaos_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractDatabaseDaos:"
#endif // DEBUG_PREFIX

namespace ns {

AbstractDatabaseDaosPrivate::AbstractDatabaseDaosPrivate() :
    q_ptr(0)
{
}

AbstractDatabaseDaosPrivate::~AbstractDatabaseDaosPrivate()
{
}

AbstractDatabaseDaos::AbstractDatabaseDaos() :
    d_ptr(new AbstractDatabaseDaosPrivate)
{
    d_ptr->q_ptr = this;
}

AbstractDatabaseDaos::AbstractDatabaseDaos(AbstractDatabaseDaosPrivate& dd) :
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractDatabaseDaos::~AbstractDatabaseDaos()
{
    delete d_ptr;
}

bool AbstractDatabaseDaos::isValid() const
{
    Q_D(const AbstractDatabaseDaos);
    return d->m_db != 0;
}

Database* AbstractDatabaseDaos::database() const
{
    Q_D(const AbstractDatabaseDaos);
    return d->m_db;
}

void AbstractDatabaseDaos::setDatabase(Database* db)
{
    Q_D(AbstractDatabaseDaos);

    if (d->m_db == db)
        return;

    d->m_db = db;
    updateDatabase();
}

void AbstractDatabaseDaos::updateDatabase()
{
    // Noop
}

} // namespace ns
