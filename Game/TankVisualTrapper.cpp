#include "stdafx.h"
#include "TankVisual.h"


namespace
{
    // ========================================================
    // Trapper Launcher
    //
    // 本家Trapperは
    //
    // ・本体側に細い根元
    // ・先端側に広がったLauncher
    //
    // という見た目。
    //
    // Bolv.ioでは既存の
    //
    // TankBarrel.tkm
    // +
    // TankMachineGunBarrel.tkm
    //
    // を組み合わせる。
    // ========================================================

    const float TRAPPER_BASE_WIDTH_SCALE =
        0.72f;


    const float TRAPPER_BASE_LENGTH_SCALE =
        0.48f;


    const float TRAPPER_LAUNCHER_WIDTH_SCALE =
        1.18f;


    const float TRAPPER_LAUNCHER_LENGTH_SCALE =
        0.72f;


    // Launcherを少し前方へ配置。
    const float TRAPPER_LAUNCHER_FORWARD_OFFSET =
        11.0f;
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
    // 根元
    // ========================================================

    m_barrelRender1.SetPosition(
        modelPosition
    );


    m_barrelRender1.SetScale(
        Vector3(
            TRAPPER_BASE_WIDTH_SCALE *
            m_levelScale,

            TRAPPER_BASE_WIDTH_SCALE *
            m_levelScale,

            TRAPPER_BASE_LENGTH_SCALE *
            m_levelScale
        )
    );


    // ========================================================
    // Launcher
    // ========================================================

    const Vector3 launcherPosition =
        modelPosition +
        direction *
        (
            TRAPPER_LAUNCHER_FORWARD_OFFSET *
            m_levelScale
            );


    m_machineGunBarrelRender.SetPosition(
        launcherPosition
    );


    m_machineGunBarrelRender.SetScale(
        Vector3(
            TRAPPER_LAUNCHER_WIDTH_SCALE *
            m_levelScale,

            TRAPPER_LAUNCHER_WIDTH_SCALE *
            m_levelScale,

            TRAPPER_LAUNCHER_LENGTH_SCALE *
            m_levelScale
        )
    );
}


// ============================================================
// RenderTrapper
// ============================================================

void TankVisual::RenderTrapper(
    RenderContext& rc
)
{
    // ============================================
    // 根元
    // ============================================

    m_barrelRender1.Draw(
        rc
    );


    // ============================================
    // Launcher
    // ============================================

    m_machineGunBarrelRender.Draw(
        rc
    );
}