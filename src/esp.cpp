#include "esp.hpp"
#include "player.hpp"
#include "config.hpp"
#include "menu.hpp"   // uses DrawCmd buffer
#include "il2cpp.hpp"
#include "math.hpp"

// simple perspective projection fallback when Camera.WorldToScreenPoint
// is not yet wired — replace with real W2S when camera method is resolved
static bool WorldToScreen(const Vec3& world, float sw, float sh, Vec2& out) {
    // placeholder: assume camera looks down +Z, up +Y
    // real build: invoke Camera.WorldToScreenPoint
    Vec3 rel = Sub(world, g_players.cam_pos);
    float z = Dot(rel, g_players.cam_fwd);
    if (z < 0.1f) return false;
    // crude FOV projection
    float fov_rad = 70.f * 0.0174533f;
    float scale = (sh * 0.5f) / tanf(fov_rad * 0.5f);
    out.x = sw * 0.5f + (rel.x / z) * scale;
    out.y = sh * 0.5f - (rel.y / z) * scale;
    return out.x > -50 && out.x < sw+50 && out.y > -50 && out.y < sh+50;
}

void Esp::Draw(float sw, float sh) {
    if (!g_cfg.esp_box && !g_cfg.esp_skel && !g_cfg.esp_name && !g_cfg.esp_hp)
        return;

    std::lock_guard<std::mutex> lk(g_players.mtx);
    for (auto& s : g_players.list) {
        if (s.is_local || s.is_dead) continue;
        if (g_cfg.team_check && s.team == g_players.local_team) continue;

        Vec2 feet, head;
        if (!WorldToScreen(s.pos, sw, sh, feet)) continue;
        if (!WorldToScreen(s.head, sw, sh, head)) continue;

        float h = feet.y - head.y;
        if (h < 4.f) continue;
        float w = h * 0.45f;
        float x = head.x - w * 0.5f;
        float y = head.y;

        if (g_cfg.esp_box) {
            Menu::Rect(x, y, w, h, 0xFF00B4FF);
        }
        if (g_cfg.esp_skel) {
            // simple 3-bone line
            Menu::Line(head.x, head.y, feet.x, feet.y, 0xFF00E5FF);
        }
        if (g_cfg.esp_name) {
            Menu::Text(x, y - 14, s.name.empty() ? "player" : s.name.c_str(), 0xFFFFFFFF);
        }
        if (g_cfg.esp_hp) {
            float pct = fmaxf(0.f, fminf(1.f, s.hp / 100.f));
            Menu::Rect(x - 6, y, 3, h, 0xFF222222);
            Menu::Rect(x - 6, y + h*(1.f-pct), 3, h*pct, 0xFF00FF66);
        }
    }
}
