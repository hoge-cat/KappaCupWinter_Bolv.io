#pragma once


enum class EnemyMoveType
{
    Chase,
    KeepDistance,
    Orbit
};


class EnemyMovement
{
public:

    void Update(
        Vector3& position,
        Vector3& aimDirection,
        const Vector3& targetPosition,
        EnemyMoveType type,
        float deltaTime,
        float movementSpeedMultiplier,
        float maxAimErrorDeg,
        float aimChangeInterval
    );


    void SetOrbitDirection(
        bool clockwise
    )
    {
        m_orbitSign =
            clockwise ?
            1.0f :
            -1.0f;
    }


    // ============================================
    // ノックバック
    // ============================================

    void AddKnockback(
        const Vector3& direction,
        float power
    );


private:

    // ============================================
    // 照準誤差更新
    // ============================================

    void UpdateAimError(
        float deltaTime,
        float maxAimErrorDeg,
        float aimChangeInterval
    );


    float RandomRange(
        float minValue,
        float maxValue
    );


private:

    // ============================================
    // 基礎移動速度
    // ============================================

    float m_baseSpeed =
        80.0f;


    // ============================================
    // Orbit方向
    // ============================================

    float m_orbitSign =
        1.0f;


    // ============================================
    // ノックバック速度
    // ============================================

    Vector3 m_knockbackVelocity =
        Vector3::Zero;


    // ============================================
    // 照準
    //
    // current:
    // 現在の誤差角度
    //
    // target:
    // 次に目指す誤差角度
    // ============================================

    float m_currentAimErrorDeg =
        0.0f;


    float m_targetAimErrorDeg =
        0.0f;


    float m_aimChangeTimer =
        0.0f;
};