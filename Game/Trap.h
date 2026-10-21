#pragma once


// ============================================================
// Trapperが発射するTrap。
//
// 描画はTrap自身では行わず、
// TrapRendererがまとめてインスタンシング描画する。
// ============================================================

class Trap :
    public IGameObject
{
public:

    bool Start();

    void Update();

    void Render(
        RenderContext& rc
    );


    // ========================================================
    // 初期化
    // ========================================================

    void Init(
        const Vector3& position,
        const Vector3& direction,
        int damage,
        float speed,
        int penetration,
        int collisionDamage,
        float levelScale
    );


    // ========================================================
    // TrapRenderer用
    // ========================================================

    const Vector3& GetPosition() const
    {
        return
            m_position;
    }


    const Quaternion& GetRotation() const
    {
        return
            m_rotation;
    }


    float GetLevelScale() const
    {
        return
            m_levelScale;
    }


    bool IsDead() const
    {
        return
            m_remainingTime <=
            0.0f ||
            m_penetration <=
            0;
    }


private:

    bool IsPointNearSegmentXZ(
        const Vector3& start,
        const Vector3& end,
        const Vector3& point,
        float radius
    ) const;


private:

    // ========================================================
    // Position / Direction
    // ========================================================

    Vector3 m_position =
        Vector3::Zero;


    Vector3 m_direction =
        Vector3(
            0.0f,
            0.0f,
            1.0f
        );


    Quaternion m_rotation =
        Quaternion::Identity;


    // ========================================================
    // Movement
    // ========================================================

    float m_speed =
        0.0f;


    Vector3 m_pushVelocity =
        Vector3::Zero;


    // ========================================================
    // Life
    // ========================================================

    float m_remainingTime =
        24.0f;


    float m_tankHitCooldown =
        0.0f;


    // ========================================================
    // Performance
    // ========================================================

    int m_damage =
        8;


    int m_penetration =
        300;


    int m_collisionDamage =
        210;


    // ========================================================
    // Size
    // ========================================================

    float m_levelScale =
        1.0f;


    float m_radius =
        12.0f;
};