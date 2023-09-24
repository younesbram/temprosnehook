/*
 * projlogging.cpp
 *
 *  Created on: May 26, 2017
 *      Author: nullifiedcat
 */

#include "projlogging.hpp"
#include "common.hpp"

namespace projectile_logging
{
std::unordered_map<u_int16_t, Vector> prevloc;

void Update()
{
    for (const auto &ent : entity_cache::valid_ents)
    {
        bool issandwich         = false;
        const uint16_t curr_idx = ent->m_IDX;
        if (ent->m_Type() == ENTITY_PROJECTILE || issandwich)
        {
            /*int owner = HandleToIDX(CE_INT(ent, 0x894));
            if (owner != LOCAL_W->m_IDX)
                continue;*/
            if (tickcount % 20 == 0)
            {
                Vector abs_orig = RAW_ENT(ent)->GetAbsOrigin();
                float movement  = prevloc[curr_idx].DistTo(abs_orig);
                logging::Info("movement: %f", movement);
                prevloc[curr_idx] = abs_orig;
                const Vector &v   = ent->m_vecVelocity;
                Vector eav;
                velocity::EstimateAbsVelocity(RAW_ENT(ent), eav);
                //				logging::Info("%d [%s]: CatVelocity: %.2f %.2f
                //%.2f
                //(%.2f) | EAV: %.2f %.2f %.2f (%.2f)", i,
                // RAW_ENT(ent)->GetClientClass()->GetName(), v.x, v.y, v.z,
                // v.Length(), a.x, a.y, a.z);
                logging::Info("%d [%s]: CatVelocity: %.2f %.2f %.2f (%.2f) | "
                              "EAV: %.2f %.2f %.2f (%.2f)",
                              curr_idx, RAW_ENT(ent)->GetClientClass()->GetName(), v.x, v.y, v.z, v.Length(), eav.x, eav.y, eav.z, eav.Length());
            }
        }
    }
}
} // namespace projectile_logging
