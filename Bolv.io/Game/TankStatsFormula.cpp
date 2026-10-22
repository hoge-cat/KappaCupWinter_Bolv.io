#include "stdafx.h"
#include "TankStatsFormula.h"

#include <cmath>


// ============================================================
// ClampStatLevel
// ============================================================

int TankStatsFormula::ClampStatLevel(
    int level,
    int maxLevel)
{
    if (level <
        0)
    {
        return 0;
    }


    if (level >
        maxLevel)
    {
        return maxLevel;
    }


    return level;
}


// ============================================================
// ClampPlayerLevel
// ============================================================

int TankStatsFormula::ClampPlayerLevel(
    int level)
{
    if (level <
        1)
    {
        return 1;
    }


    if (level >
        45)
    {
        return 45;
    }


    return level;
}


// ============================================================
// Max Health
// ============================================================

int TankStatsFormula::GetMaxHealth(
    int currentLevel,
    int maxHealthLevel)
{
    currentLevel =
        ClampPlayerLevel(
            currentLevel
        );


    maxHealthLevel =
        ClampStatLevel(
            maxHealthLevel,
            10
        );


    const int baseHealth =
        50 +
        2 *
        (
            currentLevel -
            1
            );


    const int upgradeHealth =
        20 *
        maxHealthLevel;


    return
        baseHealth +
        upgradeHealth;
}


// ============================================================
// Health Regen
// ============================================================

float TankStatsFormula::GetHealthRegenPerSecond(
    int maxHP,
    int healthRegenLevel)
{
    if (maxHP <=
        0)
    {
        return 0.0f;
    }


    healthRegenLevel =
        ClampStatLevel(
            healthRegenLevel,
            10
        );


    return
        static_cast<float>(
            maxHP
            ) /
        30.0f *
        (
            0.03f +
            0.12f *
            static_cast<float>(
                healthRegenLevel
                )
            );
}


// ============================================================
// Body Damage - Tank
// ============================================================

int TankStatsFormula::GetBodyDamageVsTank(
    int bodyDamageLevel)
{
    bodyDamageLevel =
        ClampStatLevel(
            bodyDamageLevel,
            10
        );


    return
        30 +
        6 *
        bodyDamageLevel;
}


// ============================================================
// Body Damage - Shape
// ============================================================

int TankStatsFormula::GetBodyDamageVsShape(
    int bodyDamageLevel)
{
    bodyDamageLevel =
        ClampStatLevel(
            bodyDamageLevel,
            10
        );


    return
        20 +
        4 *
        bodyDamageLevel;
}


// ============================================================
// Body Damage - Projectile
// ============================================================

int TankStatsFormula::GetBodyDamageVsProjectile(
    int bodyDamageLevel)
{
    bodyDamageLevel =
        ClampStatLevel(
            bodyDamageLevel,
            10
        );


    return
        5 +
        bodyDamageLevel;
}


// ============================================================
// Bullet Damage
// ============================================================

int TankStatsFormula::GetBulletDamage(
    int bulletDamageLevel)
{
    bulletDamageLevel =
        ClampStatLevel(
            bulletDamageLevel,
            7
        );


    // ============================================
    // Diep.io Basic Tank
    //
    // Tank / Shape / Droneへ与えるDamage。
    //
    // Base = 7
    // +3 per point
    //
    // Lv0 = 7
    // Lv1 = 10
    // Lv2 = 13
    // Lv3 = 16
    // Lv4 = 19
    // Lv5 = 22
    // Lv6 = 25
    // Lv7 = 28
    // ============================================

    return
        7 +
        3 *
        bulletDamageLevel;
}


// ============================================================
// Bullet Speed
// ============================================================

float TankStatsFormula::GetBulletSpeed(
    int bulletSpeedLevel)
{
    bulletSpeedLevel =
        ClampStatLevel(
            bulletSpeedLevel,
            7
        );


    // ============================================
    // Bolv.io換算
    //
    // 0～5:
    // 1ptごとに約12%
    //
    // 6～7:
    // 伸びを半分程度に落とす。
    //
    // 本家の
    // 「最初の5ptが有効、
    //   6～7ptは伸びが小さめ」
    // 傾向を再現。
    // ============================================

    float multiplier =
        1.0f;


    for (int i = 0;
        i < bulletSpeedLevel;
        ++i)
    {
        if (i <
            5)
        {
            multiplier +=
                0.12f;
        }
        else
        {
            multiplier +=
                0.06f;
        }
    }


    // ============================================
    // Bolv.io座標系基準速度
    // ============================================

    return
        300.0f *
        multiplier;
}


// ============================================================
// Bullet Penetration
// ============================================================

int TankStatsFormula::GetBulletPenetration(
    int bulletPenetrationLevel)
{
    bulletPenetrationLevel =
        ClampStatLevel(
            bulletPenetrationLevel,
            7
        );


    // ============================================
    // Diep.io Basic Bullet Health
    //
    // 2 + 1.5 * Level
    //
    // Bolv.ioでは100倍。
    //
    // Lv0
    // 2.0 → 200
    //
    // Lv1
    // 3.5 → 350
    //
    // Lv2
    // 5.0 → 500
    //
    // Lv3
    // 6.5 → 650
    //
    // Lv4
    // 8.0 → 800
    //
    // Lv5
    // 9.5 → 950
    //
    // Lv6
    // 11.0 → 1100
    //
    // Lv7
    // 12.5 → 1250
    // ============================================

    return
        200 +
        150 *
        bulletPenetrationLevel;
}


// ============================================================
// Bullet Collision Damage
// ============================================================

int TankStatsFormula::GetBulletCollisionDamage(
    int bulletDamageLevel)
{
    bulletDamageLevel =
        ClampStatLevel(
            bulletDamageLevel,
            7
        );


    // ============================================
    // Diep.io Basic
    //
    // 通常Bullet Damage:
    //
    // 7 + 3 * Level
    //
    // Bullet / Trapへ与えるDamageは
    // 75%減少
    // = 通常Damageの25%
    //
    //
    // Lv0
    //
    // 7 × 0.25
    // = 1.75
    //
    //
    // Lv1
    //
    // 10 × 0.25
    // = 2.50
    //
    //
    // Lv2
    //
    // 13 × 0.25
    // = 3.25
    //
    //
    // ...
    //
    // Lv7
    //
    // 28 × 0.25
    // = 7.00
    //
    //
    // Bolv.ioではBullet Penetrationを
    // 100倍スケールで保持しているため、
    // Collision Damageも100倍する。
    //
    //
    // Lv0 = 175
    // Lv1 = 250
    // Lv2 = 325
    // Lv3 = 400
    // Lv4 = 475
    // Lv5 = 550
    // Lv6 = 625
    // Lv7 = 700
    // ============================================

    return
        175 +
        75 *
        bulletDamageLevel;
}


// ============================================================
// Reload
// ============================================================

float TankStatsFormula::GetReloadInterval(
    int reloadLevel)
{
    reloadLevel =
        ClampStatLevel(
            reloadLevel,
            7
        );


    // ============================================
    // Basic Tank
    //
    // 15tick → 8tick
    //
    // 1tick = 0.04 sec
    //
    // Lv0 = 0.60
    // Lv1 = 0.56
    // Lv2 = 0.52
    // Lv3 = 0.48
    // Lv4 = 0.44
    // Lv5 = 0.40
    // Lv6 = 0.36
    // Lv7 = 0.32
    // ============================================

    const int ticks =
        15 -
        reloadLevel;


    return
        static_cast<float>(
            ticks
            ) *
        0.04f;
}


// ============================================================
// Movement Speed Stat Multiplier
// ============================================================

float TankStatsFormula::GetMovementSpeedStatMultiplier(
    int movementSpeedLevel)
{
    movementSpeedLevel =
        ClampStatLevel(
            movementSpeedLevel,
            10
        );


    // ============================================
    // 約1.07倍ずつ。
    //
    // Lv7 ≒ 1.61倍
    // Lv10 ≒ 1.97倍
    // ============================================

    return
        std::pow(
            1.07f,
            static_cast<float>(
                movementSpeedLevel
                )
        );
}


// ============================================================
// Level Movement Multiplier
// ============================================================

float TankStatsFormula::GetLevelMovementMultiplier(
    int currentLevel)
{
    currentLevel =
        ClampPlayerLevel(
            currentLevel
        );


    // ============================================
    // 本家：
    //
    // Lv上昇で基礎移動速度が下がる。
    //
    // 正確な式は未確認なので、
    // Bolv.ioでは
    //
    // Lv1  = 1.00
    // Lv45 = 0.75
    //
    // として近似。
    // ============================================

    const float levelRate =
        static_cast<float>(
            currentLevel -
            1
            ) /
        44.0f;


    return
        1.0f -
        0.25f *
        levelRate;
}


// ============================================================
// Movement Speed
// ============================================================

float TankStatsFormula::GetMovementSpeedMultiplier(
    int currentLevel,
    int movementSpeedLevel)
{
    return
        GetLevelMovementMultiplier(
            currentLevel
        ) *
        GetMovementSpeedStatMultiplier(
            movementSpeedLevel
        );
}


// ============================================================
// Level Scale
// ============================================================

float TankStatsFormula::GetLevelScale(
    int currentLevel)
{
    currentLevel =
        ClampPlayerLevel(
            currentLevel
        );


    return
        std::pow(
            1.01f,
            static_cast<float>(
                currentLevel -
                1
                )
        );
}