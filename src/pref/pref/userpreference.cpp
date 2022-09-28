// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "userpreference.h"
#include "userpreference_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Pref {

UserPreferencePrivate::UserPreferencePrivate() :
    QSharedData()
{
}

UserPreferencePrivate::UserPreferencePrivate(const UserPreferencePrivate& rhs) :
    QSharedData(rhs),
    m_application(rhs.m_application),
    m_path(rhs.m_path),
    m_value(rhs.m_value)
{
}

UserPreferencePrivate::UserPreferencePrivate(const QString& application, const QString& path) :
    QSharedData(),
    m_application(application),
    m_path(path)
{
}

UserPreferencePrivate::UserPreferencePrivate(const QSqlQuery& query) :
    QSharedData(),
    m_application(query.value(0)),
    m_path(query.value(1)),
    m_value(query.value(2))
{
}

UserPreferencePrivate::UserPreferencePrivate(const QSqlRecord& record) :
    QSharedData(),
    m_application(record.value(QLatin1String("app"))),
    m_path(record.value(QLatin1String("path"))),
    m_value(record.value(QLatin1String("value")))
{
}

UserPreferencePrivate::~UserPreferencePrivate()
{
}

UserPreference::UserPreference() :
    d(new UserPreferencePrivate)
{
}

UserPreference::UserPreference(const UserPreference& rhs) :
    d(rhs.d)
{
}

UserPreference::UserPreference(const QString& application, const QString& path) :
    d(new UserPreferencePrivate(application, path))
{
}

UserPreference::UserPreference(const QSqlQuery& query) :
    d(new UserPreferencePrivate(query))
{
}

UserPreference::UserPreference(const QSqlRecord& record) :
    d(new UserPreferencePrivate(record))
{
}

UserPreference::~UserPreference()
{
}

UserPreference& UserPreference::operator=(const UserPreference& rhs)
{
    d = rhs.d;
    return *this;
}

bool UserPreference::isValid() const
{
    return d->m_application.isValid() && d->m_path.isValid();
}

UserPreference UserPreference::identity() const
{
    UserPreference entity;
    entity.d->m_application = d->m_application;
    entity.d->m_path = d->m_path;
    return entity;
}

bool UserPreference::identityEqual(const UserPreference& rhs) const
{
    return d->m_application == rhs.d->m_application && d->m_path == rhs.d->m_path;
}

bool UserPreference::isApplicationNull() const
{
    return d->m_application.isNull();
}

QString UserPreference::application() const
{
    return d->m_application.toString();
}

void UserPreference::setApplicationNull()
{
    d->m_application = QVariant(QVariant::String);
}

void UserPreference::setApplication(const QString& application)
{
    d->m_application = application;
}

bool UserPreference::isPathNull() const
{
    return d->m_path.isNull();
}

QString UserPreference::path() const
{
    return d->m_path.toString();
}

void UserPreference::setPathNull()
{
    d->m_path = QVariant(QVariant::String);
}

void UserPreference::setPath(const QString& path)
{
    d->m_path = path;
}

bool UserPreference::isValueNull() const
{
    return d->m_value.isNull();
}

QVariant UserPreference::value() const
{
    return d->m_value;
}

void UserPreference::setValueNull()
{
    d->m_value = QVariant(QVariant::String);
}

void UserPreference::setValue(const QVariant& value)
{
    d->m_value = value;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns
