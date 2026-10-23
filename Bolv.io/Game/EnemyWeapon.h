
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

    // ============================================
    // 通常射撃のリロード管理
    // ============================================

    float m_remainingTime = 0.0f;

    // ============================================
    // Twin
    // ============================================

    bool m_twinFireLeft = true;

    // ============================================
    // Twin Flank
    //
    // 前後の左砲身 → 前後の右砲身
    // を交互に使用する。
    // ============================================

    bool m_twinFlankFireLeft = true;

    // ============================================
    // Sprayer
    // ============================================

    bool m_sprayerSecondShot = false;

    // ============================================
    // Hunter
    // ============================================

    bool m_hunterSecondShotPending = false;

    float m_hunterSecondShotRemainingTime = 0.0f;
};
