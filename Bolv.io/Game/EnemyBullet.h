#pragma once


class Enemy;


class EnemyBullet : public IGameObject
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
        float levelScale,
        Enemy* owner
    );


    // ============================================
    // Renderer用
    // ============================================

    const Vector3& GetPosition() const
    {
        return m_position;
    }


    float GetLevelScale() const
    {
        return m_levelScale;
    }


    // ============================================
    // 状態
    // ============================================

    bool IsDead() const
    {
        return
            m_bulletHealth <= 0;
    }


    // ============================================
    // Bullet Collision Damage
    // ============================================

    int GetCollisionDamage() const
    {
        return m_collisionDamage;
    }


    void TakeCollisionDamage(
        int damage
    )
    {
        if (damage <=
            0)
        {
            return;
        }


        m_bulletHealth -=
            damage;
    }


    // ============================================
    // Player Bulletとの衝突
    // ============================================

    bool IsHitByBullet(
        const Vector3& start,
        const Vector3& end,
        float bulletRadius
    ) const;


private:

    // ============================================
    // Playerとの衝突判定
    // ============================================

    bool IsHitPlayer(
        const Vector3& start,
        const Vector3& end,
        const Vector3& playerPosition,
        float playerRadius
    ) const;


private:

    Vector3 m_position =
        Vector3::Zero;


    Vector3 m_direction =
        Vector3::Zero;


    // ============================================
    // 発射者
    //
    // 自分自身の弾が
    // 自分に当たるのを防ぐ。
    // ============================================

    Enemy* m_owner =
        nullptr;


    // ============================================
    // Bullet性能
    // ============================================

    float m_speed =
        300.0f;


    float m_remainingTime =
        3.0f;


    // Tank・Shapeへ与えるDamage。
    int m_damage =
        1;


    // Bullet Penetration
    //
    // 弾そのもののHPとして扱う。
    int m_bulletHealth =
        100;


    // 弾同士で衝突した時のDamage。
    int m_collisionDamage =
        100;


    // ============================================
    // サイズ
    // ============================================

    float m_levelScale =
        1.0f;


    float m_radius =
        6.0f;
};