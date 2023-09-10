/*
 * colors.cpp
 *
 *  Created on: May 22, 2017
 *      Author: nullifiedcat
 */

#include <PlayerTools.hpp>
#include "common.hpp"

namespace colors
{
settings::Rgba col_guicolor{ "colors.guicolor", "ffffffff" };

rgba_t gui    = *col_guicolor;

static InitRoutine init(
    []()
    {
        col_guicolor.installChangeCallback([](settings::VariableBase<rgba_t> &var, rgba_t after) { gui = after; });
    });
} // namespace colors

rgba_t colors::EntityF(CachedEntity *ent)
{
    rgba_t result, plclr;
    int skin;
    const model_t *model = RAW_ENT(ent)->GetModel();

    using namespace colors;
    result = white;
    if (model)
    {
        if (Hash::IsHealth(g_IModelInfo->GetModelName(model)))
            result = green;
        else if (Hash::IsPowerup(g_IModelInfo->GetModelName(model)))
        {
            skin = RAW_ENT(ent)->GetSkin();
            switch (skin)
            {
            case 1:
                result = red;
                break;
            case 2:
                result = blu;
                break;
            default:
                result = yellow;
                break;
            }
        }
    }

    if (ent->m_iClassID() == CL_CLASS(CCurrencyPack) && !CE_BYTE(ent, netvar.bDistributed))
        result = green;

    if (ent->m_Type() == ENTITY_PROJECTILE)
    {
        switch (ent->m_iTeam())
        {
        case TEAM_BLU:
            result = blu;
            break;
        case TEAM_RED:
            result = red;
            break;
        }
        if (ent->m_bCritProjectile())
        {
            switch (ent->m_iTeam())
            {
            case TEAM_BLU:
                result = blu_u;
                break;
            case TEAM_RED:
                result = red_u;
                break;
            }
        }
    }

    if (ent->m_Type() == ENTITY_PLAYER || ent->m_Type() == ENTITY_BUILDING)
    {
        switch (ent->m_iTeam())
        {
        case TEAM_BLU:
            result = blu;
            break;
        case TEAM_RED:
            result = red;
            break;
        }
        if (ent->m_Type() == ENTITY_PLAYER)
        {
            if (IsPlayerInvulnerable(ent))
            {
                switch (ent->m_iTeam())
                {
                case TEAM_BLU:
                    result = blu_u;
                    break;
                case TEAM_RED:
                    result = red_u;
                    break;
                }
            }
            if (HasCondition<TFCond_UberBulletResist>(ent))
            {
                switch (ent->m_iTeam())
                {
                case TEAM_BLU:
                    result = blu_v;
                    break;
                case TEAM_RED:
                    result = red_v;
                    break;
                }
            }
            auto o = player_tools::forceEspColor(ent);
            if (o.has_value())
                return *o;
        }
    }

    return result;
}

// Timescale determines how fast it changes
rgba_t colors::Fade(rgba_t color_a, rgba_t color_b, float time, float timescale)
{
    // Determine how much percent should be used from color_a, also remap sin to be 0.0f -> 1.0f
    float percentage_a = fabsf(sin(time * timescale));
    rgba_t new_color;
    new_color.r = (color_b.r - color_a.r) * percentage_a + color_a.r;
    new_color.g = (color_b.g - color_a.g) * percentage_a + color_a.g;
    new_color.b = (color_b.b - color_a.b) * percentage_a + color_a.b;
    new_color.a = (color_b.a - color_a.a) * percentage_a + color_a.a;
    return new_color;
}

rgba_t colors::RainbowCurrent()
{
    return colors::FromHSL(fabs(sin(g_GlobalVars->curtime / 2.0f)) * 360.0f, 0.85f, 0.9f);
}

static unsigned char hexToChar(char i)
{
    if (i >= '0' && i <= '9')
        return i - '0';
    if (i >= 'a' && i <= 'f')
        return i - 'a' + 10;
    if (i >= 'A' && i <= 'F')
        return i - 'A' + 10;
    return 0;
}

static unsigned int hexToByte(char hi, char lo)
{
    return (hexToChar(hi) << 4) | (hexToChar(lo));
}

colors::rgba_t::rgba_t(const char hex[6])
{
    auto ri = hexToByte(hex[0], hex[1]);
    auto gi = hexToByte(hex[2], hex[3]);
    auto bi = hexToByte(hex[4], hex[5]);
    r       = float(ri) / 255.0f;
    g       = float(gi) / 255.0f;
    b       = float(bi) / 255.0f;
    a       = 1.0f;
}
