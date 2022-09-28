// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_USERDAOS_P_H
#define NS_USER_USERDAOS_P_H

#include "abstractdatabasedaos.h"
#include "user.h"
#include "loginhistorydao_p.h"
#include "callrecorddao_p.h"
#include "screenshotdao_p.h"
#include "blacklistdao_p.h"
#include "contactdao_p.h"
#include "phonehistorydao_p.h"

namespace ns {
namespace User {

class UserDaos : public AbstractDatabaseDaos
{
public:
    LoginHistoryDao loginHistoryDao;
    CallRecordDao callRecordDao;
    ScreenshotDao screenshotDao;
    BlacklistDao blacklistDao;
    ContactDao contactDao;
    PhoneHistoryDao phoneHistoryDao;

    UserDaos();
    explicit UserDaos(Database* db);
    virtual ~UserDaos();

    virtual bool load(EntityData* data, ExecutionMonitor* monitor = 0);
    virtual bool save(const EntityData& data, ExecutionMonitor* monitor = 0);

protected:
    virtual void updateDatabase();

private:
    Q_DISABLE_COPY(UserDaos)
};

} // namespace ns::User
} // namespace ns

#endif // NS_USER_USERDAOS_P_H
