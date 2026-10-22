#include "stdafx.h"
#include "TankVisual.h"

#include <cmath>


namespace
{
    // ========================================================
    // Triple Shot左右砲身角度
    //
    // Weapon側と同じ値。
    // ========================================================

    const float TRIPLE_SHOT_SIDE_ANGLE =
        35.0f;


    const float PI =
        3.14159265f;
}


// ============================================================
// UpdateTripleShot
// ============================================================

void TankVisual::UpdateTripleShot(
    const Vector3& inputDirection
)
{
    Vector3 direction =
        inputDirection;


    direction.y =
        0.0f;


    const float lengthSquared =
        direction.x *
        direction.x +
        direction.z *
        direction.z;


    if (lengthSquared <=
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
    // 中央砲身
    // ========================================================

    m_barrelRender1
        .SetRotation(
            CreateBarrelRotation(
                direction
            )
        );


    // ========================================================
    // 左右方向作成
    // ========================================================

    const float angleRad =
        TRIPLE_SHOT_SIDE_ANGLE *
        PI /
        180.0f;


    const float cosValue =
        std::cos(
            angleRad
        );


    const float sinValue =
        std::sin(
            angleRad
        );


    // ========================================================
    // 左砲身
    // ========================================================

    Vector3 leftDirection;


    leftDirection.x =
        direction.x *
        cosValue -
        direction.z *
        sinValue;


    leftDirection.y =
        0.0f;


    leftDirection.z =
        direction.x *
        sinValue +
        direction.z *
        cosValue;


    leftDirection.Normalize();


    m_barrelRender2
        .SetRotation(
            CreateBarrelRotation(
                leftDirection
            )
        );


    // ========================================================
    // 右砲身
    // ========================================================

    Vector3 rightDirection;


    rightDirection.x =
        direction.x *
        cosValue +
        direction.z *
        sinValue;


    rightDirection.y =
        0.0f;


    rightDirection.z =
        -direction.x *
        sinValue +
        direction.z *
        cosValue;


    rightDirection.Normalize();


    m_barrelRender3
        .SetRotation(
            CreateBarrelRotation(
                rightDirection
            )
        );


    // ========================================================
    // Scale
    //
    // 3本とも通常砲身。
    // ========================================================

    const Vector3 barrelScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_barrelRender1
        .SetScale(
            barrelScale
        );


    m_barrelRender2
        .SetScale(
            barrelScale
        );


    m_barrelRender3
        .SetScale(
            barrelScale
        );
}


// ============================================================
// RenderTripleShot
// ============================================================

void TankVisual::RenderTripleShot(
    RenderContext& rc
)
{
    // ============================================
    // 中央
    // ============================================

    m_barrelRender1.Draw(
        rc
    );


    // ============================================
    // 左
    // ============================================

    m_barrelRender2.Draw(
        rc
    );


    // ============================================
    // 右
    // ============================================

    m_barrelRender3.Draw(
        rc
    );
}