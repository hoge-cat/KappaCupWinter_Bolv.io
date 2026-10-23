
#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// UpdateMachineGun
//
// Lv45 Sprayerの外側砲身と同じサイズにする。
// ============================================================

void TankVisual::UpdateMachineGun()
{
    // ========================================================
    // Machine Gun砲身
    //
    // 横幅：1.35倍
    // 高さ：1.35倍
    // 長さ：1.00倍
    // ========================================================

    const Vector3 machineGunScale(
        1.35f * m_levelScale,
        1.35f * m_levelScale,
        1.00f * m_levelScale
    );

    m_machineGunBarrelRender.SetScale(
        machineGunScale
    );
}


// ============================================================
// RenderMachineGun
// ============================================================

void TankVisual::RenderMachineGun(
    RenderContext& rc
)
{
    m_machineGunBarrelRender.Draw(
        rc
    );
}
