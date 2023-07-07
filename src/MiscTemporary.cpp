/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "MiscTemporary.hpp"
#include "Warp.hpp"
#include "nospread.hpp"

std::array<Timer, 32> timers{};
std::array<int, 32> bruteint{};

int spectator_target;
CLC_VoiceData *voicecrash{};
bool firstcm = false;
Timer DelayTimer{};
float prevflow = 0.0f;
int prevflowticks = 0;
int stored_buttons = 0;
bool calculated_can_shoot = false;
bool ignoredc = false;

bool *bSendPackets{nullptr};
bool ignoreKeys{false};
settings::Boolean clean_chat{"chat.clean", "false"};

#if ENABLE_TEXTMODE
settings::Boolean nolerp{"misc.no-lerp", "true"};
#else
settings::Boolean nolerp{"misc.no-lerp", "false"};
#endif
float backup_lerp = 0.0f;
settings::Int fakelag_amount{"misc.fakelag", "0"};
settings::Boolean fakelag_midair{"misc.fakelag-midair-only", "false"};
settings::Boolean no_zoom{"remove.zoom", "true"};
settings::Boolean no_scope{"remove.scope", "true"};
settings::Boolean disable_visuals{"visual.disable", "false"};
settings::Int print_r{"print.rgb.r", "183"};
settings::Int print_g{"print.rgb.b", "27"};
settings::Int print_b{"print.rgb.g", "139"};
Color menu_color{*print_r, *print_g, *print_b, 255};

void color_callback(settings::VariableBase<int> &, int)
{
    menu_color = Color(*print_r, *print_g, *print_b, 255);
}
DetourHook cl_warp_sendmovedetour;
DetourHook cl_nospread_sendmovedetour;

static std::optional<BytePatch> zoom_patch;

void ApplyZoomPatch()
{
    if (!zoom_patch)
    {
        // Keep rifle visible
        zoom_patch = BytePatch(CSignature::GetClientSignature, "74 ? A1 ? ? ? ? 8B 40 ? 85 C0 75 ? C9", 0x0, {0x70});
    }

    zoom_patch->Patch();
}

void ShutdownZoomPatch()
{
    if (zoom_patch)
    {
        zoom_patch->Shutdown();
    }
}

void InitializePatches()
{
    no_zoom.installChangeCallback(
        [](settings::VariableBase<bool> &, bool after)
        {
            if (after)
                ApplyZoomPatch();
            else
                ShutdownZoomPatch();
        });
}

static InitRoutine misc_init([]() {
    static auto cl_sendmove_addr = CSignature::GetEngineSignature("55 89 E5 57 56 53 81 EC 2C 10 00 00 C6 85 ? ? ? ? 01");
    // Order matters!
    cl_warp_sendmovedetour.Init(cl_sendmove_addr, (void *)hacks::warp::CL_SendMove_hook);
    cl_nospread_sendmovedetour.Init(cl_sendmove_addr, (void *)hacks::nospread::CL_SendMove_hook);

    print_r.installChangeCallback(color_callback);
    print_g.installChangeCallback(color_callback);
    print_b.installChangeCallback(color_callback);

    InitializePatches();

    EC::Register(EC::Shutdown, []() {
        cl_warp_sendmovedetour.Shutdown();
        cl_nospread_sendmovedetour.Shutdown();
        ShutdownZoomPatch();
    },
        "misctemp_shutdown");

#if ENABLE_TEXTMODE
    // Ensure that we trigger the callback for textmode builds
    nolerp = false;
    nolerp = true;
#endif
});