#ifndef PREFERENCETRANSACTION_H
#define PREFERENCETRANSACTION_H

#include <QtCore/QPointer>
#include "prefglobal.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
QT_END_NAMESPACE

namespace ns {

class PreferenceManager;
class AbstractPreference;

class BUILDSYS_PREF_DECL PreferenceTransaction
{
public:
    explicit PreferenceTransaction(PreferenceManager* manager);
    explicit PreferenceTransaction(AbstractPreference* pref);
    ~PreferenceTransaction();

    bool isOwner() const;

    bool commit();
    bool rollback();

    void addPreferenceChange(const QString& path);
    void addPreferenceChange(const QStringList& paths);

private:
    void init(AbstractPreference* initiator);

    QPointer<PreferenceManager> m_manager;
    bool m_owner;

    Q_DISABLE_COPY(PreferenceTransaction)
};

} // namespace ns

#endif // PREFERENCETRANSACTION_H
