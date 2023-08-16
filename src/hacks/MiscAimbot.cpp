//
// Created by bencat07 on 28.09.18.
//

#include "common.hpp"
#include "settings/Bool.hpp"
#include "PlayerTools.hpp"
#include "MiscAimbot.hpp"
#include "DetourHook.hpp"
#include "Backtrack.hpp"

namespace hacks::misc_aimbot
{
constexpr float grav           = 0.25f;
constexpr float initial_vel    = 200.0f;
int prevent                    = -1;
static Timer previous_entity_delay{};

std::pair<CachedEntity *, Vector> FindBestEnt(bool teammate, bool Predict, bool zcheck,  float range)
{
    CachedEntity *bestent                             = nullptr;
    float bestscr                                     = FLT_MAX;
    std::optional<backtrack::BacktrackData> best_data = std::nullopt;
    Vector predicted{};
    // Too long since we focused it
    if (previous_entity_delay.check(100))
        prevent = -1;

    bool shouldBacktrack = backtrack::backtrackEnabled() && !backtrack::hasData();

    auto calculateEntity = [&](CachedEntity *ent) -> std::optional<std::tuple<float, Vector, std::optional<backtrack::BacktrackData>>>
    {
        if (CE_BAD(ent) || !ent->m_bAlivePlayer() || (teammate && ent->m_iTeam() != g_pLocalPlayer->team) || ent == LOCAL_E)
            return std::nullopt;
        if (!teammate && ent->m_iTeam() == g_pLocalPlayer->team)
            return std::nullopt;
        if (!ent->hitboxes.GetHitbox(1))
            return std::nullopt;
        if (!teammate && !player_tools::shouldTarget(ent))
            return std::nullopt;

        Vector target{};

        else
            target = ent->hitboxes.GetHitbox(1)->center;

        if (!shouldBacktrack && !IsEntityVectorVisible(ent, target))
            return std::nullopt;

        if (zcheck && (ent->m_vecOrigin().z - g_pLocalPlayer->v_Origin.z) > 200.0f)
            return std::nullopt;

        float scr = ent->m_flDistance();

        std::optional<backtrack::BacktrackData> data = std::nullopt;

        if (g_pPlayerResource->GetClass(ent) == tf_medic)
            scr *= 0.5f;

        return { { scr, target, data } };
    };

    for (const auto &ent : entity_cache::player_cache)
    {
        if (prevent != -1)
        {
            auto result = calculateEntity(ent);
            if (result && std::get<0>(*result) < bestscr)
            {
                bestent   = ent;
                predicted = std::get<1>(*result);
                bestscr   = std::get<0>(*result);
                prevent   = ent->m_IDX;
                if (shouldBacktrack)
                    best_data = std::get<2>(*result);
            }
            previous_entity_delay.update();
        }
    }
    return { bestent, predicted };
}
static float slow_change_dist_y{};
static float slow_change_dist_p{};
void DoSlowAim(Vector &input_angle, float speed)
{
    auto viewangles = current_user_cmd->viewangles;

    // Yaw
    if (viewangles.y != input_angle.y)
    {
        float flChargeYawCap = re::CTFPlayerShared::CalculateChargeCap(re::CTFPlayerShared::GetPlayerShared(RAW_ENT(LOCAL_E)));
        flChargeYawCap *= 2.5f;

        // Check if input angle and user angle are on opposing sides of yaw so
        // we can correct for that
        bool slow_opposing = false;
        if ((input_angle.y < -360 && viewangles.y > 360) || (input_angle.y > 360 && viewangles.y < -360))
            slow_opposing = true;

        // Direction
        bool slow_dir = false;
        if (slow_opposing)
        {
            if (input_angle.y > 360 && viewangles.y < -360)
                slow_dir = true;
        }
        else if (viewangles.y > input_angle.y)
            slow_dir = true;

        // Speed, check if opposing. We dont get a new distance due to the
        // opposing sides making the distance spike, so just cheap out and reuse
        // our last one.
        if (!slow_opposing)
            slow_change_dist_y = std::abs(viewangles.y - input_angle.y) / (int) speed;

        // Move in the direction of the input angle
        if (slow_dir)
            input_angle.y = viewangles.y - slow_change_dist_y;
        else
            input_angle.y = viewangles.y + slow_change_dist_y;
    }

    // Pitch
    if (viewangles.x != input_angle.x)
    {
        // Get speed
        slow_change_dist_p = std::abs(viewangles.x - input_angle.x) / speed;

        // Move in the direction of the input angle
        if (viewangles.x > input_angle.x)
            input_angle.x = viewangles.x - slow_change_dist_p;
        else
            input_angle.x = viewangles.x + slow_change_dist_p;
    }

    // Clamp as we changed angles
    fClampAngle(input_angle);
}

static void CreateMove()
{

}

DetourHook CAM_CapYaw_detour;
typedef float (*CAM_CapYaw_t)(IInput *, float);
// Fix client side limit being applied weirdly, Note that most of this is taken from the source leak directly
float CAM_CapYaw_Hook(IInput *this_, float fVal)
{
    if (CE_INVALID(LOCAL_E))
        return fVal;

    if (HasCondition<TFCond_Charging>(LOCAL_E))
    {
        float flChargeYawCap = re::CTFPlayerShared::CalculateChargeCap(re::CTFPlayerShared::GetPlayerShared(RAW_ENT(LOCAL_E)));

        // Our only change
        flChargeYawCap *= 2.5f;

        if (fVal > flChargeYawCap)
            return flChargeYawCap;
        else if (fVal < -flChargeYawCap)
            return -flChargeYawCap;
    }

    return fVal;
}

#define foffset(p, i) ((uint8_t *) &p)[i]
static InitRoutine init(
    []()
    {
        EC::Register(EC::CreateMove, CreateMove, "cm_miscaimbot", EC::average);
        EC::Register(EC::CreateMoveWarp, CreateMove, "cmw_miscaimbot", EC::average);

        static auto signature = CSignature::GetClientSignature("55 89 E5 53 83 EC 14 E8 ? ? ? ? 85 C0 74 ? 8D 98 ? ? ? ? C7 44 24 ? 11 00 00 00");

        CAM_CapYaw_detour.Init(signature, (void *) CAM_CapYaw_Hook);
        EC::Register(
            EC::Shutdown, []() { CAM_CapYaw_detour.Shutdown(); }, "chargeaim_shutdown");
    });
} // namespace hacks::misc_aimbot