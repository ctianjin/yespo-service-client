#ifndef QPID_PREFERENCEMANAGER_P_H
#define QPID_PREFERENCEMANAGER_P_H

#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include "preferencemanager.h"
#include "pref/preferencefwd.h"

QT_BEGIN_NAMESPACE
class QStringList;
class QVariant;
QT_END_NAMESPACE

namespace ns {

class PreferenceTransactionContext;

class PreferenceManagerPrivate
{
    Q_DECLARE_PUBLIC(PreferenceManager)

public:
    static const QString defaultApplication;
    static const QString versionPropertyFormat;
    static const QString defaultPropertyFormat;

    PreferenceManager* q_ptr;

    QString m_application;
    QPointer<DatabaseRegistrar> m_databaseRegistrar;
    QScopedPointer<Pref::PreferenceDatabase> m_db;
    QScopedPointer<Pref::DefaultPreferenceDao> m_defaultDao;
    QScopedPointer<Pref::UserPreferenceDao> m_userDao;

    explicit PreferenceManagerPrivate(const QString& application = QString());
    virtual ~PreferenceManagerPrivate();

    bool validateDatabase();

    QString versionProperty() const;
    QString defaultProperty() const;

    QStringList defaultSubPaths(const QString& path) const;
    QStringList userSubPaths(const QString& path) const;

    bool hasDefaultValue(const QString& path) const;
    bool hasUserValue(const QString& path) const;

    QVariant getDefaultValue(const QString& path) const;
    QVariant getUserValue(const QString& path) const;

    void putDefaultValue(const QString& path, const QVariant& value) const;
    void putUserValue(const QString& path, const QVariant& value) const;

    void removeDefaultValue(const QString& path) const;
    void removeUserValue(const QString& path) const;

    void removeAllDefaultValues() const;
    void removeAllUserValues() const;
    QStringList removeAllUserValues(const QString& path) const;

    PreferenceTransactionContext* transaction() const;
    PreferenceTransactionContext* newTransaction(AbstractPreference* initiator);
    void clearTransaction();

private:
    QScopedPointer<PreferenceTransactionContext> m_transaction;
};

inline PreferenceTransactionContext* PreferenceManagerPrivate::transaction() const
{
    return m_transaction.data();
}

} // namespace ns

#endif // QPID_PREFERENCEMANAGER_P_H
