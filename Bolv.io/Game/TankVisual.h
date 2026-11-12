#pragma once

#include "PlayerEvolution.h"
#include <array>


// ============================================================
// 戦車の見た目を管理する。
// ============================================================

class TankVisual
{
public:

    void Init();
    void OnShotFired(const Vector3& muzzlePosition, const Vector3& direction,
        float bulletSizeMultiplier = 1.0f);


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
        if (m_tankClass != tankClass)
        {
            m_tankClass = tankClass;
            ResetRecoil();
        }
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

    // Visual-only barrel transforms; collision and muzzle positions stay unchanged.
    class RecoilBarrelRender : public ModelRender
    {
    public:
        void SetPosition(const Vector3& position);
        void SetRotation(const Quaternion& rotation);
        void Kick(float distance);
        void AdvanceRecoil(float deltaTime);
        void ResetRecoil();
        const Vector3& GetRestPosition() const { return m_restPosition; }
        const Vector3& GetForward() const { return m_forward; }

    private:
        void ApplyRecoil();
        Vector3 m_restPosition = Vector3::Zero;
        Vector3 m_forward = Vector3(0.0f, 0.0f, -1.0f);
        float m_recoilTime = 0.0f;
        float m_recoilDistance = 0.0f;
    };

    std::array<RecoilBarrelRender*, 7> GetBarrels();
    void ResetRecoil();

    // ========================================================
    // Body
    // ========================================================

    ModelRender m_bodyRender;


    // ========================================================
    // 通常砲身
    // ========================================================

    RecoilBarrelRender m_barrelRender1;

    RecoilBarrelRender m_barrelRender2;

    RecoilBarrelRender m_barrelRender3;

    RecoilBarrelRender m_barrelRender4;

    RecoilBarrelRender m_barrelRender5;


    // ========================================================
    // Machine Gun
    // ========================================================

    RecoilBarrelRender m_machineGunBarrelRender;


    // ========================================================
    // Trapper
    // ========================================================

    RecoilBarrelRender m_trapperBarrelRender;


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