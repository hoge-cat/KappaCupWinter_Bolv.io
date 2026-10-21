#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ========================================================
    // Twin Flankの左右砲身位置
    // ========================================================

    const float TWIN_FLANK_BARREL_OFFSET =
        11.0f;
}


// ============================================================
// UpdateTwinFlank
//
//       前
//
//     ┃     ┃
//     ┃     ┃
//
//        ●
//
//     ┃     ┃
//     ┃     ┃
//
//       後
//
// 前後それぞれにTwinを配置する。
// ============================================================

void TankVisual::UpdateTwinFlank(
    const Vector3& modelPosition,
    const Vector3& inputDirection,
    const Vector3& inputRight
)
{
    // ========================================================
    // Direction
    // ========================================================

    Vector3 direction =
        inputDirection;


    direction.y =
        0.0f;


    const float directionLengthSquared =
        direction.x *
        direction.x +
        direction.z *
        direction.z;


    if (directionLengthSquared <=
        0.0001f)
    {
        direction =
            Vector3(
                0.0f,
                0.0f,
                1.0f
            );
    }
    else
    {
        direction.Normalize();
    }


    // ========================================================
    // Right
    // ========================================================

    Vector3 right =
        inputRight;


    right.y =
        0.0f;


    const float rightLengthSquared =
        right.x *
        right.x +
        right.z *
        right.z;


    if (rightLengthSquared >
        0.0001f)
    {
        right.Normalize();
    }
    else
    {
        right =
            Vector3(
                direction.z,
                0.0f,
                -direction.x
            );


        right.Normalize();
    }


    // ========================================================
    // Back Direction
    // ========================================================

    const Vector3 backDirection(
        -direction.x,
        0.0f,
        -direction.z
    );


    // ========================================================
    // 砲身位置
    //
    // 1 = 前左
    // 2 = 前右
    // 3 = 後左
    // 4 = 後右
    // ========================================================

    const Vector3 leftPosition =
        modelPosition -
        right *
        (
            TWIN_FLANK_BARREL_OFFSET *
            m_levelScale
            );


    const Vector3 rightPosition =
        modelPosition +
        right *
        (
            TWIN_FLANK_BARREL_OFFSET *
            m_levelScale
            );


    // ============================================
    // Front Left
    // ============================================

    m_barrelRender1.SetPosition(
        leftPosition
    );


    // ============================================
    // Front Right
    // ============================================

    m_barrelRender2.SetPosition(
        rightPosition
    );


    // ============================================
    // Back Left
    // ============================================

    m_barrelRender3.SetPosition(
        leftPosition
    );


    // ============================================
    // Back Right
    // ============================================

    m_barrelRender4.SetPosition(
        rightPosition
    );


    // ========================================================
    // Rotation
    // ========================================================

    const Quaternion frontRotation =
        CreateBarrelRotation(
            direction
        );


    const Quaternion backRotation =
        CreateBarrelRotation(
            backDirection
        );


    // ============================================
    // Front
    // ============================================

    m_barrelRender1.SetRotation(
        frontRotation
    );


    m_barrelRender2.SetRotation(
        frontRotation
    );


    // ============================================
    // Back
    // ============================================

    m_barrelRender3.SetRotation(
        backRotation
    );


    m_barrelRender4.SetRotation(
        backRotation
    );


    // ========================================================
    // Scale
    // ========================================================

    const Vector3 barrelScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_barrelRender1.SetScale(
        barrelScale
    );


    m_barrelRender2.SetScale(
        barrelScale
    );


    m_barrelRender3.SetScale(
        barrelScale
    );


    m_barrelRender4.SetScale(
        barrelScale
    );
}


// ============================================================
// RenderTwinFlank
// ============================================================

void TankVisual::RenderTwinFlank(
    RenderContext& rc
)
{
    // ============================================
    // Front Left
    // ============================================

    m_barrelRender1.Draw(
        rc
    );


    // ============================================
    // Front Right
    // ============================================

    m_barrelRender2.Draw(
        rc
    );


    // ============================================
    // Back Left
    // ============================================

    m_barrelRender3.Draw(
        rc
    );


    // ============================================
    // Back Right
    // ============================================

    m_barrelRender4.Draw(
        rc
    );
}