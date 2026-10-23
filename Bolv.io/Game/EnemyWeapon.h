
#pragma once

class Enemy;

class EnemyWeapon
{
public:

    void Update(
        Enemy& enemy,
        const Vector3& position,
        const Vector3& direction
    );

private:

    void Fire(
        Enemy& enemy,
        const Vector3& position,
        const Vector3& direction,
        float sideOffset = 0.0f,
        float spreadAngleDeg = 0.0f,
        float damageMultiplier = 1.0f,
        float bulletScaleMultiplier = 1.0f,
        float muzzleDistanceMultiplier = 1.0f
    );

private:

    float m_remainingTime =
        0.0f;

    // Twin左右交互。
    bool m_twinFireLeft =
        true;

    // Sprayerの発射サイクル。
    //
    // false: 大弾1発
    // true : 大弾＋小弾
    bool m_sprayerSecondShot =
        false;

    // Hunterの2発目の発射待ち。
    bool m_hunterSecondShotPending =
        false;

    // Hunterの2発目までの残り時間。
    float m_hunterSecondShotRemainingTime =
        0.0f;
};
