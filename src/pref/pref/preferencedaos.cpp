// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include "preferencedaos_p.h"
#include "database.h"
#include "transaction.h"
#include "entitydata.h"
#include "executionmonitor.h"

namespace ns {
namespace Pref {

PreferenceDaos::PreferenceDaos()
{
}

PreferenceDaos::PreferenceDaos(Database* db)
{
    setDatabase(db);
}

PreferenceDaos::~PreferenceDaos()
{
}

void PreferenceDaos::updateDatabase()
{
    Database* db = database();

    defaultPreferenceDao.setDatabase(db);
    userPreferenceDao.setDatabase(db);
}

bool PreferenceDaos::load(EntityData* data, ExecutionMonitor* monitor)
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
    monitor->setProgressRange(0, 2);

    Transaction trans(db);

    // DefaultPreference
    {
        if (monitor->isCanceled())
            return false;

        const DefaultPreferenceList entities = defaultPreferenceDao.findAll();
        if (defaultPreferenceDao.isError()) {
            monitor->setError(defaultPreferenceDao.errorText());
            return false;
        }

        data->setValue(entities);
        monitor->progress();
    }

    // UserPreference
    {
        if (monitor->isCanceled())
            return false;

        const UserPreferenceList entities = userPreferenceDao.findAll();
        if (userPreferenceDao.isError()) {
            monitor->setError(userPreferenceDao.errorText());
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

bool PreferenceDaos::save(const EntityData& data, ExecutionMonitor* monitor)
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
    monitor->setProgressRange(0, 2);

    Transaction trans(db);

    // DefaultPreference
    {
        if (monitor->isCanceled())
            return false;

        if (!defaultPreferenceDao.removeAll()) {
            monitor->setError(defaultPreferenceDao.errorText());
            return false;
        }

        Q_FOREACH(DefaultPreference entity, data.value<DefaultPreferenceList>()) {
            if (!defaultPreferenceDao.persist(entity)) {
                monitor->setError(defaultPreferenceDao.errorText());
                return false;
            }
        }
        monitor->progress();
    }

    // UserPreference
    {
        if (monitor->isCanceled())
            return false;

        if (!userPreferenceDao.removeAll()) {
            monitor->setError(userPreferenceDao.errorText());
            return false;
        }

        Q_FOREACH(UserPreference entity, data.value<UserPreferenceList>()) {
            if (!userPreferenceDao.persist(entity)) {
                monitor->setError(userPreferenceDao.errorText());
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

} // namespace ns::Pref
} // namespace ns
