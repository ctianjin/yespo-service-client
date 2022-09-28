#ifndef QPID_PREFERENCETRANSACTION_P_H
#define QPID_PREFERENCETRANSACTION_P_H

#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtCore/QString>
#include "transaction.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

namespace ns {

class PreferenceManager;
class AbstractPreference;
class Database;

class PreferenceTransactionContext
{
public:
    PreferenceTransactionContext(
        PreferenceManager* manager, AbstractPreference* initiator, Database* db);
    ~PreferenceTransactionContext();

    bool commit();
    bool rollback();

    void addPreferenceChange(const QString& path);
    void addPreferenceChange(const QStringList& paths);

private:
    PreferenceManager* m_manager; // Life time is within a preference manager
    QPointer<AbstractPreference> m_initiator;
    Transaction m_transaction;
    QSet<QString> m_changedPaths;
};

} // namespace ns

#endif // QPID_PREFERENCETRANSACTION_P_H
