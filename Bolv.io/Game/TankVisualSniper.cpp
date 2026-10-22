#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ============================================
    // Sniper
    // ============================================

    // Basicより少し細い。
    const float SNIPER_BARREL_WIDTH_SCALE =
        0.90f;


    // ============================================
    // 以前：
    // 1.18
    //
    // 今回：
    // 1.12
    //
    // 「Sniperらしく少し長い」程度まで短縮。
    // ============================================

    const float SNIPER_BARREL_LENGTH_SCALE =
        1.12f;
}


// ============================================================
// UpdateSniper
// ============================================================

void TankVisual::UpdateSniper()
{
    const Vector3 sniperBarrelScale(
        SNIPER_BARREL_WIDTH_SCALE *
        m_levelScale,

        SNIPER_BARREL_WIDTH_SCALE *
        m_levelScale,

        SNIPER_BARREL_LENGTH_SCALE *
        m_levelScale
    );


    m_barrelRender1.SetScale(
        sniperBarrelScale
    );
}


// ============================================================
// RenderSniper
// ============================================================

void TankVisual::RenderSniper(
    RenderContext& rc
)
{
    m_barrelRender1.Draw(
        rc
    );
}