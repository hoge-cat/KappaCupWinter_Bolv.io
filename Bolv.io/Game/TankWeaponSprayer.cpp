
#include "stdafx.h"
#include "TankWeapon.h"
#include "Player.h"

#include <cmath>
#include <cstdlib>


namespace
{
    // ========================================================
    // Sprayer
    //
    // 1tick = 0.04秒
    //
    // 外側：8tick = 0.32秒
    // 内側：15tick = 0.60秒
    //
    // Reloadステータスによって発射間隔を短縮する。
    // ========================================================

    const float DIEP_TICK_SECONDS = 0.04f;

    const int SPRAYER_OUTER_RELOAD_TICKS[8] =
    {
        8, 7, 7, 6, 6, 5, 5, 4
    };

    const int SPRAYER_INNER_RELOAD_TICKS[8] =
    {
        15, 14, 13, 12, 11, 10, 9, 8
    };


    // ========================================================
    // 弾の拡散角度
    //
    // 外側：±15度
    // 内側：±5度
    // ========================================================

    const float SPRAYER_OUTER_SPREAD = 10.0f;
    const float SPRAYER_INNER_SPREAD = 3.0f;


    // ========================================================
    // 弾の大きさ・ダメージ
    // ========================================================

    const float SPRAYER_OUTER_BULLET_SIZE = 1.0f;
    const float SPRAYER_INNER_BULLET_SIZE = 0.7f;

    const float SPRAYER_OUTER_DAMAGE = 0.7f;
    const float SPRAYER_INNER_DAMAGE = 0.1f;


    // ========================================================
    // 指定範囲のランダム角度を取得
    // ========================================================

    float RandomSpread(float maxAngle)
    {
        const float randomValue =
            static_cast<float>(std::rand()) /
            static_cast<float>(RAND_MAX);

        return (randomValue * 2.0f - 1.0f) *
            maxAngle;
    }
}


// ============================================================
// GetSprayerOuterReloadInterval
// ============================================================

float TankWeapon::GetSprayerOuterReloadInterval(
    int reloadLevel
) const
{
    if (reloadLevel < 0)
    {
        reloadLevel = 0;
    }

    if (reloadLevel > 7)
    {
        reloadLevel = 7;
    }

    return static_cast<float>(
        SPRAYER_OUTER_RELOAD_TICKS[reloadLevel]
        ) * DIEP_TICK_SECONDS;
}


// ============================================================
// GetSprayerInnerReloadInterval
// ============================================================

float TankWeapon::GetSprayerInnerReloadInterval(
    int reloadLevel
) const
{
    if (reloadLevel < 0)
    {
        reloadLevel = 0;
    }

    if (reloadLevel > 7)
    {
        reloadLevel = 7;
    }

    return static_cast<float>(
        SPRAYER_INNER_RELOAD_TICKS[reloadLevel]
        ) * DIEP_TICK_SECONDS;
}


// ============================================================
// FireSprayerOuter
//
// Machine Gun砲身から発射。
// ============================================================

void TankWeapon::FireSprayerOuter(
    const Vector3& playerPosition,
    const Vector3& aimDirection
)
{
    const float angleOffset =
        RandomSpread(SPRAYER_OUTER_SPREAD);

    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        angleOffset,
        SPRAYER_OUTER_BULLET_SIZE,
        1.0f,
        SPRAYER_OUTER_DAMAGE
    );
}


// ============================================================
// FireSprayerInner
//
// 中央の細い砲身から発射。
// ============================================================

void TankWeapon::FireSprayerInner(
    const Vector3& playerPosition,
    const Vector3& aimDirection
)
{
    const float angleOffset =
        RandomSpread(SPRAYER_INNER_SPREAD);

    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        angleOffset,
        SPRAYER_INNER_BULLET_SIZE,
        1.0f,
        SPRAYER_INNER_DAMAGE
    );
}


// ============================================================
// UpdateSprayer
//
// 2つの砲身のリロードを独立して管理する。
// ============================================================

void TankWeapon::UpdateSprayer(
    const Vector3& playerPosition,
    const Vector3& aimDirection,
    float deltaTime
)
{
    Player* player =
        FindGO<Player>("player");

    if (player == nullptr)
    {
        return;
    }

    int reloadLevel =
        player->GetStats().GetLevel(
            PlayerStats::Reload
        );

    // ========================================================
    // リロード時間を更新
    // ========================================================

    m_sprayerOuterRemainingTime -= deltaTime;
    m_sprayerInnerRemainingTime -= deltaTime;


    // ========================================================
    // 外側：Machine Gun砲身
    // ========================================================

    if (m_sprayerOuterRemainingTime <= 0.0f)
    {
        FireSprayerOuter(
            playerPosition,
            aimDirection
        );

        m_sprayerOuterRemainingTime =
            GetSprayerOuterReloadInterval(
                reloadLevel
            );
    }


    // ========================================================
    // 内側：中央砲身
    // ========================================================

    if (m_sprayerInnerRemainingTime <= 0.0f)
    {
        FireSprayerInner(
            playerPosition,
            aimDirection
        );

        m_sprayerInnerRemainingTime =
            GetSprayerInnerReloadInterval(
                reloadLevel
            );
    }
}
