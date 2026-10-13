#pragma once
#include "math.hpp"
#include <vector>
#include <mutex>
#include <string>

struct PlayerSnap {
    void*       obj        = nullptr;
    void*       transform  = nullptr;
    void*       aim_ctrl   = nullptr;
    Vec3        pos{};
    Vec3        head{};
    float       hp         = 100.f;
    int         team       = -1;
    bool        is_local   = false;
    bool        is_dead    = false;
    std::string name;
};

struct PlayerSystem {
    std::mutex              mtx;
    std::vector<PlayerSnap> list;
    void*                   local      = nullptr;
    Vec3                    local_pos{};
    Vec3                    cam_pos{};
    Vec3                    cam_fwd{};
    void*                   cam_tr     = nullptr;
    int                     local_team = -1;

    void Collect();
    PlayerSnap* BestTarget(float max_fov);
};

inline PlayerSystem g_players;
