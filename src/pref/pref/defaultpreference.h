// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_DEFAULTPREFERENCE_H
#define NS_PREF_DEFAULTPREFERENCE_H

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

class DefaultPreference;
class DefaultPreferencePrivate;
typedef QList<DefaultPreference> DefaultPreferenceList;

class DefaultPreference
{
public:
    DefaultPreference();
    DefaultPreference(const DefaultPreference& rhs);
    DefaultPreference(const QString& application, const QString& path);
    explicit DefaultPreference(const QSqlQuery& query);
    explicit DefaultPreference(const QSqlRecord& record);
    ~DefaultPreference();

    DefaultPreference& operator=(const DefaultPreference& rhs);

    bool isValid() const;
    DefaultPreference identity() const;
    bool identityEqual(const DefaultPreference& rhs) const;

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
    friend class DefaultPreferenceDaoBase;
    friend class DefaultPreferenceDao;

    QSharedDataPointer<DefaultPreferencePrivate> d;
};

} // namespace ns::Pref
} // namespace ns

Q_DECLARE_METATYPE(ns::Pref::DefaultPreference)
Q_DECLARE_METATYPE(ns::Pref::DefaultPreferenceList)

#endif // NS_PREF_DEFAULTPREFERENCE_H
