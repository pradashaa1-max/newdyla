#include "aimbot.hpp"
#include "player.hpp"
#include "config.hpp"
#include "il2cpp.hpp"
#include "offsets.hpp"

namespace Aim {
Vec3  silent_dir{};
bool  has_silent = false;

void Update() {
    has_silent = false;
    if (!g_cfg.aimbot && !g_cfg.silent) return;
    if (!g_players.cam_tr) return;

    auto* t = g_players.BestTarget(g_cfg.fov.load());
    if (!t) return;

    Vec3 bone = Add(t->pos, BoneOffset(g_cfg.bone.load()));
    Vec3 dir  = Norm(Sub(bone, g_players.cam_pos));

    if (g_cfg.silent) {
        silent_dir = dir;
        has_silent = true;
    }

    // visible aim: smooth rotation write into AimController
    // (optional — enable when you want visible snap)
    if (g_cfg.aimbot && !g_cfg.silent && g_players.local) {
        void* aim = Field<void*>(g_players.local, Off::AimController);
        if (!aim) return;
        // sensitivity scale can be used for smooth factor
        // full rotation write requires set_eulerAngles / set_rotation invoke
        // left as extension point — silent path is primary
    }
}
} // namespace Aim
