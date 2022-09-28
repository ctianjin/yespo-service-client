#ifndef QPID_PREFERENCESUPPORT_P_H
#define QPID_PREFERENCESUPPORT_P_H

#include <QtCore/QObject>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace ns {

class PreferenceSupportImpl : public QObject
{
    Q_OBJECT

public:
    PreferenceSupport* q;
    Preference* m_pref;
    QTimer* m_timer;

    PreferenceSupportImpl();
    virtual ~PreferenceSupportImpl();

    QTimer* timer();

private Q_SLOTS:
    void slotPreferenceChanged(const QSet<QString>& paths);
    void slotSavePreference();
};

} // namespace ns

#endif // QPID_PREFERENCESUPPORT_P_H
