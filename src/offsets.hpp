#pragma once
#include <cstddef>

// PlayerController (TypeDefIndex 2782)
namespace Off {
    constexpr size_t Team           = 0x59;
    constexpr size_t AimController  = 0x60;
    constexpr size_t Weaponry       = 0x68;
    constexpr size_t HitController  = 0x88;
    constexpr size_t MainCamera     = 0xC0;
    constexpr size_t FpsCamera      = 0xC8;
    constexpr size_t PhotonView     = 0x120;
    constexpr size_t PhotonPlayer   = 0x130;

    // PlayerManager (2794)
    constexpr size_t PM_Dict        = 0x20;
    constexpr size_t PM_HashSet     = 0x28;
    constexpr size_t PM_LocalA      = 0x68;
    constexpr size_t PM_LocalB      = 0x70;

    // AimController (2930)
    constexpr size_t SensX          = 0x50;
    constexpr size_t SensY          = 0x54;
    constexpr size_t FpsCamTr       = 0x70;
    constexpr size_t CamTransform   = 0x80;
    constexpr size_t AimParams      = 0x88;

    // HitController base (FloatValue Health often @ 0x18 in derived)
    constexpr size_t HealthFloat    = 0x18;
}
