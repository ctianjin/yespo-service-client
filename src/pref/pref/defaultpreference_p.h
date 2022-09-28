// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_DEFAULTPREFERENCE_P_H
#define NS_PREF_DEFAULTPREFERENCE_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace Pref {

class DefaultPreferencePrivate : public QSharedData
{
public:
    QVariant m_application;
    QVariant m_path;
    QVariant m_value;

    DefaultPreferencePrivate();
    DefaultPreferencePrivate(const DefaultPreferencePrivate& rhs);
    DefaultPreferencePrivate(const QString& application, const QString& path);
    explicit DefaultPreferencePrivate(const QSqlQuery& query);
    explicit DefaultPreferencePrivate(const QSqlRecord& record);
    ~DefaultPreferencePrivate();
};

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_DEFAULTPREFERENCE_P_H
