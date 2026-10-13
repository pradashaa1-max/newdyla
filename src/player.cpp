#include "player.hpp"
#include "il2cpp.hpp"
#include "offsets.hpp"
#include "config.hpp"
#include <algorithm>

static Il2CppClass*  s_pm_cls      = nullptr;
static Il2CppClass*  s_tr_cls      = nullptr;
static Il2CppMethod* s_get_pos     = nullptr;
static Il2CppMethod* s_get_fwd     = nullptr;
static Il2CppMethod* s_pm_instance = nullptr;
static Il2CppMethod* s_pm_local    = nullptr;
static bool          s_ready       = false;

static bool Ensure() {
    if (s_ready) return true;
    s_tr_cls = FindClass("UnityEngine", "Transform");
    s_pm_cls = FindClass("", "PlayerManager");
    if (!s_tr_cls) return false;
    s_get_pos = g_api.class_get_method_from_name(s_tr_cls, "get_position", 0);
    s_get_fwd = g_api.class_get_method_from_name(s_tr_cls, "get_forward", 0);
    if (s_pm_cls) {
        s_pm_instance = g_api.class_get_method_from_name(s_pm_cls, "get_Instance", 0);
        if (!s_pm_instance)
            s_pm_instance = g_api.class_get_method_from_name(s_pm_cls, "get_instance", 0);
        s_pm_local = g_api.class_get_method_from_name(s_pm_cls, "get_HEHGCFGHADFHHGH", 0);
    }
    s_ready = s_get_pos != nullptr;
    return s_ready;
}

static Vec3 InvokeVec3(Il2CppMethod* m, void* obj) {
    if (!m || !obj) return {};
    void* exc = nullptr;
    auto r = g_api.runtime_invoke(m, obj, nullptr, &exc);
    if (exc || !r) return {};
    return *reinterpret_cast<Vec3*>(g_api.object_unbox(r));
}

void PlayerSystem::Collect() {
    if (!Ensure()) return;

    void* pm = nullptr;
    if (s_pm_instance) {
        void* exc = nullptr;
        pm = g_api.runtime_invoke(s_pm_instance, nullptr, nullptr, &exc);
    }
    if (!pm) return;

    // local player
    void* local = nullptr;
    if (s_pm_local) {
        void* exc = nullptr;
        local = g_api.runtime_invoke(s_pm_local, pm, nullptr, &exc);
    }
    if (!local)
        local = Field<void*>(pm, Off::PM_LocalA);
    if (!local)
        local = Field<void*>(pm, Off::PM_LocalB);

    this->local = local;
    if (local) {
        local_team = (int)Field<uint8_t>(local, Off::Team);
        void* aim = Field<void*>(local, Off::AimController);
        if (aim) {
            cam_tr = Field<void*>(aim, Off::CamTransform);
            if (cam_tr) {
                cam_pos = InvokeVec3(s_get_pos, cam_tr);
                cam_fwd = InvokeVec3(s_get_fwd, cam_tr);
                local_pos = cam_pos;
            }
        }
    }

    // players from HashSet / Dict — for full list we rely on hooks.cpp
    // which pushes every spawned PlayerController into g_players.list
    // here we only refresh positions of already known objects
    std::lock_guard<std::mutex> lk(mtx);
    for (auto& s : list) {
        if (!s.obj) continue;
        s.team     = (int)Field<uint8_t>(s.obj, Off::Team);
        s.is_local = (s.obj == local);
        void* aim  = Field<void*>(s.obj, Off::AimController);
        if (aim) {
            void* tr = Field<void*>(aim, Off::CamTransform);
            if (!tr) tr = Field<void*>(aim, Off::FpsCamTr);
            if (tr) {
                s.pos  = InvokeVec3(s_get_pos, tr);
                s.head = Add(s.pos, BoneOffset(0));
            }
        }
        // health — best-effort from hit controller
        void* hit = Field<void*>(s.obj, Off::HitController);
        if (hit) {
            // FloatValue often stores float at +0x10 or +0x18 depending on layout
            s.hp = Field<float>(hit, Off::HealthFloat);
            if (s.hp < 0.f || s.hp > 200.f) s.hp = 100.f;
        }
        s.is_dead = s.hp <= 0.f;
    }
}

PlayerSnap* PlayerSystem::BestTarget(float max_fov) {
    PlayerSnap* best = nullptr;
    float best_ang = max_fov;
    std::lock_guard<std::mutex> lk(mtx);
    for (auto& s : list) {
        if (s.is_local || s.is_dead || s.hp <= 0.f) continue;
        if (g_cfg.team_check && s.team == local_team) continue;
        Vec3 bone = Add(s.pos, BoneOffset(g_cfg.bone.load()));
        float a = AngDeg(cam_pos, bone, cam_fwd);
        if (a < best_ang) {
            best_ang = a;
            best = &s;
        }
    }
    return best;
}
