#ifndef PREFERENCESUPPORT_H
#define PREFERENCESUPPORT_H

#include <QtCore/QSet>
#include <QtCore/QString>
#include "preference.h"
#include "preferencetransaction.h"
#include "prefglobal.h"

namespace ns {

class PreferenceSupportImpl;

class BUILDSYS_PREF_DECL PreferenceSupport
{
public:
    PreferenceSupport();
    virtual ~PreferenceSupport();

    Preference* preference() const;
    virtual void setPreference(Preference* pref); // XXX Takes ownership

protected:
    void scheduleSavePreference(int msec = 500);

    virtual void applyPreference(const QSet<QString>& paths, bool force);
    virtual void savePreference();

private:
    friend class PreferenceSupportImpl;
    PreferenceSupportImpl* const m_impl;

    Q_DISABLE_COPY(PreferenceSupport)
};

} // namespace ns

#endif // PREFERENCESUPPORT_H
