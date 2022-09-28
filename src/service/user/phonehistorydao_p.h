// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_PHONEHISTORYDAO_P_H
#define NS_USER_PHONEHISTORYDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "phonehistory.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class PhoneHistoryDaoBase : public AbstractEntityDao
{
public:
    typedef PhoneHistory Entity;
    typedef PhoneHistoryList EntityList;

    explicit PhoneHistoryDaoBase(Database* db = 0);
    virtual ~PhoneHistoryDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const PhoneHistory& entity);
    virtual int countAll();

    virtual PhoneHistory find(qlonglong id);
    virtual PhoneHistory findByExample(const PhoneHistory& entity);
    virtual PhoneHistoryList findAll();
    virtual PhoneHistoryList findAllByExample(const PhoneHistory& entity);

    virtual bool persist(PhoneHistory& entity);
    virtual bool merge(const PhoneHistory& entity);
    virtual bool update(const PhoneHistory& entity);
    virtual bool remove(const PhoneHistory& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(PhoneHistory& entity);

private:
    Q_DISABLE_COPY(PhoneHistoryDaoBase)
};

//$BEGIN-CUSTOM$
class PhoneHistoryDao : public PhoneHistoryDaoBase
{
public:
    explicit PhoneHistoryDao(Database* db = 0);
    virtual ~PhoneHistoryDao();

    virtual bool createTable();
    virtual bool dropTable();

	PhoneHistoryList findAllByUserName(const QString& userName);
	bool removeByUserNameAndMeetCDRID(const QString& userName, const QString& meetCDRID);
	PhoneHistory findRecentByUserName(const QString& userName);

private:
    Q_DISABLE_COPY(PhoneHistoryDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_PHONEHISTORYDAO_P_H
