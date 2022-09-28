// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_USERPREFERENCE_P_H
#define NS_PREF_USERPREFERENCE_P_H

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {
namespace Pref {

class UserPreferencePrivate : public QSharedData
{
public:
    QVariant m_application;
    QVariant m_path;
    QVariant m_value;

    UserPreferencePrivate();
    UserPreferencePrivate(const UserPreferencePrivate& rhs);
    UserPreferencePrivate(const QString& application, const QString& path);
    explicit UserPreferencePrivate(const QSqlQuery& query);
    explicit UserPreferencePrivate(const QSqlRecord& record);
    ~UserPreferencePrivate();
};

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_USERPREFERENCE_P_H
