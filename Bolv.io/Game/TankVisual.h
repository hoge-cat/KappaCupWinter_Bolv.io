#pragma once

#include "PlayerEvolution.h"


// ============================================================
// 戦車の見た目を管理する。
// ============================================================

class TankVisual
{
public:

    void Init();


    void Update(
        const Vector3& position,
        const Vector3& aimDirection
    );


    void Render(
        RenderContext& rc
    );


    // ========================================================
    // Level Scale
    // ========================================================

    void SetLevelScale(
        float scale
    );


    // ========================================================
    // Tank Class
    // ========================================================

    void SetTankClass(
        PlayerEvolution::TankClass tankClass
    )
    {
        m_tankClass =
            tankClass;
    }


private:

    // ========================================================
    // 共通
    // ========================================================

    Quaternion CreateBarrelRotation(
        const Vector3& direction
    );


    // ========================================================
    // Basic
    // ========================================================

    void UpdateBasic();


    void RenderBasic(
        RenderContext& rc
    );


    // ========================================================
    // Twin
    // ========================================================

    void UpdateTwin(
        const Vector3& modelPosition,
        const Vector3& right
    );


    void RenderTwin(
        RenderContext& rc
    );


    // ========================================================
    // Triple Shot
    // ========================================================

    void UpdateTripleShot(
        const Vector3& direction
    );


    void RenderTripleShot(
        RenderContext& rc
    );


    // ========================================================
    // Quad Tank
    // ========================================================

    void UpdateQuadTank(
        const Vector3& direction
    );


    void RenderQuadTank(
        RenderContext& rc
    );


    // ========================================================
    // Twin Flank
    // ========================================================

    void UpdateTwinFlank(
        const Vector3& modelPosition,
        const Vector3& direction,
        const Vector3& right
    );


    void RenderTwinFlank(
        RenderContext& rc
    );


    // ========================================================
    // Sniper
    // ========================================================

    void UpdateSniper();


    void RenderSniper(
        RenderContext& rc
    );


    // ========================================================
    // Assassin
    // ========================================================

    void UpdateAssassin();


    void RenderAssassin(
        RenderContext& rc
    );


    // ========================================================
    // Hunter
    // ========================================================

    void UpdateHunter(
        const Vector3& modelPosition,
        const Vector3& right
    );


    void RenderHunter(
        RenderContext& rc
    );


    // ========================================================
    // Trapper
    // ========================================================

    void UpdateTrapper(
        const Vector3& modelPosition,
        const Vector3& direction
    );


    void RenderTrapper(
        RenderContext& rc
    );


    // ========================================================
    // Machine Gun
    // ========================================================

    void UpdateMachineGun();


    void RenderMachineGun(
        RenderContext& rc
    );


    // ========================================================
    // Flank Guard
    // ========================================================

    void UpdateFlankGuard(
        const Vector3& direction
    );


    void RenderFlankGuard(
        RenderContext& rc
    );


    // ========================================================
    // Smasher
    // ========================================================

    void UpdateSmasher(
        const Vector3& modelPosition
    );


    void RenderSmasher(
        RenderContext& rc
    );


    // ========================================================
    // Sprayer
    // ========================================================

    void UpdateSprayer(
        const Vector3& direction
    );


    void RenderSprayer(
        RenderContext& rc
    );


private:

    // ========================================================
    // Body
    // ========================================================

    ModelRender m_bodyRender;


    // ========================================================
    // 通常砲身
    // ========================================================

    ModelRender m_barrelRender1;

    ModelRender m_barrelRender2;

    ModelRender m_barrelRender3;

    ModelRender m_barrelRender4;

    ModelRender m_barrelRender5;


    // ========================================================
    // Machine Gun
    // ========================================================

    ModelRender m_machineGunBarrelRender;


    // ========================================================
    // Trapper
    // ========================================================

    ModelRender m_trapperBarrelRender;


    // ========================================================
    // Smasher
    // ========================================================

    ModelRender m_smasherShellRender;


    float m_smasherRotationDeg =
        0.0f;


    // ========================================================
    // Level Scale
    // ========================================================

    float m_levelScale =
        1.0f;


    // ========================================================
    // Tank Class
    // ========================================================

    PlayerEvolution::TankClass m_tankClass =
        PlayerEvolution::Basic;
};