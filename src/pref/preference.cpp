#include <QtCore/QDebug>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include "preference.h"
#include "preferencemanager_p.h"
#include "preferencetransaction.h"
#include "abstractpreference_p.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Preference:"
#endif // DEBUG_PREFIX

namespace ns {

class PreferencePrivate : public AbstractPreferencePrivate
{
    Q_DECLARE_PUBLIC(Preference)

public:
    PreferencePrivate(PreferenceManager* manager, const QString& path);

    void _q_preferenceChanged(const QSet<QString>& paths, AbstractPreference* initiator);

private:
    void init();
};

PreferencePrivate::PreferencePrivate(PreferenceManager* manager, const QString& path) :
    AbstractPreferencePrivate(manager, path)
{
}

void PreferencePrivate::init()
{
    Q_Q(Preference);
    Q_ASSERT(q);

    QObject::connect(m_manager, SIGNAL(preferenceChanged(QSet<QString>, AbstractPreference*)),
        q, SLOT(_q_preferenceChanged(QSet<QString>, AbstractPreference*)));
}

void PreferencePrivate::_q_preferenceChanged(const QSet<QString>& paths, AbstractPreference* initiator)
{
    Q_Q(Preference);

    // Do not emit anything if is initiator
    if (initiator == q) {
        qDebug() << DEBUG_PREFIX << "Change ignored for initiator" << m_path;
        return;
    }

    QSet<QString> newPaths = q->translatePaths(paths);
    if (!newPaths.isEmpty()) {
        Q_EMIT q->changed(newPaths);
    }
}

Preference::Preference(PreferenceManager* manager, QObject* parent) :
    AbstractPreference(*new PreferencePrivate(manager, QString()), parent)
{
    Q_D(Preference);
    d->init();
}

Preference::Preference(PreferenceManager* manager, const QString& path, QObject* parent) :
    AbstractPreference(*new PreferencePrivate(manager, path), parent)
{
    Q_D(Preference);
    d->init();
}

Preference::Preference(const Preference* pref, QObject* parent) :
    AbstractPreference(*new PreferencePrivate(pref->manager(), pref->path()), parent)
{
    Q_D(Preference);
    d->init();
}

Preference::Preference(const Preference* pref, const QString& path, QObject* parent) :
    AbstractPreference(*new PreferencePrivate(pref->manager(), pref->fullPath(path)), parent)
{
    Q_D(Preference);
    d->init();
}

Preference::Preference(PreferencePrivate& dd, QObject* parent) :
    AbstractPreference(dd, parent)
{
    Q_D(Preference);
    d->init();
}

Preference::~Preference()
{
}

QStringList Preference::subPaths() const
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return QStringList();
    }

    QSet<QString> subPaths = QSet<QString>::fromList(managerd->userSubPaths(d->m_path));
    subPaths |= QSet<QString>::fromList(managerd->defaultSubPaths(d->m_path));

    return translatePaths(subPaths).toList();
}

void Preference::reset(const QString& path)
{
    resetValue(path);
}

void Preference::resetAll()
{
    resetAllValues();
}

QSet<QString> Preference::translatePaths(const QSet<QString>& paths) const
{
    Q_D(const Preference);
    return translatePaths(paths, d->m_path);
}

QSet<QString> Preference::translatePaths(const QSet<QString>& paths, const QString& basePath)
{
    if (basePath.isEmpty())
        return paths;

    QSet<QString> ret;

    QString prefix = basePath;
    prefix += QLatin1Char('.');

    foreach (const QString& path, paths) {
        if (path == basePath) {
            ret << QString();
        } else if (path.startsWith(prefix)) {
            ret << path.mid(prefix.length());
        }
    }

    return ret;
}

bool Preference::hasValue(const QString& path) const
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return false;
    }

    const QString fpath = fullPath(path);
    return managerd->hasUserValue(fpath) || managerd->hasDefaultValue(fpath);
}

QVariant Preference::getValue(const QString& path) const
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return QVariant();
    }

    const QString fpath = fullPath(path);

    QVariant value = managerd->getUserValue(fpath);
    if (value.isNull()) {
        value = managerd->getDefaultValue(fpath);
    }
    return value;
}

void Preference::putValue(const QString& path, const QVariant& value)
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return;
    }

    PreferenceTransaction trans(this);

    const QString fpath = fullPath(path);
    QVariant oldValue = managerd->getUserValue(fpath);
    if (oldValue.isNull()) {
        oldValue = managerd->getDefaultValue(fpath);
    }
    if (value != oldValue) {
        managerd->putUserValue(fpath, value);
        trans.addPreferenceChange(fpath);
    }

    trans.commit();
}

void Preference::resetValue(const QString& path)
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return;
    }

    PreferenceTransaction trans(this);

    const QString fpath = fullPath(path);
    if (managerd->hasUserValue(fpath)) {
        managerd->removeUserValue(fpath);
        trans.addPreferenceChange(fpath);
    }

    trans.commit();
}

void Preference::resetAllValues()
{
    Q_D(const Preference);

    PreferenceManagerPrivate* managerd = d->managerd();
    if (!managerd) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return;
    }

    PreferenceTransaction trans(this);

    const QStringList paths = managerd->removeAllUserValues(d->m_path);

    trans.addPreferenceChange(paths);
    trans.commit();
}

} // namespace ns

#include "moc_preference.cpp"
