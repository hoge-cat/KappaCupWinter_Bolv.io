#include "stdafx.h"
#include "TankVisual.h"

#include <cmath>

void TankVisual::RecoilBarrelRender::SetPosition(const Vector3& position)
{
    m_restPosition = position;
    ApplyRecoil();
}

void TankVisual::RecoilBarrelRender::SetRotation(const Quaternion& rotation)
{
    ModelRender::SetRotation(rotation);
    m_forward = Vector3(0.0f, 0.0f, -1.0f);
    rotation.Apply(m_forward);
    m_forward.Normalize();
    ApplyRecoil();
}

void TankVisual::RecoilBarrelRender::ApplyRecoil()
{
    const float phase = m_recoilTime / 0.14f;
    ModelRender::SetPosition(m_restPosition - m_forward *
        (m_recoilDistance * phase * phase));
}

void TankVisual::RecoilBarrelRender::Kick(float distance)
{
    // A new shot resets the kick instead of accumulating displacement.
    m_recoilTime = 0.14f;
    m_recoilDistance = distance;
    ApplyRecoil();
    ModelRender::Update();
}

void TankVisual::RecoilBarrelRender::AdvanceRecoil(float deltaTime)
{
    m_recoilTime -= deltaTime;
    if (m_recoilTime < 0.0f)
    {
        m_recoilTime = 0.0f;
    }
    ApplyRecoil();
}

void TankVisual::RecoilBarrelRender::ResetRecoil()
{
    m_recoilTime = 0.0f;
    m_recoilDistance = 0.0f;
    ApplyRecoil();
}

std::array<TankVisual::RecoilBarrelRender*, 7> TankVisual::GetBarrels()
{
    return {{ &m_barrelRender1, &m_barrelRender2, &m_barrelRender3,
        &m_barrelRender4, &m_barrelRender5, &m_machineGunBarrelRender,
        &m_trapperBarrelRender }};
}

void TankVisual::ResetRecoil()
{
    for (auto* barrel : GetBarrels())
    {
        barrel->ResetRecoil();
    }
}

void TankVisual::OnShotFired(const Vector3& muzzlePosition,
    const Vector3& direction, float bulletSizeMultiplier)
{
    auto barrels = GetBarrels();
    int count = 1;
    switch (m_tankClass)
    {
    case PlayerEvolution::Smasher:
        return;
    case PlayerEvolution::MachineGun:
        barrels[0] = &m_machineGunBarrelRender;
        break;
    case PlayerEvolution::Trapper:
        barrels[0] = &m_trapperBarrelRender;
        break;
    case PlayerEvolution::Hunter:
        barrels[0] = bulletSizeMultiplier < 1.0f ?
            &m_barrelRender2 : &m_barrelRender1;
        break;
    case PlayerEvolution::Twin:
    case PlayerEvolution::FlankGuard:
        count = 2;
        break;
    case PlayerEvolution::TripleShot:
        count = 3;
        break;
    case PlayerEvolution::QuadTank:
    case PlayerEvolution::TwinFlank:
        count = 4;
        break;
    case PlayerEvolution::Sprayer:
        count = 5;
        break;
    default:
        break;
    }

    // Match the actual shot to the rendered barrel by facing and lateral offset.
    RecoilBarrelRender* selected = barrels[0];
    float bestScore = 1.0e30f;
    for (int i = 0; i < count; ++i)
    {
        const Vector3& forward = barrels[i]->GetForward();
        const float alignment = forward.x * direction.x + forward.z * direction.z;
        const Vector3 offset = muzzlePosition - barrels[i]->GetRestPosition();
        const float lateral = offset.x * forward.z - offset.z * forward.x;
        const float score = (1.0f - alignment) * 10000.0f + lateral * lateral;
        if (score < bestScore)
        {
            bestScore = score;
            selected = barrels[i];
        }
    }
    selected->Kick(5.0f * m_levelScale);
}



namespace
{
    const float PI =
        3.14159265f;
}


// ============================================================
// Init
// ============================================================

void TankVisual::Init()
{
    // ============================================
    // 本体
    // ============================================

    m_bodyRender.Init(
        "Assets/modelData/tank/TankBody.tkm"
    );


    // ============================================
    // 通常砲身
    // ============================================

    m_barrelRender1.Init(
        "Assets/modelData/tank/TankBarrel.tkm"
    );


    m_barrelRender2.Init(
        "Assets/modelData/tank/TankBarrel.tkm"
    );


    m_barrelRender3.Init(
        "Assets/modelData/tank/TankBarrel.tkm"
    );


    m_barrelRender4.Init(
        "Assets/modelData/tank/TankBarrel.tkm"
    );


    m_barrelRender5.Init(
        "Assets/modelData/tank/TankBarrel.tkm"
    );


    // ============================================
    // Machine Gun用
    // ============================================

    m_machineGunBarrelRender.Init(
        "Assets/modelData/tank/TankMachineGunBarrel.tkm"
    );


    // ============================================
    // Trapper専用砲身
    // ============================================

    m_trapperBarrelRender.Init(
        "Assets/modelData/tank/TankTrapperBarrel.tkm"
    );


    // ============================================
    // Smasher
    // ============================================

    m_smasherShellRender.Init(
        "Assets/modelData/tank/TankSmasherShell.tkm"
    );
}


// ============================================================
// SetLevelScale
// ============================================================

void TankVisual::SetLevelScale(
    float scale)
{
    m_levelScale =
        scale;
}


// ============================================================
// CreateBarrelRotation
// ============================================================

Quaternion TankVisual::CreateBarrelRotation(
    const Vector3& inputDirection)
{
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


    // ============================================
    // モデルの基準前方向
    // ============================================

    const Vector3 modelForward(
        0.0f,
        0.0f,
        -1.0f
    );


    // ============================================
    // 回転軸
    // ============================================

    Vector3 rotationAxis;


    rotationAxis.Cross(
        modelForward,
        direction
    );


    const float axisLength =
        std::sqrt(
            rotationAxis.x *
            rotationAxis.x +
            rotationAxis.y *
            rotationAxis.y +
            rotationAxis.z *
            rotationAxis.z
        );


    Quaternion rotation =
        Quaternion::Identity;


    if (axisLength >
        0.0001f)
    {
        rotationAxis.Normalize();


        const float angle =
            std::atan2(
                axisLength,
                -direction.z
            );


        rotation.SetRotationDeg(
            rotationAxis,
            angle *
            180.0f /
            PI
        );
    }
    else if (
        direction.z >
        0.0f)
    {
        rotation.SetRotationDeg(
            Vector3::AxisY,
            180.0f
        );
    }


    return rotation;
}


// ============================================================
// Update
// ============================================================

void TankVisual::Update(
    const Vector3& position,
    const Vector3& aimDirection)
{
    for (auto* barrel : GetBarrels())
    {
        barrel->AdvanceRecoil(g_gameTime->GetFrameDeltaTime());
    }

    // ============================================
    // 戦車中央位置
    // ============================================

    Vector3 modelPosition =
        position;


    modelPosition.y +=
        30.0f *
        m_levelScale;


    m_bodyRender.SetPosition(
        modelPosition
    );


    // ============================================
    // 本体サイズ
    // ============================================

    const Vector3 bodyScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_bodyRender.SetScale(
        bodyScale
    );


    // ============================================
    // 照準方向
    // ============================================

    Vector3 direction =
        aimDirection;


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


    // ============================================
    // 右方向
    // ============================================

    Vector3 right(
        direction.z,
        0.0f,
        -direction.x
    );


    right.Normalize();


    // ============================================
    // 本体回転
    // ============================================

    const float yaw =
        std::atan2(
            direction.x,
            direction.z
        );


    Quaternion bodyRotation;


    bodyRotation.SetRotationDeg(
        Vector3::AxisY,
        yaw *
        180.0f /
        PI +
        180.0f
    );


    m_bodyRender.SetRotation(
        bodyRotation
    );


    // ============================================
    // 共通砲身回転
    // ============================================

    const Quaternion forwardRotation =
        CreateBarrelRotation(
            direction
        );


    // ============================================
    // デフォルト位置
    // ============================================

    const Vector3 normalBarrelScale(
        m_levelScale,
        m_levelScale,
        m_levelScale
    );


    m_barrelRender1.SetPosition(
        modelPosition
    );


    m_barrelRender2.SetPosition(
        modelPosition
    );


    m_barrelRender3.SetPosition(
        modelPosition
    );


    m_barrelRender4.SetPosition(
        modelPosition
    );


    m_barrelRender5.SetPosition(
        modelPosition
    );


    m_machineGunBarrelRender.SetPosition(
        modelPosition
    );


    m_trapperBarrelRender.SetPosition(
        modelPosition
    );


    // ============================================
    // デフォルトサイズ
    // ============================================

    m_barrelRender1.SetScale(
        normalBarrelScale
    );


    m_barrelRender2.SetScale(
        normalBarrelScale
    );


    m_barrelRender3.SetScale(
        normalBarrelScale
    );


    m_barrelRender4.SetScale(
        normalBarrelScale
    );


    m_barrelRender5.SetScale(
        normalBarrelScale
    );


    m_machineGunBarrelRender.SetScale(
        normalBarrelScale
    );


    m_trapperBarrelRender.SetScale(
        normalBarrelScale
    );


    // ============================================
    // デフォルト回転
    // ============================================

    m_barrelRender1.SetRotation(
        forwardRotation
    );


    m_barrelRender2.SetRotation(
        forwardRotation
    );


    m_barrelRender3.SetRotation(
        forwardRotation
    );


    m_barrelRender4.SetRotation(
        forwardRotation
    );


    m_barrelRender5.SetRotation(
        forwardRotation
    );


    m_machineGunBarrelRender.SetRotation(
        forwardRotation
    );


    m_trapperBarrelRender.SetRotation(
        forwardRotation
    );


    // ========================================================
    // Basic
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Basic)
    {
        UpdateBasic();
    }


    // ========================================================
    // Twin
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Twin)
    {
        UpdateTwin(
            modelPosition,
            right
        );
    }


    // ========================================================
    // Triple Shot
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::TripleShot)
    {
        UpdateTripleShot(
            direction
        );
    }


    // ========================================================
    // Quad Tank
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::QuadTank)
    {
        UpdateQuadTank(
            direction
        );
    }


    // ========================================================
    // Twin Flank
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::TwinFlank)
    {
        UpdateTwinFlank(
            modelPosition,
            direction,
            right
        );
    }


    // ========================================================
    // Sniper
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Sniper)
    {
        UpdateSniper();
    }


    // ========================================================
    // Assassin
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Assassin)
    {
        UpdateAssassin();
    }


    // ========================================================
    // Hunter
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Hunter)
    {
        UpdateHunter(
            modelPosition,
            right
        );
    }


    // ========================================================
    // Trapper
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Trapper)
    {
        UpdateTrapper(
            modelPosition,
            direction
        );
    }


    // ========================================================
    // Machine Gun
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::MachineGun)
    {
        UpdateMachineGun();
    }


    // ========================================================
    // Flank Guard
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::FlankGuard)
    {
        UpdateFlankGuard(
            direction
        );
    }


    // ========================================================
    // Smasher
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Smasher)
    {
        UpdateSmasher(
            modelPosition
        );
    }


    // ========================================================
    // Sprayer
    // ========================================================

    else if (
        m_tankClass ==
        PlayerEvolution::Sprayer)
    {
        UpdateSprayer(
            direction
        );
    }


    // ============================================
    // モデル更新
    // ============================================

    m_bodyRender.Update();


    m_barrelRender1.Update();

    m_barrelRender2.Update();

    m_barrelRender3.Update();

    m_barrelRender4.Update();

    m_barrelRender5.Update();


    m_machineGunBarrelRender.Update();


    m_trapperBarrelRender.Update();


    m_smasherShellRender.Update();
}


// ============================================================
// Render
// ============================================================

void TankVisual::Render(
    RenderContext& rc)
{
    // ============================================
    // 本体
    // ============================================

    m_bodyRender.Draw(
        rc
    );


    // ========================================================
    // Basic
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Basic)
    {
        RenderBasic(
            rc
        );


        return;
    }


    // ========================================================
    // Twin
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Twin)
    {
        RenderTwin(
            rc
        );


        return;
    }


    // ========================================================
    // Triple Shot
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::TripleShot)
    {
        RenderTripleShot(
            rc
        );


        return;
    }


    // ========================================================
    // Quad Tank
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::QuadTank)
    {
        RenderQuadTank(
            rc
        );


        return;
    }


    // ========================================================
    // Twin Flank
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::TwinFlank)
    {
        RenderTwinFlank(
            rc
        );


        return;
    }


    // ========================================================
    // Sniper
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Sniper)
    {
        RenderSniper(
            rc
        );


        return;
    }


    // ========================================================
    // Assassin
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Assassin)
    {
        RenderAssassin(
            rc
        );


        return;
    }


    // ========================================================
    // Hunter
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Hunter)
    {
        RenderHunter(
            rc
        );


        return;
    }


    // ========================================================
    // Trapper
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Trapper)
    {
        RenderTrapper(
            rc
        );


        return;
    }


    // ========================================================
    // Machine Gun
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::MachineGun)
    {
        RenderMachineGun(
            rc
        );


        return;
    }


    // ========================================================
    // Flank Guard
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::FlankGuard)
    {
        RenderFlankGuard(
            rc
        );


        return;
    }


    // ========================================================
    // Smasher
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Smasher)
    {
        RenderSmasher(
            rc
        );


        return;
    }


    // ========================================================
    // Sprayer
    // ========================================================

    if (m_tankClass ==
        PlayerEvolution::Sprayer)
    {
        RenderSprayer(
            rc
        );


        return;
    }
}