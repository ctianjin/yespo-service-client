#ifndef PREFERENCE_H
#define PREFERENCE_H

#include "abstractpreference.h"
#include "prefglobal.h"

QT_BEGIN_NAMESPACE
template <typename T> class QSet;
QT_END_NAMESPACE

namespace ns {

class PreferencePrivate;

class BUILDSYS_PREF_DECL Preference : public AbstractPreference
{
    Q_OBJECT

public:
    explicit Preference(PreferenceManager* manager, QObject* parent = 0);
    Preference(PreferenceManager* manager, const QString& path, QObject* parent = 0);
    explicit Preference(const Preference* pref, QObject* parent = 0);
    Preference(const Preference* pref, const QString& path, QObject* parent = 0);
    virtual ~Preference();

    QStringList subPaths() const;

    void reset(const QString& path);
    void resetAll();

    QSet<QString> translatePaths(const QSet<QString>& paths) const;

    static QSet<QString> translatePaths(const QSet<QString>& paths, const QString& basePath);

Q_SIGNALS:
    void changed(const QSet<QString>& paths);

protected:
    Preference(PreferencePrivate& dd, QObject* parent = 0);

    virtual bool hasValue(const QString& path) const;
    virtual QVariant getValue(const QString& path) const;
    virtual void putValue(const QString& path, const QVariant& value);
    virtual void resetValue(const QString& path);
    virtual void resetAllValues();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_preferenceChanged(const QSet<QString>&, AbstractPreference*))

    Q_DECLARE_PRIVATE(Preference)
    Q_DISABLE_COPY(Preference)
};

} // namespace ns

#endif // PREFERENCE_H
