#include "stdafx.h"
#include "EnemyMovement.h"

#include <cmath>
#include <cstdlib>


namespace
{
    const float PI =
        3.14159265f;
}


// ============================================================
// RandomRange
// ============================================================

float EnemyMovement::RandomRange(
    float minValue,
    float maxValue)
{
    const float random01 =
        static_cast<float>(
            std::rand()
            ) /
        static_cast<float>(
            RAND_MAX
            );


    return
        minValue +
        (
            maxValue -
            minValue
            ) *
        random01;
}


// ============================================================
// AddKnockback
// ============================================================

void EnemyMovement::AddKnockback(
    const Vector3& direction,
    float power)
{
    Vector3 knockbackDirection =
        direction;


    knockbackDirection.y =
        0.0f;


    const float lengthSquared =
        knockbackDirection.x *
        knockbackDirection.x +
        knockbackDirection.z *
        knockbackDirection.z;


    if (lengthSquared <=
        0.0001f)
    {
        return;
    }


    knockbackDirection.Normalize();


    m_knockbackVelocity.x +=
        knockbackDirection.x *
        power;


    m_knockbackVelocity.z +=
        knockbackDirection.z *
        power;
}


// ============================================================
// UpdateAimError
// ============================================================

void EnemyMovement::UpdateAimError(
    float deltaTime,
    float maxAimErrorDeg,
    float aimChangeInterval)
{
    // ============================================
    // 安全対策
    // ============================================

    if (maxAimErrorDeg <
        0.0f)
    {
        maxAimErrorDeg =
            0.0f;
    }


    if (aimChangeInterval <
        0.1f)
    {
        aimChangeInterval =
            0.1f;
    }


    // ============================================
    // 次の誤差角度を決めるタイマー
    // ============================================

    m_aimChangeTimer -=
        deltaTime;


    if (m_aimChangeTimer <=
        0.0f)
    {
        // ========================================
        // -max ～ +max の範囲で
        // 新しい照準誤差を作る。
        // ========================================

        m_targetAimErrorDeg =
            RandomRange(
                -maxAimErrorDeg,
                maxAimErrorDeg
            );


        // ========================================
        // 毎回同じ周期だと機械的なので
        // 0.7～1.3倍で少し揺らす。
        // ========================================

        m_aimChangeTimer =
            aimChangeInterval *
            RandomRange(
                0.7f,
                1.3f
            );
    }


    // ============================================
    // 現在値 → 目標値へ滑らかに近づける
    //
    // フレームレートに依存しにくい補間。
    // ============================================

    const float aimFollow =
        1.0f -
        std::pow(
            0.05f,
            deltaTime
        );


    m_currentAimErrorDeg +=
        (
            m_targetAimErrorDeg -
            m_currentAimErrorDeg
            ) *
        aimFollow;


    // ============================================
    // 最大誤差を超えないようにする
    // ============================================

    if (m_currentAimErrorDeg >
        maxAimErrorDeg)
    {
        m_currentAimErrorDeg =
            maxAimErrorDeg;
    }


    if (m_currentAimErrorDeg <
        -maxAimErrorDeg)
    {
        m_currentAimErrorDeg =
            -maxAimErrorDeg;
    }
}


// ============================================================
// Update
// ============================================================

void EnemyMovement::Update(
    Vector3& position,
    Vector3& aimDirection,
    const Vector3& targetPosition,
    EnemyMoveType type,
    float deltaTime,
    float movementSpeedMultiplier,
    float maxAimErrorDeg,
    float aimChangeInterval)
{
    // ============================================
    // 実際の移動速度
    // ============================================

    if (movementSpeedMultiplier <
        0.1f)
    {
        movementSpeedMultiplier =
            0.1f;
    }


    const float moveSpeed =
        m_baseSpeed *
        movementSpeedMultiplier;


    // ============================================
    // プレイヤー方向
    // ============================================

    Vector3 direction =
        targetPosition -
        position;


    direction.y =
        0.0f;


    const float distance =
        std::sqrt(
            direction.x *
            direction.x +
            direction.z *
            direction.z
        );


    // ============================================
    // 通常AI移動
    // ============================================

    if (distance >=
        0.001f)
    {
        direction.Normalize();


        // ====================================================
        // Orbit
        // ====================================================

        if (type ==
            EnemyMoveType::Orbit)
        {
            // プレイヤー方向に対する横方向。
            const Vector3 tangent(
                -direction.z,
                0.0f,
                direction.x
            );


            // ================================================
            // 距離400へ戻りながら旋回
            // ================================================

            Vector3 velocity =
                direction *
                (
                    (
                        distance -
                        400.0f
                        ) *
                    0.8f
                    ) +
                tangent *
                moveSpeed *
                m_orbitSign;


            const float speed =
                std::sqrt(
                    velocity.x *
                    velocity.x +
                    velocity.z *
                    velocity.z
                );


            // ================================================
            // 移動速度上限
            // ================================================

            if (speed >
                moveSpeed)
            {
                velocity *=
                    moveSpeed /
                    speed;
            }


            position +=
                velocity *
                deltaTime;
        }


        // ====================================================
        // Chase / KeepDistance
        // ====================================================

        else
        {
            float signedDistance =
                0.0f;


            // ================================================
            // Chase
            // ================================================

            if (type ==
                EnemyMoveType::Chase)
            {
                if (distance >
                    55.0f)
                {
                    signedDistance =
                        distance -
                        55.0f;
                }
            }


            // ================================================
            // KeepDistance
            // ================================================

            else
            {
                if (distance >
                    450.0f)
                {
                    signedDistance =
                        distance -
                        450.0f;
                }
                else if (
                    distance <
                    350.0f)
                {
                    signedDistance =
                        distance -
                        350.0f;
                }
            }


            const float maxMove =
                moveSpeed *
                deltaTime;


            if (signedDistance >
                maxMove)
            {
                signedDistance =
                    maxMove;
            }
            else if (
                signedDistance <
                -maxMove)
            {
                signedDistance =
                    -maxMove;
            }


            position +=
                direction *
                signedDistance;
        }
    }


    // ============================================
    // ノックバック減衰
    // ============================================

    const float knockbackFriction =
        std::pow(
            0.84f,
            deltaTime /
            0.04f
        );


    m_knockbackVelocity.x *=
        knockbackFriction;


    m_knockbackVelocity.z *=
        knockbackFriction;


    // ============================================
    // ノックバック移動
    // ============================================

    position.x +=
        m_knockbackVelocity.x *
        deltaTime;


    position.z +=
        m_knockbackVelocity.z *
        deltaTime;


    // ============================================
    // 微小速度を停止
    // ============================================

    if (std::fabs(
        m_knockbackVelocity.x) <
        0.01f)
    {
        m_knockbackVelocity.x =
            0.0f;
    }


    if (std::fabs(
        m_knockbackVelocity.z) <
        0.01f)
    {
        m_knockbackVelocity.z =
            0.0f;
    }


    // ============================================
    // 壁の内側
    // ============================================

    const float limit =
        1470.0f;


    if (position.x <
        -limit)
    {
        position.x =
            -limit;
    }


    if (position.x >
        limit)
    {
        position.x =
            limit;
    }


    if (position.z <
        -limit)
    {
        position.z =
            -limit;
    }


    if (position.z >
        limit)
    {
        position.z =
            limit;
    }


    // ========================================================
    // 照準誤差更新
    // ========================================================

    UpdateAimError(
        deltaTime,
        maxAimErrorDeg,
        aimChangeInterval
    );


    // ========================================================
    // 移動後の位置からプレイヤー方向
    // ========================================================

    Vector3 aim =
        targetPosition -
        position;


    aim.y =
        0.0f;


    if (
        aim.x *
        aim.x +
        aim.z *
        aim.z >
        0.0001f)
    {
        aim.Normalize();


        // ========================================
        // 誤差角度をXZ平面へ適用
        // ========================================

        const float radian =
            m_currentAimErrorDeg *
            PI /
            180.0f;


        const float cosValue =
            std::cos(
                radian
            );


        const float sinValue =
            std::sin(
                radian
            );


        const float originalX =
            aim.x;


        const float originalZ =
            aim.z;


        aim.x =
            originalX *
            cosValue +
            originalZ *
            sinValue;


        aim.z =
            -originalX *
            sinValue +
            originalZ *
            cosValue;


        aim.Normalize();


        aimDirection =
            aim;
    }
}