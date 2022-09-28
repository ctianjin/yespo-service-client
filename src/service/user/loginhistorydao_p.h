// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_LOGINHISTORYDAO_P_H
#define NS_USER_LOGINHISTORYDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "loginhistory.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class LoginHistoryDaoBase : public AbstractEntityDao
{
public:
    typedef LoginHistory Entity;
    typedef LoginHistoryList EntityList;

    explicit LoginHistoryDaoBase(Database* db = 0);
    virtual ~LoginHistoryDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(const QString& userName);
    virtual int countByExample(const LoginHistory& entity);
    virtual int countAll();

    virtual LoginHistory find(const QString& userName);
    virtual LoginHistory findByExample(const LoginHistory& entity);
    virtual LoginHistoryList findAll();
    virtual LoginHistoryList findAllByExample(const LoginHistory& entity);

    virtual bool persist(LoginHistory& entity);
    virtual bool merge(const LoginHistory& entity);
    virtual bool update(const LoginHistory& entity);
    virtual bool remove(const LoginHistory& entity);
    virtual bool removeAll();
    virtual bool refresh(LoginHistory& entity);

private:
    Q_DISABLE_COPY(LoginHistoryDaoBase)
};

//$BEGIN-CUSTOM$
class LoginHistoryDao : public LoginHistoryDaoBase
{
public:
    explicit LoginHistoryDao(Database* db = 0);
    virtual ~LoginHistoryDao();

    virtual bool createTable();
    virtual bool dropTable();

private:
    Q_DISABLE_COPY(LoginHistoryDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_LOGINHISTORYDAO_P_H
