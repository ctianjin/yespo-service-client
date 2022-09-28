#ifndef QPID_ABSTRACTPREFERENCE_P_H
#define QPID_ABSTRACTPREFERENCE_P_H

#include <QtCore/QPointer>
#include <QtCore/QString>
#include "abstractpreference.h"
#include "preferencemanager.h"

namespace ns {

class PreferenceManagerPrivate;

class AbstractPreferencePrivate
{
    Q_DECLARE_PUBLIC(AbstractPreference)

public:
    AbstractPreference* q_ptr;

    QPointer<PreferenceManager> m_manager;
    QString m_path;

    AbstractPreferencePrivate(PreferenceManager* manager, const QString& path);
    virtual ~AbstractPreferencePrivate();

    PreferenceManagerPrivate* managerd() const;
};

inline PreferenceManagerPrivate* AbstractPreferencePrivate::managerd() const
{
    return m_manager ? m_manager->d_func() : 0;
}

} // namespace ns

#endif // QPID_ABSTRACTPREFERENCE_P_H
