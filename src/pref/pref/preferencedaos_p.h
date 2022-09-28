// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "pref.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_PREF_PREFERENCEDAOS_P_H
#define NS_PREF_PREFERENCEDAOS_P_H

#include "abstractdatabasedaos.h"
#include "preference.h"
#include "defaultpreferencedao_p.h"
#include "userpreferencedao_p.h"

namespace ns {
namespace Pref {

class PreferenceDaos : public AbstractDatabaseDaos
{
public:
    DefaultPreferenceDao defaultPreferenceDao;
    UserPreferenceDao userPreferenceDao;

    PreferenceDaos();
    explicit PreferenceDaos(Database* db);
    virtual ~PreferenceDaos();

    virtual bool load(EntityData* data, ExecutionMonitor* monitor = 0);
    virtual bool save(const EntityData& data, ExecutionMonitor* monitor = 0);

protected:
    virtual void updateDatabase();

private:
    Q_DISABLE_COPY(PreferenceDaos)
};

} // namespace ns::Pref
} // namespace ns

#endif // NS_PREF_PREFERENCEDAOS_P_H
