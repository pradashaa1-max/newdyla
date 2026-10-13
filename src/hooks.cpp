#include "hooks.hpp"
#include "il2cpp.hpp"
#include "player.hpp"
#include "aimbot.hpp"
#include "esp.hpp"
#include "menu.hpp"
#include "config.hpp"
#include "offsets.hpp"
#include <vector>

// simple fishhook-style / substrate placeholder
// on iOS use fishhook or Dobby / Substrate
// here we expose the frame entry; real method hooks go below

// ─── Player list maintenance ─────────────────────────────────────────────────
// Call RegisterPlayer(obj) from a hooked PlayerController.OnEnable / constructor
// Call UnregisterPlayer(obj) from OnDisable / Destroy

extern "C" void RegisterPlayer(void* obj) {
    if (!obj) return;
    std::lock_guard<std::mutex> lk(g_players.mtx);
    for (auto& s : g_players.list)
        if (s.obj == obj) return;
    PlayerSnap snap;
    snap.obj = obj;
    g_players.list.push_back(snap);
}

extern "C" void UnregisterPlayer(void* obj) {
    std::lock_guard<std::mutex> lk(g_players.mtx);
    g_players.list.erase(
        std::remove_if(g_players.list.begin(), g_players.list.end(),
            [obj](const PlayerSnap& s){ return s.obj == obj; }),
        g_players.list.end());
}

// ─── Silent aim: call before bullet direction is finalised ───────────────────
// Hook the method that builds shoot ray / bullet velocity and replace direction
extern "C" bool GetSilentDir(float* x, float* y, float* z) {
    if (!Aim::has_silent) return false;
    *x = Aim::silent_dir.x;
    *y = Aim::silent_dir.y;
    *z = Aim::silent_dir.z;
    return true;
}

void Hooks::Install() {
    // example (pseudo):
    // MSHookFunction(PlayerController_OnEnable, hooked, &orig);
    // MSHookFunction(Weaponry_Fire, hooked_fire, &orig_fire);
    // real addresses: resolve via class_get_method_from_name + method pointer
    // or use the RVAs from dump.cs (VA column) + slide
}

void Hooks::OnFrame(float sw, float sh) {
    g_players.Collect();
    Aim::Update();
    Menu::Render(sw, sh);
    Esp::Draw(sw, sh);
}
