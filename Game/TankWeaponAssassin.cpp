#include "stdafx.h"
#include "TankWeapon.h"


namespace
{
    // ============================================
    // Assassin
    //
    // Sniperよりさらに遠距離向け。
    //
    // 本家の方向性をBolv.ioに合わせて
    // 再現するための調整値。
    // ============================================

    const float ASSASSIN_BULLET_SPEED_MULTIPLIER =
        1.50f;


    const float ASSASSIN_RELOAD_MULTIPLIER =
        1.50f;


    const float ASSASSIN_MUZZLE_DISTANCE_MULTIPLIER =
        1.32f;
}


// ============================================================
// FireAssassin
// ============================================================

void TankWeapon::FireAssassin(
    const Vector3& playerPosition,
    const Vector3& aimDirection
)
{
    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        0.0f
    );


    m_twinFireLeft =
        true;
}


// ============================================================
// GetAssassinReloadMultiplier
// ============================================================

float TankWeapon::GetAssassinReloadMultiplier() const
{
    return
        ASSASSIN_RELOAD_MULTIPLIER;
}


// ============================================================
// GetAssassinBulletSpeedMultiplier
// ============================================================

float TankWeapon::GetAssassinBulletSpeedMultiplier() const
{
    return
        ASSASSIN_BULLET_SPEED_MULTIPLIER;
}


// ============================================================
// GetAssassinMuzzleDistanceMultiplier
// ============================================================

float TankWeapon::GetAssassinMuzzleDistanceMultiplier() const
{
    return
        ASSASSIN_MUZZLE_DISTANCE_MULTIPLIER;
}