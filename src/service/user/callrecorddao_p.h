// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CALLRECORDDAO_P_H
#define NS_USER_CALLRECORDDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "callrecord.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class CallRecordDaoBase : public AbstractEntityDao
{
public:
    typedef CallRecord Entity;
    typedef CallRecordList EntityList;

    explicit CallRecordDaoBase(Database* db = 0);
    virtual ~CallRecordDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const CallRecord& entity);
    virtual int countAll();

    virtual CallRecord find(qlonglong id);
    virtual CallRecord findByExample(const CallRecord& entity);
    virtual CallRecordList findAll();
    virtual CallRecordList findAllByExample(const CallRecord& entity);

    virtual bool persist(CallRecord& entity);
    virtual bool merge(const CallRecord& entity);
    virtual bool update(const CallRecord& entity);
    virtual bool remove(const CallRecord& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(CallRecord& entity);

private:
    Q_DISABLE_COPY(CallRecordDaoBase)
};

//$BEGIN-CUSTOM$
class CallRecordDao : public CallRecordDaoBase
{
public:
    explicit CallRecordDao(Database* db = 0);
    virtual ~CallRecordDao();

    virtual bool createTable();
    virtual bool dropTable();

	CallRecordList findAllByOwner(const QString& owner);
	CallRecord findRecentByOwner(const QString& owner);
	CallRecord findByOwnerAndMeetCDRId(const QString& owner, const QString& meetCDRId);
	CallRecordList callRecordAnswered(const QString& owner, const QString& start, const QString& end);
	CallRecordList callRecordSuccess(const QString& owner, const QString& start, const QString& end);
	CallRecordList callRecordMissed(const QString& owner, const QString& start, const QString& end);

private:
    Q_DISABLE_COPY(CallRecordDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_CALLRECORDDAO_P_H
