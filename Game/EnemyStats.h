#pragma once

#include "TankStatsFormula.h"


class EnemyStats
{
public:

    enum StatType
    {
        HealthRegen,
        MaxHealth,
        BodyDamage,
        BulletSpeed,
        BulletPenetration,
        BulletDamage,
        Reload,
        MovementSpeed,

        StatCount
    };


    EnemyStats();


    void Reset();


    bool Upgrade(
        StatType type,
        int maxLevel
    );


    int GetLevel(
        StatType type
    ) const
    {
        return
            m_levels[type];
    }


    // ============================================
    // Max Health
    // ============================================

    int GetMaxHealthValue(
        int currentLevel
    ) const
    {
        return
            TankStatsFormula
            ::GetMaxHealth(
                currentLevel,
                m_levels[
                    MaxHealth
                ]
            );
    }


    // ============================================
    // Health Regen
    // ============================================

    float GetHealthRegenPerSecond(
        int maxHP
    ) const
    {
        return
            TankStatsFormula
            ::GetHealthRegenPerSecond(
                maxHP,
                m_levels[
                    HealthRegen
                ]
            );
    }


    // ============================================
    // Body Damage
    // ============================================

    int GetBodyDamageVsTank() const
    {
        return
            TankStatsFormula
            ::GetBodyDamageVsTank(
                m_levels[
                    BodyDamage
                ]
            );
    }


    int GetBodyDamageVsShape() const
    {
        return
            TankStatsFormula
            ::GetBodyDamageVsShape(
                m_levels[
                    BodyDamage
                ]
            );
    }


    int GetBodyDamageVsProjectile() const
    {
        return
            TankStatsFormula
            ::GetBodyDamageVsProjectile(
                m_levels[
                    BodyDamage
                ]
            );
    }


    // ============================================
    // Movement Speed
    // ============================================

    float GetMovementSpeedMultiplier(
        int currentLevel
    ) const
    {
        return
            TankStatsFormula
            ::GetMovementSpeedMultiplier(
                currentLevel,
                m_levels[
                    MovementSpeed
                ]
            );
    }


    // 既存コード互換。
    float GetMovementSpeedMultiplier() const
    {
        return
            TankStatsFormula
            ::GetMovementSpeedStatMultiplier(
                m_levels[
                    MovementSpeed
                ]
            );
    }


    // ============================================
    // Bullet Damage
    // ============================================

    int GetBulletDamageValue() const
    {
        return
            TankStatsFormula
            ::GetBulletDamage(
                m_levels[
                    BulletDamage
                ]
            );
    }


    // ============================================
    // Bullet Speed
    // ============================================

    float GetBulletSpeedValue() const
    {
        return
            TankStatsFormula
            ::GetBulletSpeed(
                m_levels[
                    BulletSpeed
                ]
            );
    }


    // ============================================
    // Bullet Penetration
    // ============================================

    int GetBulletPenetrationValue() const
    {
        return
            TankStatsFormula
            ::GetBulletPenetration(
                m_levels[
                    BulletPenetration
                ]
            );
    }


    // ============================================
    // Bullet Collision Damage
    // ============================================

    int GetBulletCollisionDamageValue() const
    {
        return
            TankStatsFormula
            ::GetBulletCollisionDamage(
                m_levels[
                    BulletDamage
                ]
            );
    }


    // ============================================
    // Reload
    // ============================================

    float GetReloadInterval() const
    {
        return
            TankStatsFormula
            ::GetReloadInterval(
                m_levels[
                    Reload
                ]
            );
    }


private:

    int m_levels[
        StatCount
    ];
};