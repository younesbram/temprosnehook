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
// https://puu.sh/vobH4/5da8367aef.png
static settings::Int solid_when{ "glow.solid-when", "0" };
settings::Boolean enable{ "glow.enable", "false" };

struct ShaderStencilState_t
{
    bool m_bEnable{};
    StencilOperation_t m_FailOp;
    StencilOperation_t m_ZFailOp;
    StencilOperation_t m_PassOp;
    StencilComparisonFunction_t m_CompareFunc;
    int m_nReferenceValue{};
    uint32 m_nTestMask{};
    uint32 m_nWriteMask{};

    ShaderStencilState_t()
    {
        Reset();
    }

    inline void Reset()
    {
        m_bEnable = false;
        m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
        m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
        m_nReferenceValue = 0;
        m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
    }

    inline void SetStencilState(CMatRenderContextPtr &pRenderContext) const
    {
        pRenderContext->SetStencilEnable(m_bEnable);
        pRenderContext->SetStencilFailOperation(m_FailOp);
        pRenderContext->SetStencilZFailOperation(m_ZFailOp);
        pRenderContext->SetStencilPassOperation(m_PassOp);
        pRenderContext->SetStencilCompareFunction(m_CompareFunc);
        pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
        pRenderContext->SetStencilTestMask(m_nTestMask);
        pRenderContext->SetStencilWriteMask(m_nWriteMask);
    }
};

static CTextureReference buffers[4]{};

ITexture *GetBuffer(int i)
{
    if (!buffers[i])
    {
        ITexture *fullframe;
        fullframe = g_IMaterialSystem->FindTexture("_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET);
        std::unique_ptr<char[]> newname(new char[32]);
        std::string name = format("_cathook_buff", i);
        strncpy(newname.get(), name.c_str(), 30);
        logging::Info("Creating new buffer %d with size %dx%d %s", i, fullframe->GetActualWidth(), fullframe->GetActualHeight(), newname.get());

        int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA;
        int renderTargetFlags = CREATERENDERTARGETFLAGS_HDR;

        ITexture *texture;
        texture = g_IMaterialSystem->CreateNamedRenderTargetTextureEx(newname.get(), fullframe->GetActualWidth(), fullframe->GetActualHeight(), RT_SIZE_LITERAL, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_SEPARATE, textureFlags, renderTargetFlags);
        buffers[i].Init(texture);
    }
    return buffers[i];
}

static ShaderStencilState_t SS_NeverSolid{};
static ShaderStencilState_t SS_SolidInvisible{};
static ShaderStencilState_t SS_Null{};
static ShaderStencilState_t SS_Drawing{};

CatCommand fix_black_glow("fix_black_glow", "Fix Black Glow",
                          []()
                          {
                              effect_glow::g_EffectGlow.Shutdown();
                              effect_glow::g_EffectGlow.Init();
                          });

void EffectGlow::Init()
{
#if !ENFORCE_STREAM_SAFETY
    if (init)
        return;
    logging::Info("Init Glow...");
    {
        auto *kv = new KeyValues("UnlitGeneric");
        kv->SetString("$basetexture", "vgui/white_additive");
        kv->SetInt("$ignorez", 0);
        mat_unlit.Init("__cathook_glow_unlit", kv);
    }
    {
        auto *kv = new KeyValues("UnlitGeneric");
        kv->SetString("$basetexture", "vgui/white_additive");
        kv->SetInt("$ignorez", 1);
        mat_unlit_z.Init("__cathook_glow_unlit_z", kv);
    }
    // Initialize 2 buffers
    GetBuffer(1);
    GetBuffer(2);
    {
        auto *kv = new KeyValues("UnlitGeneric");
        kv->SetString("$basetexture", "_cathook_buff1");
        kv->SetInt("$additive", 1);
        mat_blit.Init("__cathook_glow_blit", TEXTURE_GROUP_CLIENT_EFFECTS, kv);
        mat_blit->Refresh();
    }
    {
        auto *kv = new KeyValues("BlurFilterX");
        kv->SetString("$basetexture", "_cathook_buff1");
        kv->SetInt("$ignorez", 1);
        kv->SetInt("$translucent", 1);
        kv->SetInt("$alphatest", 1);
        mat_blur_x.Init("__cathook_glow_blur_x", kv);
    }
    {
        auto *kv = new KeyValues("BlurFilterY");
        kv->SetString("$basetexture", "_cathook_buff2");
        kv->SetInt("$ignorez", 1);
        kv->SetInt("$translucent", 1);
        kv->SetInt("$alphatest", 1);
        mat_blur_y.Init("__cathook_glow_blur_y", kv);
    }
    init = true;
#endif
}

void EffectGlow::Shutdown()
{
    if (!init)
        return;
#if !ENFORCE_STREAM_SAFETY
    logging::Info("Shutdown Glow...");
    mat_blur_y.Shutdown();
    mat_blur_x.Shutdown();
    mat_blit.Shutdown();
    mat_unlit_z.Shutdown();
    mat_unlit.Shutdown();
    // Clear the buffers
    for (int i = 0; i < 4; ++i)
    {
        if (buffers[i])
        {
            buffers[i]->SetTextureRenderTarget(nullptr);
            buffers[i]->DecrementReferenceCount();
            buffers[i].Shutdown();
        }
    }
    init = false;
#endif
}

bool EffectGlow::ShouldRenderGlow(IClientEntity *pEntity)
{
    if (pEntity == nullptr || !pEntity->IsPlayer() || pEntity->IsDormant())
        return false;

    C_BasePlayer *player = reinterpret_cast<C_BasePlayer *>(pEntity);
    if (player->IsLocalPlayer())
        return false;

    if (pEntity->IsAlive() && player->GetTeam() != g_LocalPlayer->GetTeam())
        return true;

    return false;
}

void EffectGlow::RenderGlow(IClientEntity *pEntity)
{
#if !ENFORCE_STREAM_SAFETY
    if (pEntity->IsAlive() && !pEntity->IsDormant())
    {
        float r, g, b;
        if (health)
        {
            r = (1.0f - pEntity->GetHealth() / 100.0f) * 1.0f;
            g = pEntity->GetHealth() / 100.0f * 1.0f;
            b = 0.0f;
        }
        else
        {
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
        }
        if (aimbot_color && Aimbot::validAimbotTarget(pEntity) && Aimbot::canAimAt(pEntity))
        {
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
        }
        if (pEntity->GetTeam() == g_LocalPlayer->GetTeam() && !teammates)
            return;
        if (pEntity->IsDisguised() && !disguised)
            return;
        if (!players && pEntity->IsPlayer())
            return;
        if (!medkits && pEntity->IsHealthPack())
            return;
        if (!ammobox && pEntity->IsAmmoPack())
            return;
        if (!buildings && pEntity->IsBuilding())
            return;
        if (!stickies && pEntity->IsStickieBomb())
            return;
        if (!teammate_buildings && pEntity->IsTeammateBuilding())
            return;
        if (!show_powerups && pEntity->IsPowerup())
            return;
        if (!pEntity->IsVisibleTo(g_LocalPlayer))
            return;

        int r, g, b;
        r = g_Settings.esp_glow_color_r;
        g = g_Settings.esp_glow_color_g;
        b = g_Settings.esp_glow_color_b;

        float fAlpha = 1.0f;

        g_RenderGlowObject.SetAlpha(fAlpha);
        g_RenderGlowObject.SetColor(r, g, b);

        if (pEntity->GetClassId() == ClassId::CTFMedigun)
        {
            g_RenderGlowObject.SetAlpha(0.5f);
            g_RenderGlowObject.SetColor(255, 0, 0);
        }

        g_RenderGlowObject.RenderGlow(pEntity);
    }
#endif
}

void EffectGlow::DrawToStencil(IClientEntity *pEntity)
{
#if !ENFORCE_STREAM_SAFETY
    g_RenderGlowObject.RenderGlowToStencil(pEntity);
#endif
}

void EffectGlow::BeginRenderGlow()
{
#if !ENFORCE_STREAM_SAFETY
    CMatRenderContextPtr ptr(GET_RENDER_CONTEXT);
    // Save the current stencil state
    ptr->GetStencilState(&stencil_state);
    SS_NeverSolid.SetStencilState(ptr); // Set stencil state to SS_NeverSolid
    // Set the depth and color write masks to false to avoid writing to the depth buffer and color buffer
    ptr->SetDepthWriteEnable(false);
    ptr->SetColorWriteEnable(false, false, false, false);
#endif
}

void EffectGlow::EndRenderGlow()
{
#if !ENFORCE_STREAM_SAFETY
    CMatRenderContextPtr ptr(GET_RENDER_CONTEXT);
    ptr->SetStencilState(&stencil_state); // Restore the original stencil state
    ptr->SetDepthWriteEnable(true);
    ptr->SetColorWriteEnable(true, true, true, true);
#endif
}

void EffectGlow::StartStenciling()
{
#if !ENFORCE_STREAM_SAFETY
    CMatRenderContextPtr ptr(GET_RENDER_CONTEXT);
    // Save the current stencil state
    ptr->GetStencilState(&stencil_state);
    SS_SolidInvisible.SetStencilState(ptr); // Set stencil state to SS_SolidInvisible
#endif
}

void EffectGlow::EndStenciling()
{/home/boat/Desktop/cathook/src/visual/EffectGlow.cpp: In member function ‘virtual void effect_glow::EffectGlow::Render(int, int, int, int)’:
/home/boat/Desktop/cathook/src/visual/EffectGlow.cpp:84:25: error: ‘GetBuffer’ was not declared in this scope; did you mean ‘setbuffer’?
   84 |     ITexture *buffer2 = GetBuffer(2); // Add this line to declare 'buffer2'
      |                         ^~~~~~~~~
      |                         setbuffer
/home/boat/Desktop/cathook/src/visual/EffectGlow.cpp:98:9: error: ‘SS_Drawing’ was not declared in this scope; did you mean ‘drawing’?
   98 |         SS_Drawing.SetStencilState(ptr);
      |         ^~~~~~~~~~
      |         drawing
/home/boat/Desktop/cathook/src/visual/EffectGlow.cpp:101:9: error: ‘SS_Null’ was not declared in this scope
  101 |         SS_Null.SetStencilState(ptr);

#if !ENFORCE_STREAM_SAFETY
    CMatRenderContextPtr ptr(GET_RENDER_CONTEXT);
    ptr->SetStencilState(&stencil_state); // Restore the original stencil state
#endif
}

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
    ITexture *buffer1 = GetBuffer(1);
    ITexture *buffer2 = GetBuffer(2); // Add this line to declare 'buffer2'
    ptr->CopyRenderTargetToTexture(buffer1); // Copy the contents of the frame buffer to buffer1

    ShaderStencilState_t SS_Null{};
    ptr->SetStencilState(&SS_Null); // Disable the stencil test

    int width = w / *blur_scale;
    int height = h / *blur_scale;

    mat_blur_x->Refresh();
    mat_blur_y->Refresh();

    IMaterial *m = mat_blur_x;
    ptr->DrawScreenSpaceRectangle(m, 0, 0, w, h, x, y, x + w, y + h, w, h);

    m = mat_blur_y;
    ptr->DrawScreenSpaceRectangle(m, 0, 0, w, h, x, y, x + w, y + h, w, h);

    m = mat_blit;
    ptr->DrawScreenSpaceRectangle(m, 0, 0, w, h, x, y, x + w, y + h, w, h);

    ptr->SetStencilState(&stencil_state); // Restore the original stencil state

    ptr->SetRenderTarget(orig); // Restore the original render target
    ptr->CopyRenderTargetToTexture(buffer2); // Copy the contents of the frame buffer to buffer2

    if (*blur_scale != 1)
    {
        ptr->DrawScreenSpaceRectangle(mat_blit, 0, 0, w, h, x, y, x + w, y + h, w, h);
    }

    CTextureReference blurRef;
    blurRef.Init(buffer2);
    if (blurRef.IsValid())
    {
        ptr->SetRenderTarget(orig);
        IMaterial *mat = materials->FindMaterial("dev/glow_blur_x", TEXTURE_GROUP_OTHER, true);
        if (!mat)
            return;
        mat->Refresh();
        ptr->DrawScreenSpaceRectangle(mat, 0, 0, w, h, x, y, x + w, y + h, w, h);
    }
#endif
}

EffectGlow g_EffectGlow;
