#pragma once


class ShapeBox : public IGameObject
{
public:

    bool Start();

    void Update();

    void Render(
        RenderContext& rc
    );


    void SetPosition(
        const Vector3& position
    )
    {
        m_position =
            position;
    }


    // ============================================
    // Position
    // ============================================

    const Vector3& GetPosition() const
    {
        return
            m_position;
    }


    // ============================================
    // HP
    // ============================================

    int GetHP() const
    {
        return
            m_hp;
    }


    int GetMaxHP() const
    {
        return
            MAX_HP;
    }


    bool IsDamaged() const
    {
        return
            m_hp >
            0 &&
            m_hp <
            MAX_HP;
    }


    bool IsAlive() const
    {
        return
            m_hp >
            0;
    }


    // ============================================
    // Square Body Damage
    //
    // Diep.io Square:
    //
    // HP = 10
    // Body Damage = 8
    // ============================================

    int GetBodyDamage() const
    {
        return
            BODY_DAMAGE;
    }


    // ============================================
    // 弾との衝突
    // ============================================

    bool IsHit(
        const Vector3& start,
        const Vector3& end,
        float bulletRadius
    ) const;


    // ============================================
    // Tankとの衝突
    // ============================================

    bool IsTouch(
        const Vector3& center,
        float radius
    ) const;


    // ============================================
    // Damage
    // ============================================

    void TakeDamage(
        int damage,
        bool giveReward = true
    );


private:

    // ============================================
    // Diep.io Square
    // ============================================

    static const int MAX_HP =
        10;


    static const int BODY_DAMAGE =
        8;


private:

    Vector3 m_position =
        Vector3::Zero;


    int m_hp =
        MAX_HP;


    float m_respawnTimer =
        0.0f;
};