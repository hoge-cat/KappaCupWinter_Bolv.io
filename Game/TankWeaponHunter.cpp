#include "stdafx.h"
#include "TankWeapon.h"

#include "Player.h"

#include <cstdlib>


namespace
{
    // ========================================================
    // Hunter
    //
    // 公開されているDiep.ioの仕様をもとにした値。
    //
    // ・2本の砲身は重なっている
    // ・小さい弾 → 大きい弾 の順で素早く2連射
    // ・両方の弾速は同じ
    // ・Bullet DamageはSniper系通常弾より25%低い
    // ・小さい砲身側にはわずかな拡散がある
    //
    // 2発の正確な時間差そのものは公開資料で
    // 明確なtick値を確認できなかったため、
    // 「quick succession」をBolv.ioで再現する値として
    // 2tick = 0.08秒を採用する。
    // ========================================================

    const float HUNTER_SECOND_SHOT_DELAY =
        0.08f;


    // 本家公開値:
    // Bullet Speed = 1.4x Tank
    const float HUNTER_BULLET_SPEED_MULTIPLIER =
        1.40f;


    // 本家公開値:
    // Damage = 5.25 + 2.25 * Stat
    //
    // Basic系:
    // Damage = 7 + 3 * Stat
    //
    // よって75%。
    const float HUNTER_BULLET_DAMAGE_MULTIPLIER =
        0.75f;


    // 小さい砲身側には「slight spread」がある。
    // 正確な角度は公開値を確認できないため、
    // Bolv.ioでは±1.5度にする。
    const float HUNTER_SMALL_BULLET_SPREAD_DEG =
        1.5f;


    // 見た目側の砲身比率に合わせる。
    //
    // 小さい弾:
    // 細長い内側砲身から発射。
    const float HUNTER_SMALL_BULLET_SIZE =
        0.72f;


    const float HUNTER_SMALL_MUZZLE_DISTANCE =
        1.18f;


    // 大きい弾:
    // 太い外側砲身から発射。
    const float HUNTER_BIG_BULLET_SIZE =
        1.0f;


    const float HUNTER_BIG_MUZZLE_DISTANCE =
        1.02f;
}


// ============================================================
// FireHunter
//
// 1発目:
// 小さい砲身から小さい弾。
//
// 2発目:
// Update()側で少し遅れて発射する。
// ============================================================

void TankWeapon::FireHunter(
    const Vector3& playerPosition,
    const Vector3& aimDirection
)
{
    // ============================================
    // 小さい弾のわずかな拡散
    // ============================================

    const float random01 =
        static_cast<float>(
            std::rand()
        ) /
        static_cast<float>(
            RAND_MAX
        );


    const float spreadDeg =
        (
            random01 *
            2.0f -
            1.0f
        ) *
        HUNTER_SMALL_BULLET_SPREAD_DEG;


    // ============================================
    // 1発目
    // ============================================

    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        spreadDeg,
        HUNTER_SMALL_BULLET_SIZE,
        HUNTER_SMALL_MUZZLE_DISTANCE
    );


    // ============================================
    // 2発目を予約
    // ============================================

    m_hunterSecondShotPending =
        true;


    m_hunterSecondShotRemainingTime =
        HUNTER_SECOND_SHOT_DELAY;


    // Twin系から切り替えた場合のため。
    m_twinFireLeft =
        true;
}


// ============================================================
// FireHunterSecondShot
//
// 大きい砲身側。
// 左クリックを押し続けている場合だけUpdate()から呼ばれる。
// ============================================================

void TankWeapon::FireHunterSecondShot(
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


    if (player
        ->GetEvolution()
        .GetCurrentClass() !=
        PlayerEvolution::Hunter)
    {
        return;
    }


    Fire(
        playerPosition,
        aimDirection,
        0.0f,
        0.0f,
        HUNTER_BIG_BULLET_SIZE,
        HUNTER_BIG_MUZZLE_DISTANCE
    );
}


// ============================================================
// Second Shot Delay
// ============================================================

float TankWeapon::GetHunterSecondShotDelay() const
{
    return
        HUNTER_SECOND_SHOT_DELAY;
}


// ============================================================
// Bullet Speed
// ============================================================

float TankWeapon::GetHunterBulletSpeedMultiplier() const
{
    return
        HUNTER_BULLET_SPEED_MULTIPLIER;
}


// ============================================================
// Bullet Damage
// ============================================================

float TankWeapon::GetHunterBulletDamageMultiplier() const
{
    return
        HUNTER_BULLET_DAMAGE_MULTIPLIER;
}
