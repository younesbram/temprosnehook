/*
 * entitycache.cpp
 *
 * Rewritten on: September 9th, 2023
 *      Author: rosne-gamingyt
*/

#include "common.hpp"
#include <settings/Float.hpp>
#include "soundcache.hpp"
#include <memory>

class CachedEntity {
public:
    CachedEntity(int idx);
    ~CachedEntity();
    void Update();
    bool IsVisible();

private:
    int m_IDX;
    Entity* m_pEntity;
    bool m_bVisCheckComplete = false;
    bool m_bAnyHitboxVisible = false;
    HitboxCache hitboxes;

    std::unique_ptr<player_info_s> player_info;
};

CachedEntity::CachedEntity(int idx) : m_IDX(idx), hitboxes(hitbox_cache::EntityHitboxCache{idx}) {
    m_pEntity = nullptr;
    player_info = nullptr;
}

CachedEntity::~CachedEntity() {
    // Release player_info memory if allocated
    player_info.reset();
}

void CachedEntity::Update() {
    m_pEntity = g_IEntityList->GetClientEntity(idx);
    if (!m_pEntity)
        return;
    
    hitboxes.InvalidateCache();
    m_bVisCheckComplete = false;
}

bool CachedEntity::IsVisible() {
    PROF_SECTION(CE_IsVisible)
    if (m_bVisCheckComplete)
        return m_bAnyHitboxVisible;

    auto hitbox = hitboxes.GetHitbox(std::max(0, (hitboxes.GetNumHitboxes() >> 1) - 1));
    Vector result;
    if (!hitbox)
        result = m_vecOrigin();
    else
        result = hitbox->center;

    if (IsEntityVectorVisible(this, result, true, MASK_SHOT_HULL, nullptr, true)) {
        m_bAnyHitboxVisible = true;
        m_bVisCheckComplete = true;
        return true;
    }

    m_bAnyHitboxVisible = false;
    m_bVisCheckComplete = true;

    return false;
}

namespace entity_cache
{
boost::unordered_flat_map<int, CachedEntity> array;
std::vector<CachedEntity*> valid_ents;
std::vector<CachedEntity*> player_cache;
int previous_max = 0;
int previous_ent = 0;
int max = 1;

void Update()
{
    max = g_IEntityList->GetHighestEntityIndex();
    int current_ents = g_IEntityList->NumberOfEntities(false);
    valid_ents.clear();
    player_cache.clear();

    if (g_Settings.bInvalid)
        return;

    if (max >= MAX_ENTITIES)
        max = MAX_ENTITIES - 1;

    valid_ents.reserve(max);
    player_cache.reserve(g_GlobalVars->maxClients);

    if (previous_max == max && previous_ent == current_ents) {
        for (auto &[key, val] : array) {
            val.Update();
            auto internal_entity = val.InternalEntity();
            if (internal_entity && !internal_entity->IsDormant()) {
                valid_ents.emplace_back(&val);
                auto val_type = val.m_Type();
                if (val_type == ENTITY_PLAYER || val_type == ENTITY_BUILDING || val_type == ENTITY_NPC) {
                    if (val.m_bAlivePlayer()) {
                        val.hitboxes.UpdateBones();
                        if (val_type == ENTITY_PLAYER)
                            player_cache.emplace_back(&val);
                    }
                }

                if (val_type == ENTITY_PLAYER)
                    GetPlayerInfo(val.m_IDX, val.player_info.get());
            }
        }
    }
    else {
        for (int i = 0; i <= max; ++i) {
            if (!g_IEntityList->GetClientEntity(i) || !g_IEntityList->GetClientEntity(i)->GetClientClass()->m_ClassID)
                continue;
            CachedEntity &ent = array.try_emplace(i, CachedEntity{i}).first->second;
            ent.Update();
            auto internal_entity = ent.InternalEntity();
            if (internal_entity) {
                auto ent_type = ent.m_Type();
                if (!internal_entity->IsDormant()) {
                    valid_ents.emplace_back(&ent);
                    if (ent_type == ENTITY_PLAYER || ent_type == ENTITY_BUILDING || ent_type == ENTITY_NPC) {
                        if (ent.m_bAlivePlayer()) {
                            ent.hitboxes.UpdateBones();
                            if (ent_type == ENTITY_PLAYER)
                                player_cache.emplace_back(&ent);
                        }
                    }
                }

                if (ent_type == ENTITY_PLAYER) {
                    if (!ent.player_info)
                        ent.player_info = std::make_unique<player_info_s>();
                    GetPlayerInfo(ent.m_IDX, ent.player_info.get());
                }
            }
        }
    }
    previous_max = max;
    previous_ent = current_ents;
}

void Invalidate()
{
    array.clear();
}

void Shutdown()
{
    array.clear();
    previous_max = 0;
    max = -1;
}
} // namespace entity_cache