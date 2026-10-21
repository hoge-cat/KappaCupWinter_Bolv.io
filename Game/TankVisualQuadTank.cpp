#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// Quad Tank
//
//     ↑
//
// ←   ●   →
//
//     ↓
//
// aimDirectionを正面として、
// 90度ずつ4本配置する。
// ============================================================

void TankVisual::UpdateQuadTank(
    const Vector3& inputDirection
)
{
    // ========================================================
    // Front
    // ========================================================

    Vector3 front =
        inputDirection;


    front.y =
        0.0f;


    const float lengthSquared =
        front.x *
        front.x +
        front.z *
        front.z;


    if (lengthSquared <=
        0.0001f)
    {
        front =
            Vector3(
                0.0f,
                0.0f,
                1.0f
            );
    }
    else
    {
        front.Normalize();
    }


    // ========================================================
    // Right
    // ========================================================

    Vector3 right(
        front.z,
        0.0f,
        -front.x
    );


    right.Normalize();


    // ========================================================
    // Back
    // ========================================================

    Vector3 back(
        -front.x,
        0.0f,
        -front.z
    );


    // ========================================================
    // Left
    // ========================================================

    Vector3 left(
        -right.x,
        0.0f,
        -right.z
    );


    // ========================================================
    // Rotation
    // ========================================================

    m_barrelRender1.SetRotation(
        CreateBarrelRotation(
            front
        )
    );


    m_barrelRender2.SetRotation(
        CreateBarrelRotation(
            right
        )
    );


    m_barrelRender3.SetRotation(
        CreateBarrelRotation(
            back
        )
    );


    m_barrelRender4.SetRotation(
        CreateBarrelRotation(
            left
        )
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
// RenderQuadTank
// ============================================================

void TankVisual::RenderQuadTank(
    RenderContext& rc
)
{
    m_barrelRender1.Draw(
        rc
    );


    m_barrelRender2.Draw(
        rc
    );


    m_barrelRender3.Draw(
        rc
    );


    m_barrelRender4.Draw(
        rc
    );
}