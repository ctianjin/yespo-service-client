// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_USERPREFERENCEDAO_P_H
#define NS_PREF_USERPREFERENCEDAO_P_H

#include "abstractentitydao.h"
#include "preferencefwd.h"
#include "userpreference.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Pref {

class UserPreferenceDaoBase : public AbstractEntityDao
{
public:
    typedef UserPreference Entity;
    typedef UserPreferenceList EntityList;

    explicit UserPreferenceDaoBase(Database* db = 0);
    virtual ~UserPreferenceDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(const QString& application, const QString& path);
    virtual int countByExample(const UserPreference& entity);
    virtual int countAll();

    virtual UserPreference find(const QString& application, const QString& path);
    virtual UserPreference findByExample(const UserPreference& entity);
    virtual UserPreferenceList findAll();
    virtual UserPreferenceList findAllByExample(const UserPreference& entity);

    virtual bool persist(UserPreference& entity);
    virtual bool merge(const UserPreference& entity);
    virtual bool update(const UserPreference& entity);
    virtual bool remove(const UserPreference& entity);
    virtual bool removeAll();
    virtual bool refresh(UserPreference& entity);

protected:
    virtual QString sqlAddSort(const QString& sql) const;

private:
    Q_DISABLE_COPY(UserPreferenceDaoBase)
};

//$BEGIN-CUSTOM$
class UserPreferenceDao : public UserPreferenceDaoBase
{
public:
    explicit UserPreferenceDao(Database* db = 0);
    virtual ~UserPreferenceDao();

    virtual bool createTable();
    virtual bool dropTable();

    QList<QString> findAllPathsByApplication(const QString& application);
    QList<QString> findAllPathsByPathPrefix(const QString& application, const QString& prefix);

    UserPreferenceList findAllByApplication(const QString& application);
    UserPreferenceList findAllByPathPrefix(const QString& application, const QString& prefix);

    bool removeAllByApplication(const QString& application);
    bool removeAllByPathPrefix(const QString& application, const QString& prefix);

private:
    Q_DISABLE_COPY(UserPreferenceDao)
};
//$END-CUSTOM$

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_USERPREFERENCEDAO_P_H
