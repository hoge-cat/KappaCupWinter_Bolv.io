#include "stdafx.h"
#include "TankWeapon.h"


// ============================================================
// FireBasic
// ============================================================

void TankWeapon::FireBasic(
    const Vector3& playerPosition,
    const Vector3& aimDirection)
{
    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        0.0f
    );


    // Twinから別クラスへ切り替えた場合のために初期化。
    m_twinFireLeft =
        true;
}