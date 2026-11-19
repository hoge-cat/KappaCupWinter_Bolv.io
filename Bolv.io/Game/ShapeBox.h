#pragma once


class ShapeBox : public IGameObject
{
public:
    enum class Type { Square, Triangle, Pentagon, Count };
    void SetType(Type type);
    Type GetType() const { return m_type; }
    int GetExperienceReward() const;
    float GetCollisionRadius() const;
    float GetRenderScale() const;
    float GetHeight() const;
    Vector4 GetColor() const;

    bool Start();

    void Update();

    // 見た目・当たり判定・押し戻しで同じ中心と回転を使う。
    Vector3 GetCenter() const { return m_position + Vector3(0, GetHeight() * 0.5f, 21); }
    Quaternion GetRotation() const;
    Vector3 GetRenderPosition() const;
    void AddImpulse(const Vector3& direction, float strength);

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


    int GetMaxHP() const;


    bool IsDamaged() const
    {
        return
            m_hp >
            0 &&
            m_hp <
            GetMaxHP();
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
            GetBodyDamageValue();
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

    int GetBodyDamageValue() const;
    bool IsPolygonHit(const Vector3& start, const Vector3& end, float radius) const;
    bool IsPolygonTouch(const Vector3& center, float radius) const;
    Type m_type = Type::Square;

    void UpdateMotion(float deltaTime);
    Vector3 ToCollisionSpace(const Vector3& point) const;

    float m_driftAngle = 0.0f;
    float m_driftSpeed = 0.0f;
    float m_turnSpeed = 0.0f;
    float m_rotationAngle = 0.0f;
    float m_spinSpeed = 0.0f;
    Vector3 m_pushVelocity = Vector3::Zero;

    Vector3 m_position =
        Vector3::Zero;


    int m_hp =
        MAX_HP;


    float m_respawnTimer =
        0.0f;
};