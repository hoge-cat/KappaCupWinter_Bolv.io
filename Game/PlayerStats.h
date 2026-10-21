#pragma once

#include "TankStatsFormula.h"


class PlayerStats
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


    // ============================================
    // Upgrade
    // ============================================

    bool Upgrade(
        StatType type
    );


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


    int GetMaxLevel() const
    {
        return 7;
    }


    int GetMaxLevel(
        StatType type,
        bool isSmasher
    ) const;


    bool IsSmasherStat(
        StatType type
    ) const;


    bool IsBulletStat(
        StatType type
    ) const;


    int ResetBulletStats();


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
    // Movement Speed
    //
    // レベル低下まで含める場合はこちら。
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


    // ============================================
    // 既存コード互換
    //
    // Level補正なし。
    // ============================================

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
    // Body Damage
    // ============================================

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
    // 既存コード互換
    // ============================================

    int GetBodyDamageValue() const
    {
        return
            GetBodyDamageVsShape();
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


private:

    int m_levels[
        StatCount
    ] =
        {
            0, // HealthRegen
            0, // MaxHealth
            0, // BodyDamage
            0, // BulletSpeed
            0, // BulletPenetration
            0, // BulletDamage
            0, // Reload
            0  // MovementSpeed
        };
};