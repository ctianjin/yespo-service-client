#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include "preferencesupport.h"
#include "preferencesupport_p.h"
#include "preference.h"

namespace ns {

PreferenceSupportImpl::PreferenceSupportImpl() :
    q(0),
    m_pref(0),
    m_timer(0)
{
}

PreferenceSupportImpl::~PreferenceSupportImpl()
{
}

QTimer* PreferenceSupportImpl::timer()
{
    if (!m_timer) {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(slotSavePreference()));
    }

    return m_timer;
}

void PreferenceSupportImpl::slotPreferenceChanged(const QSet<QString>& paths)
{
    if (m_pref && m_pref == sender()) {
        q->applyPreference(paths, false);
    }
}

void PreferenceSupportImpl::slotSavePreference()
{
    if (m_pref) {
        q->savePreference();
    }
}

PreferenceSupport::PreferenceSupport() :
    m_impl(new PreferenceSupportImpl)
{
    m_impl->q = this;
}

PreferenceSupport::~PreferenceSupport()
{
    delete m_impl;
}

Preference* PreferenceSupport::preference() const
{
    return m_impl->m_pref;
}

void PreferenceSupport::setPreference(Preference* pref)
{
    if (m_impl->m_pref == pref)
        return;

    if (m_impl->m_pref) {
        QObject::disconnect(m_impl->m_pref, SIGNAL(changed(QSet<QString>)),
            m_impl, SLOT(slotPreferenceChanged(QSet<QString>)));
        m_impl->m_pref->deleteLater();
        m_impl->m_pref = 0;
    }

    m_impl->m_pref = pref;
    if (m_impl->m_pref) {
        m_impl->m_pref->setParent(m_impl);
        QObject::connect(m_impl->m_pref, SIGNAL(changed(QSet<QString>)),
            m_impl, SLOT(slotPreferenceChanged(QSet<QString>)));

        const QSet<QString> paths;
        applyPreference(paths, true);
    }
}

void PreferenceSupport::scheduleSavePreference(int msec)
{
    m_impl->timer()->start(msec);
}

void PreferenceSupport::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_UNUSED(paths);
    Q_UNUSED(force);

    // Noop
}

void PreferenceSupport::savePreference()
{
    // Noop
}

} // namespace ns
