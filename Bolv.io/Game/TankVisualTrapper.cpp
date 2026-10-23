#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ========================================================
    // Trapper砲身の調整
    // ========================================================

    // 砲身の横幅・高さ
    const float TRAPPER_BARREL_WIDTH_SCALE =
        1.25f;


    // 砲身の長さ
    const float TRAPPER_BARREL_LENGTH_SCALE =
        0.95f;


    // 砲身を前方に移動する距離
    const float TRAPPER_FORWARD_OFFSET =
        2.0f;
}


// ============================================================
// UpdateTrapper
// ============================================================

void TankVisual::UpdateTrapper(
    const Vector3& modelPosition,
    const Vector3& direction
)
{
    // ========================================================
    // 砲身の位置
    // ========================================================

    const Vector3 barrelPosition =
        modelPosition +
        direction *
        (
            TRAPPER_FORWARD_OFFSET *
            m_levelScale
            );


    m_trapperBarrelRender.SetPosition(
        barrelPosition
    );


    // ========================================================
    // 砲身の大きさ
    // ========================================================

    const Vector3 trapperBarrelScale(
        TRAPPER_BARREL_WIDTH_SCALE *
        m_levelScale,

        TRAPPER_BARREL_WIDTH_SCALE *
        m_levelScale,

        TRAPPER_BARREL_LENGTH_SCALE *
        m_levelScale
    );


    m_trapperBarrelRender.SetScale(
        trapperBarrelScale
    );
}


// ============================================================
// RenderTrapper
// ============================================================

void TankVisual::RenderTrapper(
    RenderContext& rc
)
{
    m_trapperBarrelRender.Draw(
        rc
    );
}