// $Id$
/*
 * Copyright (C) 2014 Qpid Network
 * All rights reserved.
 *
 * Contact: Development Department <addison@qpidnetwork.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef QPID_USER_CALLHISTORYDAO_P_H
#define QPID_USER_CALLHISTORYDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "callhistory.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace Qpid {
namespace User {

class CallHistoryDaoBase : public AbstractEntityDao
{
public:
    typedef CallHistory Entity;
    typedef CallHistoryList EntityList;

    explicit CallHistoryDaoBase(Database* db = 0);
    virtual ~CallHistoryDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const CallHistory& entity);
    virtual int countAll();

    virtual CallHistory find(qlonglong id);
    virtual CallHistory findByExample(const CallHistory& entity);
    virtual CallHistoryList findAll();
    virtual CallHistoryList findAllByExample(const CallHistory& entity);

    virtual bool persist(CallHistory& entity);
    virtual bool merge(const CallHistory& entity);
    virtual bool update(const CallHistory& entity);
    virtual bool remove(const CallHistory& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(CallHistory& entity);

private:
    Q_DISABLE_COPY(CallHistoryDaoBase)
};

//$BEGIN-CUSTOM$
class CallHistoryDao : public CallHistoryDaoBase
{
public:
    explicit CallHistoryDao(Database* db = 0);
    virtual ~CallHistoryDao();

    virtual bool createTable();
    virtual bool dropTable();

	CallHistoryList findAllByUsername(const QString& userName);
	CallHistoryList findAllByTypeAndDatetime(const CallHistory& entity);
	CallHistory latestCallHistory(const QString& userName);

private:
    Q_DISABLE_COPY(CallHistoryDao)
};
//$END-CUSTOM$

} // namespace Qpid::User
} // namespace Qpid

#endif // QPID_USER_CALLHISTORYDAO_P_H
