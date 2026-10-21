#include "stdafx.h"
#include "TankVisual.h"
#include "system/system.h"


namespace
{
    // ============================================
    // Smasher
    // ============================================

    // 外装の回転速度。
    // 1秒間に90度回転する。
    const float SMASHER_ROTATION_SPEED =
        90.0f;
}


// ============================================================
// UpdateSmasher
// ============================================================

void TankVisual::UpdateSmasher(
    const Vector3& modelPosition)
{
    // ============================================
    // 位置
    // ============================================

    m_smasherShellRender.SetPosition(
        modelPosition
    );


    // ============================================
    // サイズ
    // ============================================

    const Vector3 shellScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_smasherShellRender.SetScale(
        shellScale
    );


    // ============================================
    // 自動回転
    // ============================================

    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    m_smasherRotationDeg +=
        SMASHER_ROTATION_SPEED *
        deltaTime;


    if (m_smasherRotationDeg >=
        360.0f)
    {
        m_smasherRotationDeg -=
            360.0f;
    }


    Quaternion shellRotation;


    shellRotation.SetRotationDeg(
        Vector3::AxisY,
        m_smasherRotationDeg
    );


    m_smasherShellRender.SetRotation(
        shellRotation
    );
}


// ============================================================
// RenderSmasher
// ============================================================

void TankVisual::RenderSmasher(
    RenderContext& rc)
{
    m_smasherShellRender.Draw(
        rc
    );
}