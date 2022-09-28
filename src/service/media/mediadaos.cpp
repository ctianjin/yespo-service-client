// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include "mediadaos_p.h"
#include "database.h"
#include "transaction.h"
#include "entitydata.h"
#include "executionmonitor.h"

namespace ns {
namespace Media {

MediaDaos::MediaDaos()
{
}

MediaDaos::MediaDaos(Database* db)
{
    setDatabase(db);
}

MediaDaos::~MediaDaos()
{
}

void MediaDaos::updateDatabase()
{
    Database* db = database();

    mediaPhotoDao.setDatabase(db);
}

bool MediaDaos::load(EntityData* data, ExecutionMonitor* monitor)
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
    monitor->setProgressRange(0, 1);

    Transaction trans(db);

    // MediaPhoto
    {
        if (monitor->isCanceled())
            return false;

        const MediaPhotoList entities = mediaPhotoDao.findAll();
        if (mediaPhotoDao.isError()) {
            monitor->setError(mediaPhotoDao.errorText());
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

bool MediaDaos::save(const EntityData& data, ExecutionMonitor* monitor)
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
    monitor->setProgressRange(0, 1);

    Transaction trans(db);

    // MediaPhoto
    {
        if (monitor->isCanceled())
            return false;

        if (!mediaPhotoDao.removeAll()) {
            monitor->setError(mediaPhotoDao.errorText());
            return false;
        }

        Q_FOREACH(MediaPhoto entity, data.value<MediaPhotoList>()) {
            if (!mediaPhotoDao.persist(entity)) {
                monitor->setError(mediaPhotoDao.errorText());
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

} // namespace ns::Media
} // namespace ns
