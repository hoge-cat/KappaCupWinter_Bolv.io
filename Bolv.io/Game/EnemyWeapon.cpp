#include "stdafx.h"
#include "EnemyWeapon.h"

#include "Enemy.h"
#include "EnemyBullet.h"

#include <cmath>
#include <cstdlib>


namespace
{
    // ============================================
    // Player側と合わせたBolv.io調整値
    // ============================================

    const float TWIN_BARREL_OFFSET =
        11.0f;


    const float SNIPER_BULLET_SPEED_MULTIPLIER =
        1.35f;


    const float SNIPER_RELOAD_MULTIPLIER =
        1.35f;


    const float SNIPER_MUZZLE_MULTIPLIER =
        1.35f;


    const float MACHINE_GUN_RELOAD_MULTIPLIER =
        0.45f;


    const float MACHINE_GUN_SPREAD_DEG =
        6.0f;


    // SprayerはMachine Gunよりさらに
    // 弾幕を作る。
    const float SPRAYER_RELOAD_MULTIPLIER =
        0.40f;


    const float SPRAYER_SPREAD_DEG =
        6.0f;
}


// ============================================================
// Update
// ============================================================

void EnemyWeapon::Update(
    Enemy& enemy,
    const Vector3& position,
    const Vector3& direction
)
{
    m_remainingTime -=
        g_gameTime
        ->GetFrameDeltaTime();


    if (m_remainingTime >
        0.0f)
    {
        return;
    }


    Vector3 aimDirection =
        direction;


    aimDirection.y =
        0.0f;


    const float lengthSquared =
        aimDirection.x *
        aimDirection.x +
        aimDirection.z *
        aimDirection.z;


    if (lengthSquared <=
        0.0001f)
    {
        return;
    }


    aimDirection.Normalize();


    const EnemyEvolution::TankClass tankClass =
        enemy
        .GetTankClass();


    // ========================================================
    // Smasher
    // ========================================================

    if (tankClass ==
        EnemyEvolution::Smasher)
    {
        m_twinFireLeft =
            true;


        m_sprayerSecondShot =
            false;


        return;
    }


    float reloadInterval =
        enemy
        .GetReloadInterval();


    // ========================================================
    // Twin
    //
    // 左右を交互に発射。
    // ========================================================

    if (tankClass ==
        EnemyEvolution::Twin)
    {
        float sideOffset =
            TWIN_BARREL_OFFSET;


        if (m_twinFireLeft)
        {
            sideOffset =
                -TWIN_BARREL_OFFSET;
        }


        Fire(
            enemy,
            position,
            aimDirection,
            sideOffset
        );


        m_twinFireLeft =
            !m_twinFireLeft;


        reloadInterval *=
            0.5f;
    }


    // ========================================================
    // Sniper
    // ========================================================

    else if (
        tankClass ==
        EnemyEvolution::Sniper)
    {
        Fire(
            enemy,
            position,
            aimDirection
        );


        reloadInterval *=
            SNIPER_RELOAD_MULTIPLIER;
    }


    // ========================================================
    // Machine Gun
    // ========================================================

    else if (
        tankClass ==
        EnemyEvolution::MachineGun)
    {
        const float random01 =
            static_cast<float>(
                std::rand()
                ) /
            static_cast<float>(
                RAND_MAX
                );


        const float spreadAngle =
            (
                random01 *
                2.0f -
                1.0f
                ) *
            MACHINE_GUN_SPREAD_DEG;


        Fire(
            enemy,
            position,
            aimDirection,
            0.0f,
            spreadAngle
        );


        reloadInterval *=
            MACHINE_GUN_RELOAD_MULTIPLIER;


        m_twinFireLeft =
            true;
    }


    // ========================================================
    // Flank Guard
    //
    // 本家同様、前後同時発射。
    // ========================================================

    else if (
        tankClass ==
        EnemyEvolution::FlankGuard)
    {
        Fire(
            enemy,
            position,
            aimDirection
        );


        Vector3 backDirection =
            aimDirection *
            -1.0f;


        Fire(
            enemy,
            position,
            backDirection
        );
    }


    // ========================================================
    // Sprayer
    //
    // 本家の基本パターン：
    //
    // 1回目
    // 大弾
    //
    // 2回目
    // 大弾 + 小弾
    //
    // → 合計3発の周期
    // ========================================================

    else if (
        tankClass ==
        EnemyEvolution::Sprayer)
    {
        // ========================================
        // 大砲側
        // ========================================

        const float random01 =
            static_cast<float>(
                std::rand()
                ) /
            static_cast<float>(
                RAND_MAX
                );


        const float spreadAngle =
            (
                random01 *
                2.0f -
                1.0f
                ) *
            SPRAYER_SPREAD_DEG;


        Fire(
            enemy,
            position,
            aimDirection,
            0.0f,
            spreadAngle
        );


        // ========================================
        // 2回目は中央の小弾も同時発射
        // ========================================

        if (m_sprayerSecondShot)
        {
            Fire(
                enemy,
                position,
                aimDirection,
                0.0f,
                0.0f,

                // 本家では小弾がかなり弱い。
                // 現在のBolv.io整数ダメージへ
                // 落とし込むため約1/7。
                1.0f /
                7.0f,

                0.65f
            );
        }


        m_sprayerSecondShot =
            !m_sprayerSecondShot;


        reloadInterval *=
            SPRAYER_RELOAD_MULTIPLIER;
    }


    // ========================================================
    // Basic
    // ========================================================

    else
    {
        Fire(
            enemy,
            position,
            aimDirection
        );


        m_twinFireLeft =
            true;


        m_sprayerSecondShot =
            false;
    }


    // ============================================
    // 安全下限
    // ============================================

    if (reloadInterval <
        0.04f)
    {
        reloadInterval =
            0.04f;
    }


    m_remainingTime =
        reloadInterval;
}


// ============================================================
// Fire
// ============================================================

void EnemyWeapon::Fire(
    Enemy& enemy,
    const Vector3& position,
    const Vector3& originalDirection,
    float sideOffset,
    float spreadAngleDeg,
    float damageMultiplier,
    float bulletScaleMultiplier
)
{
    Vector3 direction =
        originalDirection;


    direction.y =
        0.0f;


    if (
        direction.x *
        direction.x +
        direction.z *
        direction.z <=
        0.0001f)
    {
        return;
    }


    direction.Normalize();


    const EnemyEvolution::TankClass tankClass =
        enemy
        .GetTankClass();


    // ============================================
    // 拡散
    // ============================================

    if (spreadAngleDeg !=
        0.0f)
    {
        const float radian =
            spreadAngleDeg *
            3.14159265f /
            180.0f;


        const float cosValue =
            std::cos(
                radian
            );


        const float sinValue =
            std::sin(
                radian
            );


        const float originalX =
            direction.x;


        const float originalZ =
            direction.z;


        direction.x =
            originalX *
            cosValue +
            originalZ *
            sinValue;


        direction.z =
            -originalX *
            sinValue +
            originalZ *
            cosValue;


        direction.Normalize();
    }


    const float levelScale =
        enemy
        .GetLevelScale();


    // ============================================
    // 右方向
    // ============================================

    Vector3 right(
        direction.z,
        0.0f,
        -direction.x
    );


    if (
        right.x *
        right.x +
        right.z *
        right.z >
        0.0001f)
    {
        right.Normalize();
    }


    // ============================================
    // 発射位置
    // ============================================

    Vector3 firePosition =
        position;


    firePosition.y +=
        30.0f *
        levelScale;


    firePosition +=
        right *
        (
            sideOffset *
            levelScale
            );


    float muzzleDistance =
        56.0f;


    if (tankClass ==
        EnemyEvolution::Sniper)
    {
        muzzleDistance *=
            SNIPER_MUZZLE_MULTIPLIER;
    }


    firePosition +=
        direction *
        (
            muzzleDistance *
            levelScale
            );


    // ============================================
    // 弾性能
    // ============================================

    int damage =
        enemy
        .GetBulletDamage();


    float speed =
        enemy
        .GetBulletSpeed();


    int penetration =
        enemy
        .GetBulletPenetration();


    int collisionDamage =
        enemy
        .GetBulletCollisionDamage();


    // ============================================
    // Sniper
    // ============================================

    if (tankClass ==
        EnemyEvolution::Sniper)
    {
        speed *=
            SNIPER_BULLET_SPEED_MULTIPLIER;
    }


    // ============================================
    // 小弾などの補正
    // ============================================

    damage =
        static_cast<int>(
            static_cast<float>(
                damage
                ) *
            damageMultiplier
            );


    collisionDamage =
        static_cast<int>(
            static_cast<float>(
                collisionDamage
                ) *
            damageMultiplier
            );


    // 最低1ダメージ。
    if (damage <
        1)
    {
        damage =
            1;
    }


    if (collisionDamage <
        1)
    {
        collisionDamage =
            1;
    }


    // ============================================
    // 弾生成
    // ============================================

    EnemyBullet* bullet =
        NewGO<EnemyBullet>(
            0,
            "enemyBullet"
        );


    if (bullet ==
        nullptr)
    {
        return;
    }


    bullet->Init(
        firePosition,
        direction,
        damage,
        speed,
        penetration,
        collisionDamage,
        levelScale *
        bulletScaleMultiplier,
        &enemy
    );
}