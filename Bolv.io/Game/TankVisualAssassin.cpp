#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ============================================
    // Assassin
    //
    // Sniperより細く、
    // さらに長い砲身。
    // ============================================

    const float ASSASSIN_BARREL_WIDTH_SCALE =
        0.84f;


    const float ASSASSIN_BARREL_LENGTH_SCALE =
        1.32f;
}


// ============================================================
// UpdateAssassin
// ============================================================

void TankVisual::UpdateAssassin()
{
    const Vector3 assassinBarrelScale(
        ASSASSIN_BARREL_WIDTH_SCALE *
        m_levelScale,

        ASSASSIN_BARREL_WIDTH_SCALE *
        m_levelScale,

        ASSASSIN_BARREL_LENGTH_SCALE *
        m_levelScale
    );


    m_barrelRender1.SetScale(
        assassinBarrelScale
    );
}


// ============================================================
// RenderAssassin
// ============================================================

void TankVisual::RenderAssassin(
    RenderContext& rc
)
{
    m_barrelRender1.Draw(
        rc
    );
}