#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ============================================
    // Twin
    // ============================================

    const float TWIN_BARREL_OFFSET =
        11.0f;
}


// ============================================================
// UpdateTwin
// ============================================================

void TankVisual::UpdateTwin(
    const Vector3& modelPosition,
    const Vector3& right)
{
    Vector3 leftPosition =
        modelPosition -
        right *
        (
            TWIN_BARREL_OFFSET *
            m_levelScale
            );


    Vector3 rightPosition =
        modelPosition +
        right *
        (
            TWIN_BARREL_OFFSET *
            m_levelScale
            );


    m_barrelRender1.SetPosition(
        leftPosition
    );


    m_barrelRender2.SetPosition(
        rightPosition
    );
}


// ============================================================
// RenderTwin
// ============================================================

void TankVisual::RenderTwin(
    RenderContext& rc)
{
    m_barrelRender1.Draw(
        rc
    );


    m_barrelRender2.Draw(
        rc
    );
}