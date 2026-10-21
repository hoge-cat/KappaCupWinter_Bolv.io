#include "stdafx.h"
#include "TankVisual.h"

#include <cmath>


namespace
{
    const float PI =
        3.14159265f;


    // ============================================
    // Sprayer
    // ============================================

    const float SPRAYER_ANGLE_1 =
        -14.0f;

    const float SPRAYER_ANGLE_2 =
        -7.0f;

    const float SPRAYER_ANGLE_3 =
        0.0f;

    const float SPRAYER_ANGLE_4 =
        7.0f;

    const float SPRAYER_ANGLE_5 =
        14.0f;
}


// ============================================================
// UpdateSprayer
// ============================================================

void TankVisual::UpdateSprayer(
    const Vector3& direction)
{
    // ============================================
    // 5本の角度
    // ============================================

    const float angles[5] =
    {
        SPRAYER_ANGLE_1,
        SPRAYER_ANGLE_2,
        SPRAYER_ANGLE_3,
        SPRAYER_ANGLE_4,
        SPRAYER_ANGLE_5
    };


    ModelRender* barrels[5] =
    {
        &m_barrelRender1,
        &m_barrelRender2,
        &m_barrelRender3,
        &m_barrelRender4,
        &m_barrelRender5
    };


    // ============================================
    // 各砲身を扇状にする
    // ============================================

    for (int i = 0;
        i < 5;
        ++i)
    {
        const float angleRad =
            angles[i] *
            PI /
            180.0f;


        Vector3 barrelDirection;


        barrelDirection.x =
            direction.x *
            std::cos(
                angleRad
            ) +
            direction.z *
            std::sin(
                angleRad
            );


        barrelDirection.y =
            0.0f;


        barrelDirection.z =
            -direction.x *
            std::sin(
                angleRad
            ) +
            direction.z *
            std::cos(
                angleRad
            );


        barrelDirection.Normalize();


        // ========================================
        // 共通関数で砲身回転を作る
        // ========================================

        const Quaternion barrelRotation =
            CreateBarrelRotation(
                barrelDirection
            );


        barrels[i]
            ->SetRotation(
                barrelRotation
            );
    }


    // ============================================
    // Sprayer砲身サイズ
    // ============================================

    const Vector3 sprayerScale(
        0.72f *
        m_levelScale,

        0.72f *
        m_levelScale,

        1.0f *
        m_levelScale
    );


    m_barrelRender1.SetScale(
        sprayerScale
    );


    m_barrelRender2.SetScale(
        sprayerScale
    );


    m_barrelRender3.SetScale(
        sprayerScale
    );


    m_barrelRender4.SetScale(
        sprayerScale
    );


    m_barrelRender5.SetScale(
        sprayerScale
    );
}


// ============================================================
// RenderSprayer
// ============================================================

void TankVisual::RenderSprayer(
    RenderContext& rc)
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


    m_barrelRender5.Draw(
        rc
    );
}