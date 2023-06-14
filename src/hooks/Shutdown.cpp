/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <hacks/hacklist.hpp>
#include <settings/Bool.hpp>
#include "HookedMethods.hpp"
#include "MiscTemporary.hpp"
#include "votelogger.hpp"

settings::Boolean die_if_vac{ "misc.die-if-vac", "false" };
static settings::Boolean auto_abandon{ "misc.auto-abandon", "false" };
static settings::String custom_disconnect_reason{ "misc.disconnect-reason", "" };
settings::Boolean random_name{ "misc.random-name", "false" };
extern settings::String force_name;
extern std::string name_forced;

namespace hooked_methods
{

static TextFile randomnames_file;

DEFINE_HOOKED_METHOD(Shutdown, void, INetChannel *this_, const char *reason)
{
    g_Settings.bInvalid = true;
    logging::Info("Disconnect: %s", reason);
    if (*die_if_vac && strstr(reason, "banned") || strstr(reason, "Generic_Kicked") && tfmm::IsMMBanned())
    {
        logging::Info("VAC/Matchmaking banned");
        *(int *) nullptr = 0;
        exit(1);
    }
}

} // namespace hooked_methods
