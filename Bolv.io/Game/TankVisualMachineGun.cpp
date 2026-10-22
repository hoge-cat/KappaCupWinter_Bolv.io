#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// UpdateMachineGun
// ============================================================

void TankVisual::UpdateMachineGun()
{
    // 3ds Max側ですでに
    // Machine Gun用の台形砲身になっているため、
    // ここではレベル倍率だけ適用する。

    const Vector3 machineGunScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_machineGunBarrelRender.SetScale(
        machineGunScale
    );
}


// ============================================================
// RenderMachineGun
// ============================================================

void TankVisual::RenderMachineGun(
    RenderContext& rc)
{
    m_machineGunBarrelRender.Draw(
        rc
    );
}