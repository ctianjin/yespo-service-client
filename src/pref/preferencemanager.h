#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include "prefglobal.h"

QT_BEGIN_NAMESPACE
template <typename T> class QSet;
QT_END_NAMESPACE

namespace ns {

class PreferenceManagerPrivate;
class AbstractPreference;
class DatabaseRegistrar;
class Version;

class BUILDSYS_PREF_DECL PreferenceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString application READ application WRITE setApplication)
    Q_PROPERTY(QString dataLocation READ dataLocation WRITE setDataLocation)
    Q_PROPERTY(DatabaseRegistrar* databaseRegistrar READ databaseRegistrar WRITE setDatabaseRegistrar)

public:
    explicit PreferenceManager(QObject* parent = 0);
    explicit PreferenceManager(const QString& application, QObject* parent = 0);
    virtual ~PreferenceManager();

    QString application() const;
    void setApplication(const QString& application);

    QString dataLocation() const;
    void setDataLocation(const QString& dataLocation);

    DatabaseRegistrar* databaseRegistrar() const;
    void setDatabaseRegistrar(DatabaseRegistrar* databaseRegistrar);

    bool isOpen() const;
    bool open();
    void close();

    Version version() const;
    void setVersion(const Version& version);

    bool isDefaultPreferenceSet() const;
    void setDefaultPreferenceSet(bool defaultPreferenceSet);

    void clearDefaultPreference();
    void clearUserPreference();

Q_SIGNALS:
    void opened();
    void closed();
    void preferenceChanged(const QSet<QString>& paths, AbstractPreference* initiator);

private:
    friend class PreferenceTransaction;
    friend class PreferenceTransactionContext;
    friend class AbstractPreferencePrivate;

    PreferenceManagerPrivate* const d_ptr;

    Q_DECLARE_PRIVATE(PreferenceManager)
    Q_DISABLE_COPY(PreferenceManager)
};

} // namespace ns

#endif // PREFERENCEMANAGER_H
