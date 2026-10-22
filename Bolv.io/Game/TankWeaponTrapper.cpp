#include "stdafx.h"

#include "TankWeapon.h"
#include "Trap.h"
#include "Player.h"

#include <cmath>


namespace
{
    // ========================================================
    // Trapper 本家値
    // ========================================================

    // Reload Lv0～7
    //
    // 23,21,19,18,16,15,14,12 tick
    const int TRAPPER_RELOAD_TICKS[8] =
    {
        23,
        21,
        19,
        18,
        16,
        15,
        14,
        12
    };


    // Diep.ioでは1tick = 0.04秒。
    const float DIEP_TICK_SECONDS =
        0.04f;


    // Bullet Speed
    // Tank基準の2倍。
    const float TRAPPER_SPEED_MULTIPLIER =
        2.0f;


    // Launcher先端。
    const float TRAPPER_MUZZLE_DISTANCE =
        48.0f;
}


// ============================================================
// FireTrapper
// ============================================================

void TankWeapon::FireTrapper(
    const Vector3& playerPosition,
    const Vector3& aimDirection
)
{
    Player* player =
        FindGO<Player>(
            "player"
        );


    if (player ==
        nullptr)
    {
        return;
    }


    Vector3 direction =
        aimDirection;


    direction.y =
        0.0f;


    const float directionLengthSquared =
        direction.x *
        direction.x +
        direction.z *
        direction.z;


    if (directionLengthSquared <=
        0.0001f)
    {
        return;
    }


    direction.Normalize();


    // ========================================================
    // Stat Level
    // ========================================================

    const int bulletDamageLevel =
        player
        ->GetStats()
        .GetLevel(
            PlayerStats::BulletDamage
        );


    const int bulletPenetrationLevel =
        player
        ->GetStats()
        .GetLevel(
            PlayerStats::BulletPenetration
        );


    // ========================================================
    // Trap Damage
    //
    // 本家:
    //
    // 8.4 + 3.6 × Bullet Damage
    //
    // Bolv.ioはintなので四捨五入。
    // ========================================================

    const int damage =
        static_cast<int>(
            8.4f +
            3.6f *
            static_cast<float>(
                bulletDamageLevel
                ) +
            0.5f
            );


    // ========================================================
    // Trap Penetration
    //
    // 本家:
    //
    // 3 + 2.25 × Bullet Penetration
    //
    // Bolv.ioは100倍内部スケール。
    // ========================================================

    const int penetration =
        300 +
        225 *
        bulletPenetrationLevel;


    // ========================================================
    // Projectile同士のDamage
    //
    // Damageの25%を100倍スケール。
    // ========================================================

    const int collisionDamage =
        210 +
        90 *
        bulletDamageLevel;


    // ========================================================
    // Speed
    // ========================================================

    const float speed =
        player
        ->GetBulletSpeed() *
        TRAPPER_SPEED_MULTIPLIER;


    // ========================================================
    // Level Scale
    // ========================================================

    const int currentLevel =
        player
        ->GetProgress()
        .GetLevel();


    const float levelScale =
        std::pow(
            1.01f,
            static_cast<float>(
                currentLevel -
                1
                )
        );


    // ========================================================
    // 発射位置
    // ========================================================

    Vector3 firePosition =
        playerPosition;


    firePosition.y +=
        30.0f *
        levelScale;


    firePosition +=
        direction *
        (
            TRAPPER_MUZZLE_DISTANCE *
            levelScale
            );


    // ========================================================
    // Trap生成
    // ========================================================

    Trap* trap =
        NewGO<Trap>(
            0,
            "trap"
        );


    if (trap ==
        nullptr)
    {
        return;
    }


    trap->Init(
        firePosition,
        direction,
        damage,
        speed,
        penetration,
        collisionDamage,
        levelScale
    );


    m_twinFireLeft =
        true;
}


// ============================================================
// GetTrapperReloadInterval
// ============================================================

float TankWeapon::GetTrapperReloadInterval(
    int reloadLevel
) const
{
    if (reloadLevel <
        0)
    {
        reloadLevel =
            0;
    }


    if (reloadLevel >
        7)
    {
        reloadLevel =
            7;
    }


    return
        static_cast<float>(
            TRAPPER_RELOAD_TICKS[
                reloadLevel
            ]
            ) *
        DIEP_TICK_SECONDS;
}