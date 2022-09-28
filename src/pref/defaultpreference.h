#ifndef DEFAULTPREFERENCE_H
#define DEFAULTPREFERENCE_H

#include "abstractpreference.h"
#include "prefglobal.h"

namespace ns {

class DefaultPreferencePrivate;

class BUILDSYS_PREF_DECL DefaultPreference : public AbstractPreference
{
    Q_OBJECT

public:
    explicit DefaultPreference(PreferenceManager* manager, QObject* parent = 0);
    DefaultPreference(PreferenceManager* manager, const QString& path, QObject* parent = 0);
    explicit DefaultPreference(const DefaultPreference* pref, QObject* parent = 0);
    DefaultPreference(const DefaultPreference* pref, const QString& path, QObject* parent = 0);
    virtual ~DefaultPreference();

protected:
    DefaultPreference(DefaultPreferencePrivate& dd, QObject* parent = 0);

    virtual bool hasValue(const QString& path) const;
    virtual QVariant getValue(const QString& path) const;
    virtual void putValue(const QString& path, const QVariant& value);
    virtual void resetValue(const QString& path);

private:
    Q_DECLARE_PRIVATE(DefaultPreference)
    Q_DISABLE_COPY(DefaultPreference)
};

} // namespace ns

#endif // DEFAULTPREFERENCE_H
