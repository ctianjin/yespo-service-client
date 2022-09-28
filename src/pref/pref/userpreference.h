// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_USERPREFERENCE_H
#define NS_PREF_USERPREFERENCE_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
class QString;
class QVariant;
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {
namespace Pref {

class UserPreference;
class UserPreferencePrivate;
typedef QList<UserPreference> UserPreferenceList;

class UserPreference
{
public:
    UserPreference();
    UserPreference(const UserPreference& rhs);
    UserPreference(const QString& application, const QString& path);
    explicit UserPreference(const QSqlQuery& query);
    explicit UserPreference(const QSqlRecord& record);
    ~UserPreference();

    UserPreference& operator=(const UserPreference& rhs);

    bool isValid() const;
    UserPreference identity() const;
    bool identityEqual(const UserPreference& rhs) const;

    bool isApplicationNull() const;
    QString application() const;
    void setApplicationNull();
    void setApplication(const QString& application);

    bool isPathNull() const;
    QString path() const;
    void setPathNull();
    void setPath(const QString& path);

    bool isValueNull() const;
    QVariant value() const;
    void setValueNull();
    void setValue(const QVariant& value);

    //$BEGIN-CUSTOM$
    //$END-CUSTOM$

private:
    friend class UserPreferenceDaoBase;
    friend class UserPreferenceDao;

    QSharedDataPointer<UserPreferencePrivate> d;
};

} // namespace ns::Pref
} // namespace ns

Q_DECLARE_METATYPE(ns::Pref::UserPreference)
Q_DECLARE_METATYPE(ns::Pref::UserPreferenceList)

#endif // NS_PREF_USERPREFERENCE_H
