// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIADAOS_P_H
#define NS_MEDIA_MEDIADAOS_P_H

#include "abstractdatabasedaos.h"
#include "media.h"
#include "mediaphotodao_p.h"

namespace ns {
namespace Media {

class MediaDaos : public AbstractDatabaseDaos
{
public:
    MediaPhotoDao mediaPhotoDao;

    MediaDaos();
    explicit MediaDaos(Database* db);
    virtual ~MediaDaos();

    virtual bool load(EntityData* data, ExecutionMonitor* monitor = 0);
    virtual bool save(const EntityData& data, ExecutionMonitor* monitor = 0);

protected:
    virtual void updateDatabase();

private:
    Q_DISABLE_COPY(MediaDaos)
};

} // namespace ns::Media
} // namespace ns

#endif // NS_MEDIA_MEDIADAOS_P_H
