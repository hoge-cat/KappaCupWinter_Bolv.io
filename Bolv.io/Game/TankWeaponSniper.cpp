#include "stdafx.h"
#include "TankWeapon.h"


namespace
{
    // ============================================
    // Sniper
    // ============================================

    // Basicより弾速を上げる。
    const float SNIPER_BULLET_SPEED_MULTIPLIER =
        1.35f;


    // Basicより発射間隔を少し長くする。
    const float SNIPER_RELOAD_MULTIPLIER =
        1.35f;


    // ============================================
    // 砲身を以前より短くしたので、
    // 弾の発射位置も合わせて短くする。
    //
    // 以前：
    // 1.35
    //
    // 今回：
    // 1.15
    // ============================================

    const float SNIPER_MUZZLE_DISTANCE_MULTIPLIER =
        1.15f;
}


// ============================================================
// FireSniper
// ============================================================

void TankWeapon::FireSniper(
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


    // Twin系から切り替えた場合のために初期化。
    m_twinFireLeft =
        true;
}


// ============================================================
// GetSniperReloadMultiplier
// ============================================================

float TankWeapon::GetSniperReloadMultiplier() const
{
    return
        SNIPER_RELOAD_MULTIPLIER;
}


// ============================================================
// GetSniperBulletSpeedMultiplier
// ============================================================

float TankWeapon::GetSniperBulletSpeedMultiplier() const
{
    return
        SNIPER_BULLET_SPEED_MULTIPLIER;
}


// ============================================================
// GetSniperMuzzleDistanceMultiplier
// ============================================================

float TankWeapon::GetSniperMuzzleDistanceMultiplier() const
{
    return
        SNIPER_MUZZLE_DISTANCE_MULTIPLIER;
}