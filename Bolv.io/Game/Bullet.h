#pragma once


class Bullet : public IGameObject
{
public:

    bool Start();

    void Update();

    void Render(
        RenderContext& rc
    );


    // ============================================
    // 初期化
    // ============================================

    void Init(
        const Vector3& position,
        const Vector3& direction,
        int damage,
        float speed,
        int penetration,
        int collisionDamage,
        float levelScale
    );


    // ============================================
    // Renderer用
    // ============================================

    const Vector3& GetPosition() const
    {
        return
            m_position;
    }


    float GetLevelScale() const
    {
        return
            m_levelScale;
    }


private:

    // ============================================
    // Position
    // ============================================

    Vector3 m_position =
        Vector3::Zero;


    // ============================================
    // Direction
    // ============================================

    Vector3 m_direction =
        Vector3(
            0.0f,
            0.0f,
            1.0f
        );


    // ============================================
    // Movement
    // ============================================

    float m_speed =
        500.0f;


    float m_remainingTime =
        3.0f;


    // ============================================
    // Level Scale
    // ============================================

    float m_levelScale =
        1.0f;


    float m_radius =
        6.0f;


    // ============================================
    // Bullet Damage
    // ============================================

    int m_damage =
        7;


    // ============================================
    // Bullet Penetration
    //
    // Bolv.io内部では整数スケールを使用。
    //
    // 0pt = 200
    // 1pt = 350
    //
    // これはDiep.ioの
    // 2.0 / 3.5 / 5.0 ...
    // を100倍したもの。
    // ============================================

    int m_penetration =
        200;


    // ============================================
    // Bullet vs Bullet
    // ============================================

    int m_collisionDamage =
        100;
};