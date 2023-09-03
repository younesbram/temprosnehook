/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <hacks/hacklist.hpp>
#include <settings/Bool.hpp>
#include <hacks/Thirdperson.hpp>
#include "HookedMethods.hpp"
#include "AntiAntiAim.hpp"

static settings::Boolean no_shake{ "visual.no-shake", "false" };
static settings::Boolean override_textures{ "visual.override-textures", "false" };
static settings::String override_textures_texture{ "visual.override-textures.custom-texture", "dev/dev_measuregeneric01b" };

// Should we update?
static bool update_override_textures = false;

// Which strings trigger this nightmode option
std::vector<std::string> world_strings         = { "World" };
std::vector<std::string> skybox_strings        = { "SkyBox" };
std::vector<std::string> gui_strings           = { "Other", "VGUI" };
std::vector<std::string> dont_override_strings = { "glass", "door", "water", "tools", "player", "wall28", "wall26" };
std::vector<std::string> nodraw_strings        = { "decal", "overlay", "hay" };

namespace hooked_methods
{
#include "reclasses.hpp"
DEFINE_HOOKED_METHOD(FrameStageNotify, void, void *this_, ClientFrameStage_t stage)
{
    if (!isHackActive())
        return original::FrameStageNotify(this_, stage);

    PROF_SECTION(FrameStageNotify_TOTAL)

    if (update_override_textures)
    {
        if (override_textures)
        {
            for (MaterialHandle_t i = g_IMaterialSystem->FirstMaterial(); i != g_IMaterialSystem->InvalidMaterial(); i = g_IMaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_IMaterialSystem->GetMaterial(i);
                if (!pMaterial)
                    continue;

                auto name = std::string(pMaterial->GetTextureGroupName());
                auto path = std::string(pMaterial->GetName());

                // Ensure world mat
                if (name.find("World") == std::string::npos)
                    continue;
                if (pMaterial->IsErrorMaterial() || !pMaterial->IsPrecached() || pMaterial->IsTranslucent() || pMaterial->IsSpriteCard())
                    continue;
                // Don't override this stuff
                bool good = true;
                for (const auto &entry : dont_override_strings)
                    if (path.find(entry) != std::string::npos)
                        good = false;

                // Don't draw this stuff
                for (const auto &entry : nodraw_strings)
                    if (path.find(entry) != std::string::npos)
                    {
                        pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                        good = false;
                    }
                if (!good)
                    continue;

                if (!pMaterial->GetMaterialVarFlag(MATERIAL_VAR_NO_DRAW))
                {
                    auto *kv = new KeyValues(pMaterial->GetShaderName());
                    kv->SetString("$basetexture", (*override_textures_texture).c_str());
                    kv->SetString("$surfaceprop", "concrete");
                    pMaterial->SetShaderAndParams(kv);
                }
            }
        }
        update_override_textures = false;
    }

    if (!g_IEngine->IsInGame())
        g_Settings.bInvalid = true;
    {
        PROF_SECTION(FSN_antiantiaim)
        hacks::anti_anti_aim::frameStageNotify(stage);
    }
    std::optional<Vector> backup_punch;
    if (isHackActive() && !g_Settings.bInvalid && stage == FRAME_RENDER_START)
    {
        if (no_shake && CE_GOOD(LOCAL_E) && LOCAL_E->m_bAlivePlayer())
        {
            backup_punch                                       = NET_VECTOR(RAW_ENT(LOCAL_E), netvar.vecPunchAngle);
            NET_VECTOR(RAW_ENT(LOCAL_E), netvar.vecPunchAngle) = { 0.0f, 0.0f, 0.0f };
        }
        hacks::thirdperson::frameStageNotify();
    }
    original::FrameStageNotify(this_, stage);
    if (backup_punch)
        NET_VECTOR(RAW_ENT(LOCAL_E), netvar.vecPunchAngle) = *backup_punch;
}
static InitRoutine init_fsn(
    []()
    {
        override_textures.installChangeCallback([](settings::VariableBase<bool> &, bool after) { update_override_textures = true; });
        override_textures_texture.installChangeCallback([](settings::VariableBase<std::string> &, const std::string &after) { update_override_textures = true; });
        EC::Register(
            EC::LevelInit,
            []()
            {
                update_override_textures = true;
            },
            "levelinit_fsn");
    });
} // namespace hooked_methods