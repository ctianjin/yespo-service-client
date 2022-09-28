// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include "userdaos_p.h"
#include "database.h"
#include "transaction.h"
#include "entitydata.h"
#include "executionmonitor.h"

namespace ns {
namespace User {

UserDaos::UserDaos()
{
}

UserDaos::UserDaos(Database* db)
{
    setDatabase(db);
}

UserDaos::~UserDaos()
{
}

void UserDaos::updateDatabase()
{
    Database* db = database();

    loginHistoryDao.setDatabase(db);
    callRecordDao.setDatabase(db);
    screenshotDao.setDatabase(db);
    blacklistDao.setDatabase(db);
    contactDao.setDatabase(db);
    phoneHistoryDao.setDatabase(db);
}

bool UserDaos::load(EntityData* data, ExecutionMonitor* monitor)
{
    if (monitor && monitor->isCanceled())
        return false;

    if (!data)
        return false;

    Database* db = database();
    if (!db)
        return false;

    NullExecutionMonitor nullMonitor;
    if (!monitor) {
        monitor = &nullMonitor;
    }
    monitor->setProgressRange(0, 6);

    Transaction trans(db);

    // LoginHistory
    {
        if (monitor->isCanceled())
            return false;

        const LoginHistoryList entities = loginHistoryDao.findAll();
        if (loginHistoryDao.isError()) {
            monitor->setError(loginHistoryDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // CallRecord
    {
        if (monitor->isCanceled())
            return false;

        const CallRecordList entities = callRecordDao.findAll();
        if (callRecordDao.isError()) {
            monitor->setError(callRecordDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // Screenshot
    {
        if (monitor->isCanceled())
            return false;

        const ScreenshotList entities = screenshotDao.findAll();
        if (screenshotDao.isError()) {
            monitor->setError(screenshotDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // Blacklist
    {
        if (monitor->isCanceled())
            return false;

        const BlacklistList entities = blacklistDao.findAll();
        if (blacklistDao.isError()) {
            monitor->setError(blacklistDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // Contact
    {
        if (monitor->isCanceled())
            return false;

        const ContactList entities = contactDao.findAll();
        if (contactDao.isError()) {
            monitor->setError(contactDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // PhoneHistory
    {
        if (monitor->isCanceled())
            return false;

        const PhoneHistoryList entities = phoneHistoryDao.findAll();
        if (phoneHistoryDao.isError()) {
            monitor->setError(phoneHistoryDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    if (trans.commit()) {
        return true;
    } else {
        monitor->setError(db->errorText());
        return false;
    }
}

bool UserDaos::save(const EntityData& data, ExecutionMonitor* monitor)
{
    if (monitor && monitor->isCanceled())
        return false;

    Database* db = database();
    if (!db)
        return false;

    NullExecutionMonitor nullMonitor;
    if (!monitor) {
        monitor = &nullMonitor;
    }
    monitor->setProgressRange(0, 6);

    Transaction trans(db);

    // LoginHistory
    {
        if (monitor->isCanceled())
            return false;

        if (!loginHistoryDao.removeAll()) {
            monitor->setError(loginHistoryDao.errorText());
            return false;
        }

        Q_FOREACH(LoginHistory entity, data.value<LoginHistoryList>()) {
            if (!loginHistoryDao.persist(entity)) {
                monitor->setError(loginHistoryDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // CallRecord
    {
        if (monitor->isCanceled())
            return false;

        if (!callRecordDao.removeAll()) {
            monitor->setError(callRecordDao.errorText());
            return false;
        }

        Q_FOREACH(CallRecord entity, data.value<CallRecordList>()) {
            if (!callRecordDao.persist(entity)) {
                monitor->setError(callRecordDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // Screenshot
    {
        if (monitor->isCanceled())
            return false;

        if (!screenshotDao.removeAll()) {
            monitor->setError(screenshotDao.errorText());
            return false;
        }

        Q_FOREACH(Screenshot entity, data.value<ScreenshotList>()) {
            if (!screenshotDao.persist(entity)) {
                monitor->setError(screenshotDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // Blacklist
    {
        if (monitor->isCanceled())
            return false;

        if (!blacklistDao.removeAll()) {
            monitor->setError(blacklistDao.errorText());
            return false;
        }

        Q_FOREACH(Blacklist entity, data.value<BlacklistList>()) {
            if (!blacklistDao.persist(entity)) {
                monitor->setError(blacklistDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // Contact
    {
        if (monitor->isCanceled())
            return false;

        if (!contactDao.removeAll()) {
            monitor->setError(contactDao.errorText());
            return false;
        }

        Q_FOREACH(Contact entity, data.value<ContactList>()) {
            if (!contactDao.persist(entity)) {
                monitor->setError(contactDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // PhoneHistory
    {
        if (monitor->isCanceled())
            return false;

        if (!phoneHistoryDao.removeAll()) {
            monitor->setError(phoneHistoryDao.errorText());
            return false;
        }

        Q_FOREACH(PhoneHistory entity, data.value<PhoneHistoryList>()) {
            if (!phoneHistoryDao.persist(entity)) {
                monitor->setError(phoneHistoryDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    if (trans.commit()) {
        return true;
    } else {
        monitor->setError(db->errorText());
        return false;
    }
}

} // namespace ns::User
} // namespace ns
