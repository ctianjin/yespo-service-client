#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include "preferencetransaction.h"
#include "preferencetransaction_p.h"
#include "preferencemanager.h"
#include "preferencemanager_p.h"
#include "abstractpreference.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PreferenceTransaction:"
#endif // DEBUG_PREFIX

namespace ns {

PreferenceTransactionContext::PreferenceTransactionContext(
    PreferenceManager* manager, AbstractPreference* initiator, Database* db) :
    m_manager(manager),
    m_initiator(initiator),
    m_transaction(db)
{
    Q_ASSERT(m_manager);
}

PreferenceTransactionContext::~PreferenceTransactionContext()
{
}

bool PreferenceTransactionContext::commit()
{
    qDebug() << DEBUG_PREFIX << "Commit";

    bool ret = m_transaction.commit();

    if (ret) {
        if (!m_changedPaths.isEmpty()) {
            Q_EMIT m_manager->preferenceChanged(m_changedPaths, m_initiator);
        }
        qDebug() << DEBUG_PREFIX << "Commit succeeded" << m_changedPaths;
    } else {
        qDebug() << DEBUG_PREFIX << "Commit failed" << m_changedPaths;
    }
    m_changedPaths.clear();

    return ret;
}

bool PreferenceTransactionContext::rollback()
{
    qDebug() << DEBUG_PREFIX << "Rollback";

    bool ret = m_transaction.rollback();

    if (ret) {
        qDebug() << DEBUG_PREFIX << "Rollback succeeded";
    } else {
        qDebug() << DEBUG_PREFIX << "Rollback failed";
    }
    m_changedPaths.clear();

    return ret;
}

void PreferenceTransactionContext::addPreferenceChange(const QString& path)
{
    m_changedPaths << path;

    // Also add parent paths
    const QChar dot = QLatin1Char('.');
    QString p = path;

    forever {
        int n = p.lastIndexOf(dot);
        if (n < 0)
            break;
        p = p.left(n);
        m_changedPaths << p;
    }
}

void PreferenceTransactionContext::addPreferenceChange(const QStringList& paths)
{
    foreach (const QString& path, paths) {
        addPreferenceChange(path);
    }
}

PreferenceTransaction::PreferenceTransaction(PreferenceManager* manager) :
    m_manager(manager),
    m_owner(false)
{
    init(0);
}

PreferenceTransaction::PreferenceTransaction(AbstractPreference* pref) :
    m_manager(pref ? pref->manager() : 0),
    m_owner(false)
{
    init(pref);
}

PreferenceTransaction::~PreferenceTransaction()
{
    if (m_owner && m_manager) {
        m_manager->d_func()->clearTransaction();
    }
}

void PreferenceTransaction::init(AbstractPreference* initiator)
{
    if (m_manager) {
        PreferenceManagerPrivate* managerd = m_manager->d_func();
        if (!managerd->transaction()) {
            managerd->newTransaction(initiator);
            m_owner = true;
        }
    }
}

bool PreferenceTransaction::isOwner() const
{
    return m_owner;
}

bool PreferenceTransaction::commit()
{
    if (m_owner && m_manager) {
        PreferenceTransactionContext* context = m_manager->d_func()->transaction();
        if (context) {
            return context->commit();
        }
    }
    return true;
}

bool PreferenceTransaction::rollback()
{
    if (m_owner && m_manager) {
        PreferenceTransactionContext* context = m_manager->d_func()->transaction();
        if (context) {
            return context->rollback();
        }
    }
    return true;
}

void PreferenceTransaction::addPreferenceChange(const QString& path)
{
    if (m_manager) {
        PreferenceTransactionContext* context = m_manager->d_func()->transaction();
        if (context) {
            context->addPreferenceChange(path);
        }
    }
}

void PreferenceTransaction::addPreferenceChange(const QStringList& paths)
{
    if (m_manager) {
        PreferenceTransactionContext* context = m_manager->d_func()->transaction();
        if (context) {
            context->addPreferenceChange(paths);
        }
    }
}

} // namespace ns
