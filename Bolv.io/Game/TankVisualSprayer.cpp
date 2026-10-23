
#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// UpdateSprayer
//
// 外側：Machine Gunの台形砲身
// 内側：少し太く、長くした通常砲身
// ============================================================

void TankVisual::UpdateSprayer(
    const Vector3& direction
)
{
    // ========================================================
    // 共通回転
    // ========================================================

    const Quaternion barrelRotation =
        CreateBarrelRotation(
            direction
        );


    // ========================================================
    // 外側：Machine Gun砲身（変更なし）
    // ========================================================

    const Vector3 outerBarrelScale(
        1.35f * m_levelScale,
        1.35f * m_levelScale,
        1.00f * m_levelScale
    );

    m_machineGunBarrelRender.SetRotation(
        barrelRotation
    );

    m_machineGunBarrelRender.SetScale(
        outerBarrelScale
    );


    // ========================================================
    // 内側：通常砲身
    //
    // 太さ：0.85 → 0.95
    // 長さ：1.10 → 1.20
    // ========================================================

    const Vector3 innerBarrelScale(
        0.95f * m_levelScale,
        0.95f * m_levelScale,
        1.20f * m_levelScale
    );

    m_barrelRender1.SetRotation(
        barrelRotation
    );

    m_barrelRender1.SetScale(
        innerBarrelScale
    );
}


// ============================================================
// RenderSprayer
// ============================================================

void TankVisual::RenderSprayer(
    RenderContext& rc
)
{
    // 外側：Machine Gun砲身
    m_machineGunBarrelRender.Draw(
        rc
    );

    // 内側：通常砲身
    m_barrelRender1.Draw(
        rc
    );
}
