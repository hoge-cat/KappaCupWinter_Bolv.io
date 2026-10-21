#include "stdafx.h"
#include "TankWeapon.h"

#include <cstdlib>


namespace
{
    // ============================================
    // Machine Gun
    // ============================================

    // Basicより高速連射。
    const float MACHINE_GUN_RELOAD_MULTIPLIER =
        0.45f;


    // 最大±6度ばらける。
    const float MACHINE_GUN_SPREAD_DEG =
        6.0f;
}


// ============================================================
// FireMachineGun
// ============================================================

void TankWeapon::FireMachineGun(
    const Vector3& playerPosition,
    const Vector3& aimDirection)
{
    const float random01 =
        static_cast<float>(
            std::rand()
            ) /
        static_cast<float>(
            RAND_MAX
            );


    const float spreadAngle =
        (
            random01 *
            2.0f -
            1.0f
            ) *
        MACHINE_GUN_SPREAD_DEG;


    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        spreadAngle
    );


    // Twinから切り替えた場合のために初期化。
    m_twinFireLeft =
        true;
}


// ============================================================
// GetMachineGunReloadMultiplier
// ============================================================

float TankWeapon::GetMachineGunReloadMultiplier() const
{
    return MACHINE_GUN_RELOAD_MULTIPLIER;
}