#include <settings/Bool.hpp>
#include "common.hpp"

namespace hacks::antitaunts
{
#if ENABLE_TEXTMODE
static settings::Boolean enable{ "remove.taunts", "true" };
#else
static settings::Boolean enable{ "remove.taunts", "false" };
#endif

static void CreateMove()
{
    if (!*enable)
        return;

    for (const auto &ent : entity_cache::player_cache)
    {
        if (RAW_ENT(ent)->IsDormant() || ent->m_Type() != ENTITY_PLAYER)
            continue;

        RemoveCondition<TFCond_Taunting>(ent);
    }
}

static InitRoutine EC(
    []()
    {
        EC::Register(EC::CreateMove, CreateMove, "antitaunts", EC::average);
        EC::Register(EC::CreateMoveWarp, CreateMove, "antitaunts_w", EC::average);
    });
} // namespace hacks::antitaunts
