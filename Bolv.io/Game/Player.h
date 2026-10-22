#pragma once

#include "TankVisual.h"
#include "TankWeapon.h"
#include "PlayerProgress.h"
#include "PlayerHealth.h"
#include "PlayerScore.h"
#include "PlayerContact.h"
#include "PlayerStats.h"
#include "PlayerEvolution.h"


class Player : public IGameObject
{
public:

    bool Start();

    void Update();

    void Render(
        RenderContext& rc
    );


    const Vector3& GetPosition() const
    {
        return m_position;
    }


    const Vector3& GetAimDirection() const
    {
        return m_aimDirection;
    }


    // ============================================
    // レベルによる自機サイズ倍率
    // ============================================

    float GetLevelScale() const
    {
        float scale =
            1.0f;


        const int currentLevel =
            m_progress
            .GetLevel();


        for (int level = 1;
            level <
            currentLevel;
            ++level)
        {
            scale *=
                1.01f;
        }


        return scale;
    }


    // ============================================
    // 接触判定半径
    // ============================================

    float GetCollisionRadius() const
    {
        return
            20.0f *
            GetLevelScale();
    }


    // ============================================
    // Experience
    // ============================================

    void AddExperience(
        int amount
    )
    {
        if (m_health.IsDead())
        {
            return;
        }


        m_progress
            .AddExperience(
                amount
            );
    }


    // ============================================
    // Progress
    // ============================================

    const PlayerProgress& GetProgress() const
    {
        return m_progress;
    }


    // ============================================
    // Stats
    // ============================================

    const PlayerStats& GetStats() const
    {
        return m_stats;
    }


    // ============================================
    // Bullet
    // ============================================

    int GetBulletDamage() const
    {
        return
            m_stats
            .GetBulletDamageValue();
    }


    float GetReloadInterval() const
    {
        return
            m_stats
            .GetReloadInterval();
    }


    float GetBulletSpeed() const
    {
        return
            m_stats
            .GetBulletSpeedValue();
    }


    int GetBulletPenetration() const
    {
        return
            m_stats
            .GetBulletPenetrationValue();
    }


    int GetBulletCollisionDamage() const
    {
        return
            m_stats
            .GetBulletCollisionDamageValue();
    }


    // ============================================
    // Body Damage
    // ============================================

    int GetBodyDamage() const
    {
        return
            m_stats
            .GetBodyDamageVsShape();
    }


    int GetBodyDamageVsShape() const
    {
        return
            m_stats
            .GetBodyDamageVsShape();
    }


    int GetBodyDamageVsTank() const
    {
        return
            m_stats
            .GetBodyDamageVsTank();
    }


    int GetBodyDamageVsProjectile() const
    {
        return
            m_stats
            .GetBodyDamageVsProjectile();
    }


    // ============================================
    // Health
    // ============================================

    const PlayerHealth& GetHealth() const
    {
        return m_health;
    }


    void TakeDamage(
        int damage
    )
    {
        m_health
            .TakeDamage(
                damage
            );
    }


    // ============================================
    // Knockback
    // ============================================

    void AddKnockback(
        const Vector3& direction,
        float power
    )
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


        m_moveSpeed.x +=
            knockbackDirection.x *
            power;


        m_moveSpeed.z +=
            knockbackDirection.z *
            power;
    }


    // ============================================
    // Score
    // ============================================

    void AddScore(
        int amount
    )
    {
        if (m_health.IsDead())
        {
            return;
        }


        m_score
            .AddScore(
                amount
            );
    }


    int GetScore() const
    {
        return
            m_score
            .GetScore();
    }


    // ============================================
    // Stat Upgrade
    // ============================================

    bool UpgradeStat(
        PlayerStats::StatType type
    )
    {
        if (m_progress
            .GetUpgradePoints() <=
            0)
        {
            return false;
        }


        const bool isSmasher =
            m_evolution
            .GetCurrentClass() ==
            PlayerEvolution::Smasher;


        if (isSmasher &&
            !m_stats
            .IsSmasherStat(
                type
            ))
        {
            return false;
        }


        const int maxLevel =
            m_stats
            .GetMaxLevel(
                type,
                isSmasher
            );


        if (m_stats
            .GetLevel(
                type
            ) >=
            maxLevel)
        {
            return false;
        }


        if (!m_stats
            .Upgrade(
                type,
                maxLevel
            ))
        {
            return false;
        }


        m_progress
            .UseUpgradePoint();


        // ========================================
        // Max Healthを強化した場合
        // ========================================

        if (type ==
            PlayerStats::MaxHealth)
        {
            m_health
                .SetMaxHP(
                    m_stats
                    .GetMaxHealthValue(
                        m_progress
                        .GetLevel()
                    )
                );
        }


        return true;
    }


    // ============================================
    // Evolution
    // ============================================

    const PlayerEvolution& GetEvolution() const
    {
        return m_evolution;
    }


    PlayerEvolution& GetEvolutionMutable()
    {
        return m_evolution;
    }


private:

    void Move();


    void HandleEvolutionStatChanges();


private:

    TankVisual m_tankVisual;

    TankWeapon m_tankWeapon;

    CharacterController m_charaCon;


    PlayerProgress m_progress;

    PlayerHealth m_health;

    PlayerScore m_score;

    PlayerContact m_contact;

    PlayerStats m_stats;

    PlayerEvolution m_evolution;


    PlayerEvolution::TankClass m_previousTankClass =
        PlayerEvolution::Basic;


    // ============================================
    // Health Regen
    //
    // 小数HPの回復量を保持する。
    // ============================================

    float m_healthRegenAccumulator =
        0.0f;


    bool m_debugLevelUpKeyPressed =
        false;


    Vector3 m_position =
        Vector3(
            0.0f,
            50.0f,
            0.0f
        );


    Vector3 m_moveSpeed =
        Vector3::Zero;


    Vector3 m_aimDirection =
        Vector3(
            0.0f,
            0.0f,
            1.0f
        );
};