// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_BLACKLISTDAO_P_H
#define NS_USER_BLACKLISTDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "blacklist.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class BlacklistDaoBase : public AbstractEntityDao
{
public:
    typedef Blacklist Entity;
    typedef BlacklistList EntityList;

    explicit BlacklistDaoBase(Database* db = 0);
    virtual ~BlacklistDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const Blacklist& entity);
    virtual int countAll();

    virtual Blacklist find(qlonglong id);
    virtual Blacklist findByExample(const Blacklist& entity);
    virtual BlacklistList findAll();
    virtual BlacklistList findAllByExample(const Blacklist& entity);

    virtual bool persist(Blacklist& entity);
    virtual bool merge(const Blacklist& entity);
    virtual bool update(const Blacklist& entity);
    virtual bool remove(const Blacklist& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(Blacklist& entity);

private:
    Q_DISABLE_COPY(BlacklistDaoBase)
};

//$BEGIN-CUSTOM$
class BlacklistDao : public BlacklistDaoBase
{
public:
    explicit BlacklistDao(Database* db = 0);
    virtual ~BlacklistDao();

    virtual bool createTable();
    virtual bool dropTable();

	virtual Blacklist findRecentByOwner(const QString& owner);
	virtual BlacklistList findAllByOwner(const QString& owner);
	virtual Blacklist findBlacklistIdByName(const QString& name);
	virtual void removeRecentById(const QString& id);

private:
    Q_DISABLE_COPY(BlacklistDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_BLACKLISTDAO_P_H
