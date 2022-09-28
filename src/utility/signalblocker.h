#ifndef SIGNALBLOCKER_H
#define SIGNALBLOCKER_H

#include <QtCore/QObject>
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL SignalBlocker
{
public:
    explicit SignalBlocker(QObject* obj);
    ~SignalBlocker();

private:
    QObject* m_obj;
    bool m_blocked;

    Q_DISABLE_COPY(SignalBlocker)
};

inline SignalBlocker::SignalBlocker(QObject* obj) :
    m_obj(obj),
    m_blocked(obj->signalsBlocked())
{
    m_obj->blockSignals(true);
}

inline SignalBlocker::~SignalBlocker()
{
    m_obj->blockSignals(m_blocked);
}

} // namespace ns

#endif // SIGNALBLOCKER_H
