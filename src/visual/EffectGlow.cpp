/*
 * EffectGlow.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: nullifiedcat
 */

#include <visual/EffectGlow.hpp>
#include <MiscTemporary.hpp>
#include <hacks/Aimbot.hpp>
#include <settings/Bool.hpp>
#include "common.hpp"

IMaterialSystem *materials = nullptr;

CScreenSpaceEffectRegistration *CScreenSpaceEffectRegistration::s_pHead = nullptr;
IScreenSpaceEffectManager *g_pScreenSpaceEffects = nullptr;
CScreenSpaceEffectRegistration **g_ppScreenSpaceRegistrationHead = nullptr;

CScreenSpaceEffectRegistration::CScreenSpaceEffectRegistration(const char *pName, IScreenSpaceEffect *pEffect)
{
    logging::Info("Creating new effect '%s', head: 0x%08x", pName, *g_ppScreenSpaceRegistrationHead);
    m_pEffectName = pName;
    m_pEffect = pEffect;
    m_pNext = *g_ppScreenSpaceRegistrationHead;
    *g_ppScreenSpaceRegistrationHead = this;
    logging::Info("New head: 0x%08x", *g_ppScreenSpaceRegistrationHead);
}

namespace effect_glow
{
static settings::Boolean health{ "glow.health", "false" };
static settings::Boolean aimbot_color{ "glow.aimbot.color", "true" };
static settings::Boolean teammates{ "glow.show.teammates", "false" };
static settings::Boolean disguised{ "glow.show.disguised", "true" };
static settings::Boolean players{ "glow.show.players", "true" };
static settings::Boolean medkits{ "glow.show.medkits", "false" };
static settings::Boolean ammobox{ "glow.show.ammoboxes", "false" };
static settings::Boolean buildings{ "glow.show.buildings", "true" };
static settings::Boolean stickies{ "glow.show.stickies", "true" };
static settings::Boolean teammate_buildings{ "glow.show.teammate-buildings", "false" };
static settings::Boolean show_powerups{ "glow.show.powerups", "true" };
static settings::Boolean weapons_white{ "glow.white-weapons", "true" };
static settings::Boolean glowself{ "glow.self", "true" };
static settings::Boolean rainbow{ "glow.self-rainbow", "true" };
static settings::Int blur_scale{ "glow.blur-scale", "5" };
static settings::Int solid_when{ "glow.solid-when", "0" };
settings::Boolean enable{ "glow.enable", "false" };

void EffectGlow::Render(int x, int y, int w, int h)
{
#if !ENFORCE_STREAM_SAFETY
    if (!enable)
        return;
    if (!isHackActive() || (clean_screenshots && g_IEngine->IsTakingScreenshot()) || g_Settings.bInvalid || disable_visuals)
        return;
    static ITexture *orig;
    static IMaterialVar *blury_bloomamount;
    if (!init)
        Init();
    CMatRenderContextPtr ptr(GET_RENDER_CONTEXT);
    orig = ptr->GetRenderTarget();
    BeginRenderGlow();
    for (const auto &ent_non_raw : entity_cache::valid_ents)
    {
        auto ent = RAW_ENT(ent_non_raw);
        if (ent && ShouldRenderGlow(ent))
            RenderGlow(ent);
    }
    EndRenderGlow();
    if (*solid_when != 1)
    {
        ptr->ClearStencilBufferRectangle(x, y, w, h, 0);
        StartStenciling();
        for (auto &non_raw : entity_cache::valid_ents)
        {
            auto ent = RAW_ENT(non_raw);
            if (ent && ShouldRenderGlow(ent))
                DrawToStencil(ent);
        }
        EndStenciling();
    }
    ptr->SetRenderTarget(GetBuffer(2));
    ptr->Viewport(x, y, w, h);
    ptr->ClearBuffers(true, false);
    ptr->DrawScreenSpaceRectangle(mat_blur_x, x, y, w, h, 0, 0, w - 1, h - 1, w, h);
    ptr->SetRenderTarget(GetBuffer(1));
    blury_bloomamount = mat_blur_y->FindVar("$bloomamount", nullptr);
    blury_bloomamount->SetIntValue(*blur_scale);
    ptr->DrawScreenSpaceRectangle(mat_blur_y, x, y, w, h, 0, 0, w - 1, h - 1, w, h);
    ptr->Viewport(x, y, w, h);
    ptr->SetRenderTarget(orig);
    g_IVRenderView->SetBlend(0.0f);
    if (*solid_when != 1)
        SS_Drawing.SetStencilState(ptr);
    ptr->DrawScreenSpaceRectangle(mat_blit, x, y, w, h, 0, 0, w - 1, h - 1, w, h);
    if (*solid_when != -1)
        SS_Null.SetStencilState(ptr);
#endif
}

EffectGlow g_EffectGlow;
CScreenSpaceEffectRegistration *g_pEffectGlow = nullptr;

static InitRoutine init(
    []()
    {
        EC::Register(
            EC::LevelShutdown, []() { g_EffectGlow.Shutdown(); }, "glow");
        if (g_ppScreenSpaceRegistrationHead && g_pScreenSpaceEffects)
        {
            effect_glow::g_pEffectGlow = new CScreenSpaceEffectRegistration("_cathook_glow", &effect_glow::g_EffectGlow);
            g_pScreenSpaceEffects->EnableScreenSpaceEffect("_cathook_glow");
        }
    });
} // namespace effect_glow
