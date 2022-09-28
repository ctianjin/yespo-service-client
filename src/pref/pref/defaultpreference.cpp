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
#include "defaultpreference.h"
#include "defaultpreference_p.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Pref {

DefaultPreferencePrivate::DefaultPreferencePrivate() :
    QSharedData()
{
}

DefaultPreferencePrivate::DefaultPreferencePrivate(const DefaultPreferencePrivate& rhs) :
    QSharedData(rhs),
    m_application(rhs.m_application),
    m_path(rhs.m_path),
    m_value(rhs.m_value)
{
}

DefaultPreferencePrivate::DefaultPreferencePrivate(const QString& application, const QString& path) :
    QSharedData(),
    m_application(application),
    m_path(path)
{
}

DefaultPreferencePrivate::DefaultPreferencePrivate(const QSqlQuery& query) :
    QSharedData(),
    m_application(query.value(0)),
    m_path(query.value(1)),
    m_value(query.value(2))
{
}

DefaultPreferencePrivate::DefaultPreferencePrivate(const QSqlRecord& record) :
    QSharedData(),
    m_application(record.value(QLatin1String("app"))),
    m_path(record.value(QLatin1String("path"))),
    m_value(record.value(QLatin1String("value")))
{
}

DefaultPreferencePrivate::~DefaultPreferencePrivate()
{
}

DefaultPreference::DefaultPreference() :
    d(new DefaultPreferencePrivate)
{
}

DefaultPreference::DefaultPreference(const DefaultPreference& rhs) :
    d(rhs.d)
{
}

DefaultPreference::DefaultPreference(const QString& application, const QString& path) :
    d(new DefaultPreferencePrivate(application, path))
{
}

DefaultPreference::DefaultPreference(const QSqlQuery& query) :
    d(new DefaultPreferencePrivate(query))
{
}

DefaultPreference::DefaultPreference(const QSqlRecord& record) :
    d(new DefaultPreferencePrivate(record))
{
}

DefaultPreference::~DefaultPreference()
{
}

DefaultPreference& DefaultPreference::operator=(const DefaultPreference& rhs)
{
    d = rhs.d;
    return *this;
}

bool DefaultPreference::isValid() const
{
    return d->m_application.isValid() && d->m_path.isValid();
}

DefaultPreference DefaultPreference::identity() const
{
    DefaultPreference entity;
    entity.d->m_application = d->m_application;
    entity.d->m_path = d->m_path;
    return entity;
}

bool DefaultPreference::identityEqual(const DefaultPreference& rhs) const
{
    return d->m_application == rhs.d->m_application && d->m_path == rhs.d->m_path;
}

bool DefaultPreference::isApplicationNull() const
{
    return d->m_application.isNull();
}

QString DefaultPreference::application() const
{
    return d->m_application.toString();
}

void DefaultPreference::setApplicationNull()
{
    d->m_application = QVariant(QVariant::String);
}

void DefaultPreference::setApplication(const QString& application)
{
    d->m_application = application;
}

bool DefaultPreference::isPathNull() const
{
    return d->m_path.isNull();
}

QString DefaultPreference::path() const
{
    return d->m_path.toString();
}

void DefaultPreference::setPathNull()
{
    d->m_path = QVariant(QVariant::String);
}

void DefaultPreference::setPath(const QString& path)
{
    d->m_path = path;
}

bool DefaultPreference::isValueNull() const
{
    return d->m_value.isNull();
}

QVariant DefaultPreference::value() const
{
    return d->m_value;
}

void DefaultPreference::setValueNull()
{
    d->m_value = QVariant(QVariant::String);
}

void DefaultPreference::setValue(const QVariant& value)
{
    d->m_value = value;
}

//$BEGIN-CUSTOM$
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns
