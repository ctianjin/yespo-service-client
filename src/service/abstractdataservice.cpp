#include <QtCore/QDebug>
#include "abstractdataservice.h"
#include "abstractdataservice_p.h"
#include "database.h"
#include "databaseregistrar.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractDataService:"
#endif // DEBUG_PREFIX

namespace ns {

AbstractDataServicePrivate::AbstractDataServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
    m_db(0)
{
}

AbstractDataServicePrivate::~AbstractDataServicePrivate()
{
    if (m_db) {
        if (m_db->isOpen()) {
            m_db->close();
        }

        DatabaseRegistrar* registrar = databaseRegistrar();
        if (registrar) {
            registrar->unregisterDatabase(m_db);
        }
    }
}

void AbstractDataServicePrivate::setDatabase(Database* db)
{
    Q_Q(AbstractDataService);

    if (m_db == db)
        return;

    if (m_db) {
        if (m_db->isOpen()) {
            m_db->close();
        }

        DatabaseRegistrar* registrar = databaseRegistrar();
        if (registrar) {
            registrar->unregisterDatabase(m_db);
        }

        m_db->deleteLater();
        m_db = 0;
    }

    m_db = db;
    if (m_db) {
        m_db->setParent(q);

        DatabaseRegistrar* registrar = databaseRegistrar();
        if (registrar) {
            registrar->registerDatabase(m_db);
        }
    }
}

bool AbstractDataServicePrivate::isDatabaseOpen() const
{
    return m_db && m_db->isOpen();
}

bool AbstractDataServicePrivate::openDatabase()
{
    if (!m_db) {
        qWarning() << DEBUG_PREFIX << "Database not available";
        return false;
    }

    if (m_db->isOpen()) {
        qDebug() << DEBUG_PREFIX << "Already opened";
        return true;
    }

    m_db->setLocation(dataLocation());

    if (!validateDatabase()) {
        qCritical() << DEBUG_PREFIX << "Failed to open";
        return false;
    }

    if (!m_db->open()) {
        qCritical() << DEBUG_PREFIX << "Failed to open";
        return false;
    }

    qDebug() << DEBUG_PREFIX << "Opened successfully";
    return true;
}

void AbstractDataServicePrivate::closeDatabase()
{
    if (!m_db) {
        qWarning() << DEBUG_PREFIX << "Database not available";
        return;
    }

    if (!m_db->isOpen()) {
        qDebug() << DEBUG_PREFIX << "Not open";
        return;
    }

    m_db->close();

    qDebug() << DEBUG_PREFIX << "Closed successfully";
}

bool AbstractDataServicePrivate::validateDatabase()
{
    if (!m_db) {
        qWarning() << DEBUG_PREFIX << "Database not available";
        return false;
    }

    bool ret = false;
    Database::ValidateResult result = m_db->validate();

    switch (result) {
        case Database::ValidateOk:
        case Database::ValidateNew:
        case Database::ValidateCompatible:
            ret = true;
            break;

        case Database::ValidateIncompatible:
            qCritical() << DEBUG_PREFIX << "Incompatible database version";
            ret = false;
            break;

        case Database::ValidateError:
            qCritical() << DEBUG_PREFIX << "Error while validating database" << m_db->errorText();
            ret = false;
            break;

        default:
            qWarning() << DEBUG_PREFIX << "Unknown validation result";
            Q_ASSERT(false);
            ret = true;
            break;
    }

    return ret;
}

void AbstractDataServicePrivate::startup()
{
    AbstractServicePrivate::startup();
}

void AbstractDataServicePrivate::cleanup()
{
    if (m_db && m_db->isOpen()) {
        m_db->close();
    }

    AbstractServicePrivate::cleanup();
}

AbstractDataService::AbstractDataService(ServiceManager* manager, QObject* parent) :
    AbstractService(*new AbstractDataServicePrivate(manager, QString()), parent)
{
}

AbstractDataService::AbstractDataService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractService(*new AbstractDataServicePrivate(manager, name), parent)
{
}

AbstractDataService::AbstractDataService(AbstractDataServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
}

AbstractDataService::~AbstractDataService()
{
}

Database* AbstractDataService::database() const
{
    Q_D(const AbstractDataService);
    return d->m_db;
}

void AbstractDataService::start()
{
    Q_D(AbstractDataService);

    if (!isStopped()) {
        qDebug() << DEBUG_PREFIX << d->m_name << "start rejected because not stopped";
        return;
    }

    d->setError(Service::NoError);
    d->setState(Service::InitState);

    Q_ASSERT(!d->isDatabaseOpen());
    if (!d->openDatabase()) {
        d->stopForError(Service::DataError);
        return;
    }

    d->startup();
    if (d->m_error) {
        d->setState(Service::StoppedState);
    } else {
        d->setState(Service::ActiveState);
    }
}

void AbstractDataService::stop()
{
    AbstractService::stop();
}

} // namespace ns
