#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ========================================================
    // Hunter
    //
    // 本家同様、2本の砲身を横に並べず
    // 同じ中心線上で重ねる。
    //
    // 外側：
    // 少し太く、少し短い
    //
    // 内側：
    // 少し細く、少し長い
    // ========================================================

    const float HUNTER_OUTER_BARREL_WIDTH_SCALE =
        1.18f;


    const float HUNTER_OUTER_BARREL_LENGTH_SCALE =
        1.02f;


    const float HUNTER_INNER_BARREL_WIDTH_SCALE =
        0.72f;


    const float HUNTER_INNER_BARREL_LENGTH_SCALE =
        1.18f;
}


// ============================================================
// UpdateHunter
// ============================================================

void TankVisual::UpdateHunter(
    const Vector3& modelPosition,
    const Vector3& right
)
{
    // rightは今回は使わない。
    // Hunterは左右にずらさず、同軸上に重ねる。


    // ========================================================
    // Position
    //
    // 2本とも完全に同じ中心位置。
    // ========================================================

    m_barrelRender1.SetPosition(
        modelPosition
    );


    m_barrelRender2.SetPosition(
        modelPosition
    );


    // ========================================================
    // 外側の太い砲身
    // ========================================================

    m_barrelRender1.SetScale(
        Vector3(
            HUNTER_OUTER_BARREL_WIDTH_SCALE *
            m_levelScale,

            HUNTER_OUTER_BARREL_WIDTH_SCALE *
            m_levelScale,

            HUNTER_OUTER_BARREL_LENGTH_SCALE *
            m_levelScale
        )
    );


    // ========================================================
    // 内側の細長い砲身
    // ========================================================

    m_barrelRender2.SetScale(
        Vector3(
            HUNTER_INNER_BARREL_WIDTH_SCALE *
            m_levelScale,

            HUNTER_INNER_BARREL_WIDTH_SCALE *
            m_levelScale,

            HUNTER_INNER_BARREL_LENGTH_SCALE *
            m_levelScale
        )
    );
}


// ============================================================
// RenderHunter
// ============================================================

void TankVisual::RenderHunter(
    RenderContext& rc
)
{
    // ============================================
    // 太い砲身を先に描画
    // ============================================

    m_barrelRender1.Draw(
        rc
    );


    // ============================================
    // 細い砲身を上から描画
    // ============================================

    m_barrelRender2.Draw(
        rc
    );
}