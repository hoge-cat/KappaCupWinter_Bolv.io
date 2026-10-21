#include "stdafx.h"
#include "TankWeapon.h"


// ============================================================
// FireFlankGuard
// ============================================================

void TankWeapon::FireFlankGuard(
    const Vector3& playerPosition,
    const Vector3& aimDirection)
{
    // ============================================
    // 前方へ発射
    // ============================================

    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        0.0f
    );


    // ============================================
    // 後方向を作る
    // ============================================

    Vector3 backwardDirection =
        aimDirection *
        -1.0f;


    backwardDirection.y =
        0.0f;


    // ============================================
    // 後方へ発射
    // ============================================

    Fire(
        playerPosition,
        backwardDirection,
        0.0f,
        0.0f
    );


    // Twinから切り替えた場合のために初期化。
    m_twinFireLeft =
        true;
}