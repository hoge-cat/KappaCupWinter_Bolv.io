#include "stdafx.h"
#include "Player.h"

#include <cmath>


bool Player::Start()
{
    m_charaCon.Init(
        20.0f,
        100.0f,
        m_position
    );


    m_tankVisual.Init();


    // ============================================
    // Diep.io風の初期HP
    //
    // Lv1 = 50
    // ============================================

    m_health.SetMaxHP(
        m_stats
        .GetMaxHealthValue(
            m_progress
            .GetLevel()
        )
    );


    m_previousTankClass =
        m_evolution
        .GetCurrentClass();


    m_tankVisual.Update(
        m_position,
        g_camera3D->GetForward()
    );


    return true;
}


// ============================================================
// Update
// ============================================================

void Player::Update()
{
    if (m_health.IsDead())
    {
        return;
    }


    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ============================================
    // F2 デバッグレベルアップ
    // ============================================

    const bool debugLevelUpKey =
        (
            GetAsyncKeyState(
                VK_F2
            ) &
            0x8000
            ) !=
        0;


    if (debugLevelUpKey &&
        !m_debugLevelUpKeyPressed)
    {
        if (m_progress
            .GetLevel() <
            45)
        {
            const int required =
                m_progress
                .GetRequiredExperience();


            const int current =
                m_progress
                .GetCurrentExperience();


            const int addExperience =
                required -
                current;


            if (addExperience >
                0)
            {
                m_progress
                    .AddExperience(
                        addExperience
                    );
            }
        }
    }


    m_debugLevelUpKeyPressed =
        debugLevelUpKey;


    // ============================================
    // Diep.io風
    // レベルによる最大HP増加
    //
    // Lv1 = 50
    // 以降1レベルごとに +2
    // ============================================

    const int targetMaxHP =
        m_stats
        .GetMaxHealthValue(
            m_progress
            .GetLevel()
        );


    if (m_health.GetMaxHP() !=
        targetMaxHP)
    {
        m_health.SetMaxHP(
            targetMaxHP
        );
    }


    // ============================================
    // Health Regen
    //
    // Diep.io通常回復式
    //
    // MaxHP / 30 *
    // (0.03 + 0.12 * RegenLevel)
    //
    // 小数HPはAccumulatorに貯める。
    // ============================================

    if (m_health.GetHP() <
        m_health.GetMaxHP())
    {
        const float regenPerSecond =
            m_stats
            .GetHealthRegenPerSecond(
                m_health
                .GetMaxHP()
            );


        m_healthRegenAccumulator +=
            regenPerSecond *
            deltaTime;


        const int healAmount =
            static_cast<int>(
                m_healthRegenAccumulator
                );


        if (healAmount >
            0)
        {
            m_health.Heal(
                healAmount
            );


            m_healthRegenAccumulator -=
                static_cast<float>(
                    healAmount
                    );
        }
    }
    else
    {
        m_healthRegenAccumulator =
            0.0f;
    }


    // ============================================
    // 進化
    // ============================================

    m_evolution.Update(
        m_progress
        .GetLevel()
    );


    // ============================================
    // 進化によるStat変更
    // ============================================

    HandleEvolutionStatChanges();


    // ============================================
    // 移動
    // ============================================

    Move();


    // ============================================
    // レベルによるサイズ
    // ============================================

    const int currentLevel =
        m_progress
        .GetLevel();


    const float levelScale =
        std::pow(
            1.01f,
            static_cast<float>(
                currentLevel -
                1
                )
        );


    m_tankVisual
        .SetLevelScale(
            levelScale
        );


    m_tankVisual
        .SetTankClass(
            m_evolution
            .GetCurrentClass()
        );


    // ============================================
    // 照準方向
    // ============================================

    Vector3 aimDirection =
        g_camera3D
        ->GetForward();


    aimDirection.y =
        0.0f;


    const float aimLengthSquared =
        aimDirection.x *
        aimDirection.x +
        aimDirection.z *
        aimDirection.z;


    if (aimLengthSquared >
        0.0001f)
    {
        aimDirection.Normalize();


        m_aimDirection =
            aimDirection;
    }
    else
    {
        aimDirection =
            m_aimDirection;
    }


    // ============================================
    // 見た目
    // ============================================

    m_tankVisual.Update(
        m_position,
        aimDirection
    );


    // ============================================
    // 接触
    // ============================================

    m_contact.Update(
        *this
    );


    if (m_health.IsDead())
    {
        return;
    }


    // ============================================
    // 射撃
    // ============================================

    m_tankWeapon.Update(
        m_position,
        aimDirection
    );
}


// ============================================================
// HandleEvolutionStatChanges
// ============================================================

void Player::HandleEvolutionStatChanges()
{
    const PlayerEvolution::TankClass currentClass =
        m_evolution
        .GetCurrentClass();


    if (currentClass ==
        m_previousTankClass)
    {
        return;
    }


    // ============================================
    // Smasherへ進化
    // ============================================

    if (currentClass ==
        PlayerEvolution::Smasher)
    {
        const int refundPoints =
            m_stats
            .ResetBulletStats();


        m_progress
            .AddUpgradePoints(
                refundPoints
            );


        m_health
            .SetMaxHP(
                m_stats
                .GetMaxHealthValue(
                    m_progress
                    .GetLevel()
                )
            );


        char message[128];


        sprintf_s(
            message,
            "Evolved to Smasher. Refunded %d upgrade points.\n",
            refundPoints
        );


        OutputDebugStringA(
            message
        );
    }


    m_previousTankClass =
        currentClass;
}


// ============================================================
// Move
// ============================================================

void Player::Move()
{
    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    const float maxMoveSpeed =
        150.0f *
        m_stats
        .GetMovementSpeedMultiplier(
            m_progress
            .GetLevel()
        );


    float x =
        0.0f;


    float z =
        0.0f;


    if (GetAsyncKeyState(
        'W') &
        0x8000)
    {
        z +=
            1.0f;
    }


    if (GetAsyncKeyState(
        'S') &
        0x8000)
    {
        z -=
            1.0f;
    }


    if (GetAsyncKeyState(
        'A') &
        0x8000)
    {
        x -=
            1.0f;
    }


    if (GetAsyncKeyState(
        'D') &
        0x8000)
    {
        x +=
            1.0f;
    }


    // ============================================
    // Diep.io風の慣性
    // ============================================

    const float friction =
        std::pow(
            0.9f,
            deltaTime /
            0.04f
        );


    m_moveSpeed.x *=
        friction;


    m_moveSpeed.z *=
        friction;


    if (x !=
        0.0f ||
        z !=
        0.0f)
    {
        Vector3 forward =
            g_camera3D
            ->GetForward();


        forward.y =
            0.0f;


        const float forwardLengthSquared =
            forward.x *
            forward.x +
            forward.z *
            forward.z;


        if (forwardLengthSquared >
            0.0001f)
        {
            forward.Normalize();
        }


        Vector3 right =
            g_camera3D
            ->GetRight();


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


        Vector3 moveDirection =
            forward *
            z +
            right *
            x;


        const float moveLengthSquared =
            moveDirection.x *
            moveDirection.x +
            moveDirection.z *
            moveDirection.z;


        if (moveLengthSquared >
            0.0001f)
        {
            moveDirection.Normalize();


            const float acceleration =
                maxMoveSpeed *
                (
                    1.0f -
                    friction
                    );


            m_moveSpeed.x +=
                moveDirection.x *
                acceleration;


            m_moveSpeed.z +=
                moveDirection.z *
                acceleration;
        }
    }


    if (std::fabs(
        m_moveSpeed.x) <
        0.01f)
    {
        m_moveSpeed.x =
            0.0f;
    }


    if (std::fabs(
        m_moveSpeed.z) <
        0.01f)
    {
        m_moveSpeed.z =
            0.0f;
    }


    // ============================================
    // 重力
    // ============================================

    m_moveSpeed.y -=
        980.0f *
        deltaTime;


    // ============================================
    // 移動
    // ============================================

    m_position =
        m_charaCon.Execute(
            m_moveSpeed,
            deltaTime
        );


    if (m_charaCon
        .IsOnGround())
    {
        m_moveSpeed.y =
            0.0f;
    }
}


// ============================================================
// Render
// ============================================================

void Player::Render(
    RenderContext& rc)
{
    m_tankVisual.Render(
        rc
    );
}