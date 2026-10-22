#include "stdafx.h"
#include "TankWeapon.h"


namespace
{
    // ============================================
    // Twin
    // ============================================

    const float TWIN_BARREL_OFFSET =
        11.0f;


    const float TWIN_RELOAD_MULTIPLIER =
        0.5f;
}


// ============================================================
// FireTwin
// ============================================================

void TankWeapon::FireTwin(
    const Vector3& playerPosition,
    const Vector3& aimDirection)
{
    float sideOffset =
        TWIN_BARREL_OFFSET;


    // 左砲身から撃つ場合。
    if (m_twinFireLeft)
    {
        sideOffset =
            -TWIN_BARREL_OFFSET;
    }


    Fire(
        playerPosition,
        aimDirection,
        sideOffset,
        0.0f
    );


    // 次は反対側の砲身から撃つ。
    m_twinFireLeft =
        !m_twinFireLeft;
}


// ============================================================
// GetTwinReloadMultiplier
// ============================================================

float TankWeapon::GetTwinReloadMultiplier() const
{
    return TWIN_RELOAD_MULTIPLIER;
}