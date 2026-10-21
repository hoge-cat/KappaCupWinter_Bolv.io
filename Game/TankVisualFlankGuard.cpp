#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// UpdateFlankGuard
// ============================================================

void TankVisual::UpdateFlankGuard(
    const Vector3& direction)
{
    // ============================================
    // 後ろ方向
    // ============================================

    Vector3 backward =
        direction *
        -1.0f;


    backward.y =
        0.0f;


    // ============================================
    // 後方砲身の回転
    // ============================================

    const Quaternion backwardRotation =
        CreateBarrelRotation(
            backward
        );


    m_barrelRender2.SetRotation(
        backwardRotation
    );
}


// ============================================================
// RenderFlankGuard
// ============================================================

void TankVisual::RenderFlankGuard(
    RenderContext& rc)
{
    // 前方砲身。
    m_barrelRender1.Draw(
        rc
    );


    // 後方砲身。
    m_barrelRender2.Draw(
        rc
    );
}