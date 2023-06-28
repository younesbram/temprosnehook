/*
 * AntiBackstab.cpp
 *
 *  Created on: Apr 14, 2017
 *      Author: nullifiedcat
 */

#include <settings/Bool.hpp>
#include "common.hpp"
#include "hack.hpp"

namespace hacks::autobackstab
{
extern bool angleCheck(CachedEntity *from, CachedEntity *to, std::optional<Vector> target_pos, Vector from_angle);
}
namespace hacks::antibackstab
{
static settings::Boolean enable{ "antibackstab.enable", "false" };
static settings::Float distance{ "antibackstab.distance", "200" };
bool noaa = false;

float GetAngle(CachedEntity *spy)
{
    float yaw, yaw2, anglediff;
    Vector diff;
    yaw             = g_pLocalPlayer->v_OrigViewangles.y;
    const Vector &A = LOCAL_E->m_vecOrigin();
    const Vector &B = spy->m_vecOrigin();
    diff            = (A - B);
    yaw2            = acos(diff.x / diff.Length()) * 180.0f / PI;
    if (diff.y < 0)
        yaw2 = -yaw2;
    anglediff = yaw - yaw2;
    if (anglediff > 180)
        anglediff -= 360;
    if (anglediff < -180)
        anglediff += 360;
    // logging::Info("Angle: %.2f | %.2f | %.2f | %.2f", yaw, yaw2, anglediff, yaw - yaw2);
    return anglediff;
}
static InitRoutine EC(
    []()
    {
        EC::Register(EC::CreateMove, CreateMove, "antibackstab", EC::late);
        EC::Register(EC::CreateMoveWarp, CreateMove, "antibackstab_w", EC::late);
    });
} // namespace hacks::antibackstab
