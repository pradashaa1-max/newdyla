#pragma once
#include <atomic>

struct Config {
    std::atomic<bool>  aimbot{true};
    std::atomic<bool>  silent{true};
    std::atomic<bool>  fov_circle{true};
    std::atomic<bool>  esp_box{true};
    std::atomic<bool>  esp_skel{true};
    std::atomic<bool>  esp_name{true};
    std::atomic<bool>  esp_hp{true};
    std::atomic<bool>  no_recoil{false};
    std::atomic<bool>  team_check{true};
    std::atomic<bool>  menu_open{true};
    std::atomic<float> fov{15.f};
    std::atomic<float> smooth{4.5f};
    std::atomic<int>   bone{0};
};
inline Config g_cfg;
