#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include "preferencemanager.h"
#include "preferencemanager_p.h"
#include "preferencetransaction_p.h"
#include "databaseregistrar.h"
#include "transaction.h"
#include "version.h"
#include "pref/preferencedatabase.h"
#include "pref/preferencedaos_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PreferenceManager:"
#endif // DEBUG_PREFIX

namespace ns {

const QString PreferenceManagerPrivate::defaultApplication = QLatin1String("");
const QString PreferenceManagerPrivate::versionPropertyFormat = QLatin1String("pref.version[%1]");
const QString PreferenceManagerPrivate::defaultPropertyFormat = QLatin1String("pref.default[%1]");

PreferenceManagerPrivate::PreferenceManagerPrivate(const QString& application) :
    q_ptr(0),
    m_application(application.isNull() ? defaultApplication : application),
    m_db(new Pref::PreferenceDatabase),
    m_defaultDao(new Pref::DefaultPreferenceDao(m_db.data())),
    m_userDao(new Pref::UserPreferenceDao(m_db.data()))
{
}

PreferenceManagerPrivate::~PreferenceManagerPrivate()
{
}

bool PreferenceManagerPrivate::validateDatabase()
{
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

QString PreferenceManagerPrivate::versionProperty() const
{
    return versionPropertyFormat.arg(m_application);
}

QString PreferenceManagerPrivate::defaultProperty() const
{
    return defaultPropertyFormat.arg(m_application);
}

QStringList PreferenceManagerPrivate::defaultSubPaths(const QString& path) const
{
    if (path.isEmpty()) {
        return m_defaultDao->findAllPathsByApplication(m_application);
    } else {
        QString prefix = path;
        Q_ASSERT(!prefix.isEmpty());
        prefix += QLatin1Char('.');

        return m_defaultDao->findAllPathsByPathPrefix(m_application, prefix);
    }
}

QStringList PreferenceManagerPrivate::userSubPaths(const QString& path) const
{
    if (path.isEmpty()) {
        return m_userDao->findAllPathsByApplication(m_application);
    } else {
        QString prefix = path;
        Q_ASSERT(!prefix.isEmpty());
        prefix += QLatin1Char('.');

        return m_userDao->findAllPathsByPathPrefix(m_application, prefix);
    }
}

bool PreferenceManagerPrivate::hasDefaultValue(const QString& path) const
{
    return m_defaultDao->count(m_application, path) > 0;
}

bool PreferenceManagerPrivate::hasUserValue(const QString& path) const
{
    return m_userDao->count(m_application, path) > 0;
}

QVariant PreferenceManagerPrivate::getDefaultValue(const QString& path) const
{
    Pref::DefaultPreference entity = m_defaultDao->find(m_application, path);
    return entity.isValid() ? entity.value() : QVariant();
}

QVariant PreferenceManagerPrivate::getUserValue(const QString& path) const
{
    Pref::UserPreference entity = m_userDao->find(m_application, path);
    return entity.isValid() ? entity.value() : QVariant();
}

void PreferenceManagerPrivate::putDefaultValue(const QString& path, const QVariant& value) const
{
    Pref::DefaultPreference entity;
    entity.setApplication(m_application);
    entity.setPath(path);
    entity.setValue(value);

    m_defaultDao->merge(entity);
}

void PreferenceManagerPrivate::putUserValue(const QString& path, const QVariant& value) const
{
    Pref::UserPreference entity;
    entity.setApplication(m_application);
    entity.setPath(path);
    entity.setValue(value);

    m_userDao->merge(entity);
}

void PreferenceManagerPrivate::removeDefaultValue(const QString& path) const
{
    Pref::DefaultPreference entity;
    entity.setApplication(m_application);
    entity.setPath(path);

    m_defaultDao->remove(entity);
}

void PreferenceManagerPrivate::removeUserValue(const QString& path) const
{
    Pref::UserPreference entity;
    entity.setApplication(m_application);
    entity.setPath(path);

    m_userDao->remove(entity);
}

void PreferenceManagerPrivate::removeAllDefaultValues() const
{
    m_defaultDao->removeAllByApplication(m_application);
}

void PreferenceManagerPrivate::removeAllUserValues() const
{
    m_userDao->removeAllByApplication(m_application);
}

QStringList PreferenceManagerPrivate::removeAllUserValues(const QString& path) const
{
    QStringList paths;

    Transaction trans(m_db.data());

    if (path.isEmpty()) {
        QList<QString> paths0 = m_userDao->findAllPathsByApplication(m_application);
        if (m_userDao->removeAllByApplication(m_application)) {
            paths = paths0;
        }
    } else {
        QString prefix = path;
        Q_ASSERT(!prefix.isEmpty());
        prefix += QLatin1Char('.');

        QList<QString> paths0 = m_userDao->findAllPathsByPathPrefix(m_application, prefix);
        if (m_userDao->removeAllByPathPrefix(m_application, prefix)) {
            paths = paths0;
        }

        Pref::UserPreference entity;
        entity.setApplication(m_application);
        entity.setPath(path);
        if (m_userDao->remove(entity)) {
            paths << path;
        }
    }

    if (!trans.commit()) {
        qCritical() << DEBUG_PREFIX << "removeAllUserValues failed";
        paths.clear();
    }

    qDebug() << DEBUG_PREFIX << "Removed paths:" << paths;
    return paths;
}

PreferenceTransactionContext* PreferenceManagerPrivate::newTransaction(AbstractPreference* initiator)
{
    Q_Q(PreferenceManager);

    m_transaction.reset(new PreferenceTransactionContext(q, initiator, m_db.data()));
    return m_transaction.data();
}

void PreferenceManagerPrivate::clearTransaction()
{
    m_transaction.reset();
}

PreferenceManager::PreferenceManager(QObject* parent) :
    QObject(parent),
    d_ptr(new PreferenceManagerPrivate)
{
    d_ptr->q_ptr = this;
}

PreferenceManager::PreferenceManager(const QString& application, QObject* parent) :
    QObject(parent),
    d_ptr(new PreferenceManagerPrivate(application))
{
    d_ptr->q_ptr = this;
}

PreferenceManager::~PreferenceManager()
{
    if (isOpen()) {
        close();
    }
    setDatabaseRegistrar(0);

    delete d_ptr;
}

QString PreferenceManager::application() const
{
    Q_D(const PreferenceManager);
    return d->m_application;
}

void PreferenceManager::setApplication(const QString& application)
{
    Q_D(PreferenceManager);
    d->m_application = application;
}

QString PreferenceManager::dataLocation() const
{
    Q_D(const PreferenceManager);
    return d->m_db->location();
}

void PreferenceManager::setDataLocation(const QString& dataLocation)
{
    Q_D(PreferenceManager);
    d->m_db->setLocation(dataLocation);
}

DatabaseRegistrar* PreferenceManager::databaseRegistrar() const
{
    Q_D(const PreferenceManager);
    return d->m_databaseRegistrar;
}

void PreferenceManager::setDatabaseRegistrar(DatabaseRegistrar* databaseRegistrar)
{
    Q_D(PreferenceManager);

    if (d->m_databaseRegistrar == databaseRegistrar)
        return;

    if (d->m_databaseRegistrar) {
        d->m_databaseRegistrar->unregisterDatabase(d->m_db.data());
    }

    d->m_databaseRegistrar = databaseRegistrar;
    if (d->m_databaseRegistrar) {
        d->m_databaseRegistrar->registerDatabase(d->m_db.data());
    }
}

bool PreferenceManager::isOpen() const
{
    Q_D(const PreferenceManager);
    return d->m_db->isOpen();
}

bool PreferenceManager::open()
{
    Q_D(PreferenceManager);

    if (isOpen()) {
        qDebug() << DEBUG_PREFIX << "Already opened";
        return true;
    }

    if (!d->validateDatabase()) {
        qCritical() << DEBUG_PREFIX << "Failed to open";
        return false;
    }

    if (!d->m_db->open()) {
        qCritical() << DEBUG_PREFIX << "Failed to open";
        return false;
    }

    qDebug() << DEBUG_PREFIX << "Opened successfully";
    Q_EMIT opened();

    return true;
}

void PreferenceManager::close()
{
    Q_D(const PreferenceManager);

    if (!isOpen()) {
        qDebug() << DEBUG_PREFIX << "Not open";
        return;
    }

    d->m_db->close();

    qDebug() << DEBUG_PREFIX << "Closed successfully";
    Q_EMIT closed();
}

Version PreferenceManager::version() const
{
    Q_D(const PreferenceManager);
    return Version(d->m_db->databaseProperty(d->versionProperty()).toString());
}

void PreferenceManager::setVersion(const Version& version)
{
    Q_D(const PreferenceManager);
    d->m_db->setDatabaseProperty(d->versionProperty(), version.toString());
}

bool PreferenceManager::isDefaultPreferenceSet() const
{
    Q_D(const PreferenceManager);
    return d->m_db->databaseProperty(d->defaultProperty()).toBool();
}

void PreferenceManager::setDefaultPreferenceSet(bool defaultPreferenceSet)
{
    Q_D(const PreferenceManager);
    d->m_db->setDatabaseProperty(d->defaultProperty(), defaultPreferenceSet);
}

void PreferenceManager::clearDefaultPreference()
{
    Q_D(const PreferenceManager);

    // XXX Will not emit preferenceChanged signal
    d->removeAllDefaultValues();
}

void PreferenceManager::clearUserPreference()
{
    Q_D(const PreferenceManager);

    // XXX Will not emit preferenceChanged signal
    d->removeAllUserValues();
}

} // namespace ns
