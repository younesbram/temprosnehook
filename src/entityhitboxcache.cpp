/*
 * entityhitboxcache.cpp
 *
 *  Created on: Dec 23, 2016
 *      Author: nullifiedcat
 */

#include <settings/Int.hpp>
#include "common.hpp"
// #include "SetupBonesReconst.hpp"

namespace hitbox_cache
{
void EntityHitboxCache::Init()
{
    const model_t *model;
    studiohdr_t *shdr;
    mstudiohitboxset_t *set;
    m_bInit    = true;
    parent_ref = &entity_cache::array[hit_idx];
    if (CE_BAD(parent_ref))
        return;
    model = RAW_ENT(parent_ref)->GetModel();
    if (!model)
        return;
    if (!m_bModelSet || model != m_pLastModel)
    {
        shdr = g_IModelInfo->GetStudiomodel(model);
        if (!shdr)
            return;
        set = shdr->pHitboxSet(CE_INT(parent_ref, netvar.iHitboxSet));
        if (!set)
            return;
        m_pLastModel   = const_cast<model_t *>(model);
        m_nNumHitboxes = 0;
        m_nNumHitboxes = set->numhitboxes;

        if (m_nNumHitboxes > CACHE_MAX_HITBOXES)
            m_nNumHitboxes = CACHE_MAX_HITBOXES;
        m_bModelSet = true;
    }
    m_bSuccess = true;
}

bool EntityHitboxCache::VisibilityCheck(int id)
{
    CachedHitbox *hitbox;

    if (!m_bInit)
        Init();
    if (id < 0 || id >= m_nNumHitboxes)
        return false;
    if (!m_bSuccess)
        return false;
    if (m_VisCheckValidationFlags >> id & 1)
        return m_VisCheck >> id & 1;
    // TODO corners
    hitbox = GetHitbox(id);
    if (!hitbox)
        return false;
    bool validation = IsEntityVectorVisible(parent_ref, hitbox->center, true);
    // Bitmask works sort of like an index in our case. 1 would be the first bit, and we are shifting this by id to get our index
    uint_fast64_t mask = 1ULL << id;
    // No branch conditional set https://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
    m_VisCheck = m_VisCheck & ~mask | -validation & mask;
    m_VisCheckValidationFlags |= 1ULL << id;
    return m_VisCheck >> id & 1;
}

static settings::Int setupbones_time{ "source.setupbones-time", "2" }; // 2 will make it hit p

// STOP PUTTING USELESS SIGS EVERYWHERE FAGGOT !

matrix3x4_t *EntityHitboxCache::GetBones(int numbones)
{
    static float bones_setup_time = 0.0f;
    switch (*setupbones_time)
    {
    case 0:
        bones_setup_time = 0.0f;
        break;
    case 1:
        bones_setup_time = g_GlobalVars->curtime;
        break;
    case 2:
        if (CE_GOOD(LOCAL_E))
            bones_setup_time = SERVER_TIME;
        break;
    case 3:
        if (CE_GOOD(parent_ref))
            bones_setup_time = CE_FLOAT(parent_ref, netvar.m_flSimulationTime);
        break;
    case 4:
        bones_setup_time = g_IEngine->GetLastTimeStamp();
    }

    if (!bones_setup)
    {
        // If numbones is not set, get it from some terrible and unnamed variable
        if (numbones == -1)
        {
            if (parent_ref->m_Type() == ENTITY_PLAYER)
                numbones = CE_INT(parent_ref, 0x844);
            else
                numbones = MAXSTUDIOBONES;
        }

        if (bones.size() != (size_t) numbones)
            bones.resize(numbones);

        if (g_Settings.is_create_move)
        {
            PROF_SECTION(bone_setup)

            // ALSO, WHAT IN THE NAME OF GOD IS 0x7FF00 !?!?!?!?!?!?!?!? WHY IS THIS USED INSTEAD OF BONE_USED_BY_HITBOX ENUM BUILT INTO THE SDK >?!?!?!?!?!? !?!?!?!? !? !?!?!
            // WHATEVER, ALL (PUBLIC) CHEATS USE THIS RANDOM SIGNED BINARY PIECE OF SHIT, SO THEY CANT BE WRONG (RIGHT!?!?!?!)
            // I NEED MENTAL HELP WITH THIS FAGGOT ROSNEHOOK/CATHOOK CODE
            bones_setup = RAW_ENT(parent_ref)->SetupBones(bones.data(), numbones, BONE_USED_BY_HITBOX, bones_setup_time);
        }
    }
    return bones.data();
}

CachedHitbox *EntityHitboxCache::GetHitbox(int id)
{
    if (m_CacheValidationFlags >> id & 1)
        return &m_CacheInternal[id];
    mstudiobbox_t *box;

    if (!m_bInit)
        Init();
    if (id < 0 || id >= m_nNumHitboxes)
        return nullptr;
    if (!m_bSuccess)
        return nullptr;
    if (CE_BAD(parent_ref))
        return nullptr;
    auto model = RAW_ENT(parent_ref)->GetModel();
    if (!model)
        return nullptr;
    auto shdr = g_IModelInfo->GetStudiomodel(model);
    if (!shdr)
        return nullptr;
    auto set = shdr->pHitboxSet(CE_INT(parent_ref, netvar.iHitboxSet));
    if (!set)
        return nullptr;
    if (m_nNumHitboxes > m_CacheInternal.size())
        m_CacheInternal.resize(m_nNumHitboxes);
    box = set->pHitbox(id);
    if (!box)
        return nullptr;
    if (box->bone < 0 || box->bone >= MAXSTUDIOBONES)
        return nullptr;
    VectorTransform(box->bbmin, GetBones(shdr->numbones)[box->bone], m_CacheInternal[id].min);
    VectorTransform(box->bbmax, GetBones(shdr->numbones)[box->bone], m_CacheInternal[id].max);
    m_CacheInternal[id].bbox   = box;
    m_CacheInternal[id].center = (m_CacheInternal[id].min + m_CacheInternal[id].max) / 2;
    m_CacheValidationFlags |= 1ULL << id;
    return &m_CacheInternal[id];
}
} // namespace hitbox_cache
