// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_DEFAULTPREFERENCEDAO_P_H
#define NS_PREF_DEFAULTPREFERENCEDAO_P_H

#include "abstractentitydao.h"
#include "preferencefwd.h"
#include "defaultpreference.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Pref {

class DefaultPreferenceDaoBase : public AbstractEntityDao
{
public:
    typedef DefaultPreference Entity;
    typedef DefaultPreferenceList EntityList;

    explicit DefaultPreferenceDaoBase(Database* db = 0);
    virtual ~DefaultPreferenceDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(const QString& application, const QString& path);
    virtual int countByExample(const DefaultPreference& entity);
    virtual int countAll();

    virtual DefaultPreference find(const QString& application, const QString& path);
    virtual DefaultPreference findByExample(const DefaultPreference& entity);
    virtual DefaultPreferenceList findAll();
    virtual DefaultPreferenceList findAllByExample(const DefaultPreference& entity);

    virtual bool persist(DefaultPreference& entity);
    virtual bool merge(const DefaultPreference& entity);
    virtual bool update(const DefaultPreference& entity);
    virtual bool remove(const DefaultPreference& entity);
    virtual bool removeAll();
    virtual bool refresh(DefaultPreference& entity);

protected:
    virtual QString sqlAddSort(const QString& sql) const;

private:
    Q_DISABLE_COPY(DefaultPreferenceDaoBase)
};

//$BEGIN-CUSTOM$
class DefaultPreferenceDao : public DefaultPreferenceDaoBase
{
public:
    explicit DefaultPreferenceDao(Database* db = 0);
    virtual ~DefaultPreferenceDao();

    virtual bool createTable();
    virtual bool dropTable();

    QList<QString> findAllPathsByApplication(const QString& application);
    QList<QString> findAllPathsByPathPrefix(const QString& application, const QString& prefix);

    DefaultPreferenceList findAllByApplication(const QString& application);
    DefaultPreferenceList findAllByPathPrefix(const QString& application, const QString& prefix);

    bool removeAllByApplication(const QString& application);
    bool removeAllByPathPrefix(const QString& application, const QString& prefix);

private:
    Q_DISABLE_COPY(DefaultPreferenceDao)
};
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_DEFAULTPREFERENCEDAO_P_H
