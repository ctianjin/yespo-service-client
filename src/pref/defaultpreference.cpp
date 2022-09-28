#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include "defaultpreference.h"
#include "preferencemanager_p.h"
#include "abstractpreference_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "DefaultPreference:"
#endif // DEBUG_PREFIX

namespace ns {

class DefaultPreferencePrivate : public AbstractPreferencePrivate
{
    Q_DECLARE_PUBLIC(DefaultPreference)

public:
    DefaultPreferencePrivate(PreferenceManager* manager, const QString& path);
};

DefaultPreferencePrivate::DefaultPreferencePrivate(PreferenceManager* manager, const QString& path) :
    AbstractPreferencePrivate(manager, path)
{
}

DefaultPreference::DefaultPreference(PreferenceManager* manager, QObject* parent) :
    AbstractPreference(*new DefaultPreferencePrivate(manager, QString()), parent)
{
}

DefaultPreference::DefaultPreference(PreferenceManager* manager, const QString& path, QObject* parent) :
    AbstractPreference(*new DefaultPreferencePrivate(manager, path), parent)
{
}

DefaultPreference::DefaultPreference(const DefaultPreference* pref, QObject* parent) :
    AbstractPreference(*new DefaultPreferencePrivate(pref->manager(), pref->path()), parent)
{
}

DefaultPreference::DefaultPreference(const DefaultPreference* pref, const QString& path, QObject* parent) :
    AbstractPreference(*new DefaultPreferencePrivate(pref->manager(), pref->fullPath(path)), parent)
{
}

DefaultPreference::DefaultPreference(DefaultPreferencePrivate& dd, QObject* parent) :
    AbstractPreference(dd, parent)
{
}

DefaultPreference::~DefaultPreference()
{
}

bool DefaultPreference::hasValue(const QString& path) const
{
    Q_D(const DefaultPreference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return false;
    }

    const QString fpath = fullPath(path);
    return managerd->hasDefaultValue(fpath);
}

QVariant DefaultPreference::getValue(const QString& path) const
{
    Q_D(const DefaultPreference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return QVariant();
    }

    const QString fpath = fullPath(path);
    return managerd->getDefaultValue(fpath);
}

void DefaultPreference::putValue(const QString& path, const QVariant& value)
{
    Q_D(const DefaultPreference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return;
    }

    const QString fpath = fullPath(path);
    managerd->putDefaultValue(fpath, value);
}

void DefaultPreference::resetValue(const QString& path)
{
    Q_D(const DefaultPreference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return;
    }

    const QString fpath = fullPath(path);
    managerd->removeDefaultValue(fpath);
}

} // namespace ns
