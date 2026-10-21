#pragma once


class TankStatsFormula
{
public:

    // ============================================
    // Max Health
    //
    // Diep.io:
    // 50 + 2 * (Level - 1)
    // + 20 * MaxHealthLevel
    // ============================================

    static int GetMaxHealth(
        int currentLevel,
        int maxHealthLevel
    );


    // ============================================
    // Health Regen
    //
    // 正確な内部式が確認できないため
    // 現在のBolv.io式を共通化。
    // ============================================

    static float GetHealthRegenPerSecond(
        int maxHP,
        int healthRegenLevel
    );


    // ============================================
    // Body Damage
    // ============================================

    static int GetBodyDamageVsTank(
        int bodyDamageLevel
    );


    static int GetBodyDamageVsShape(
        int bodyDamageLevel
    );


    static int GetBodyDamageVsProjectile(
        int bodyDamageLevel
    );


    // ============================================
    // Bullet Damage
    //
    // Diep.io Basic:
    //
    // Tank / Shape / Droneに対して
    //
    // 7 + 3 * Level
    //
    // Lv0 = 7
    // Lv7 = 28
    // ============================================

    static int GetBulletDamage(
        int bulletDamageLevel
    );


    // ============================================
    // Bullet Speed
    //
    // 正確な本家内部式は未確認。
    //
    // 本家の
    // 「最初の5ptが有効、
    //   6～7ptは伸びが小さめ」
    // という傾向をBolv.ioへ換算。
    // ============================================

    static float GetBulletSpeed(
        int bulletSpeedLevel
    );


    // ============================================
    // Bullet Penetration
    //
    // Diep.io Basic Bullet Health:
    //
    // 2 + 1.5 * Level
    //
    // Lv0 = 2.0
    // Lv1 = 3.5
    // Lv2 = 5.0
    // ...
    // Lv7 = 12.5
    //
    // Bolv.ioでは整数耐久値を使用するため
    // 100倍して扱う。
    //
    // Lv0 = 200
    // Lv1 = 350
    // ...
    // Lv7 = 1250
    // ============================================

    static int GetBulletPenetration(
        int bulletPenetrationLevel
    );


    // ============================================
    // Bullet同士のCollision Damage
    //
    // Diep.ioではBullet Damageが
    // Bullet / Trapへ与えるDamageは
    // 通常の25%。
    //
    // Basic:
    //
    // 通常Damage
    // 7 + 3 * Level
    //
    // Projectile Damage
    // (7 + 3 * Level) * 0.25
    //
    // Lv0 = 1.75
    // Lv1 = 2.50
    // Lv2 = 3.25
    // ...
    // Lv7 = 7.00
    //
    // Bolv.ioではBullet HPと同じ
    // 100倍スケールを使用。
    //
    // Lv0 = 175
    // Lv1 = 250
    // ...
    // Lv7 = 700
    // ============================================

    static int GetBulletCollisionDamage(
        int bulletDamageLevel
    );


    // ============================================
    // Reload
    //
    // Diep.io Basic:
    // 15tick - 1tick * Level
    //
    // 1tick ≒ 0.04秒
    //
    // Lv0 = 0.60秒
    // Lv7 = 0.32秒
    // ============================================

    static float GetReloadInterval(
        int reloadLevel
    );


    // ============================================
    // Movement Speed強化
    //
    // 測定値に近い
    // 1ptごと約1.07倍。
    // ============================================

    static float GetMovementSpeedStatMultiplier(
        int movementSpeedLevel
    );


    // ============================================
    // レベルによる移動速度低下
    //
    // 本家ではLv上昇に伴い低下することは確認済み。
    // 正確な内部式は未確認なのでBolv.io近似。
    // ============================================

    static float GetLevelMovementMultiplier(
        int currentLevel
    );


    // ============================================
    // 最終Movement倍率
    // ============================================

    static float GetMovementSpeedMultiplier(
        int currentLevel,
        int movementSpeedLevel
    );


    // ============================================
    // Level Scale
    //
    // 1Lvごとに1%拡大。
    // ============================================

    static float GetLevelScale(
        int currentLevel
    );


private:

    static int ClampStatLevel(
        int level,
        int maxLevel
    );


    static int ClampPlayerLevel(
        int level
    );
};