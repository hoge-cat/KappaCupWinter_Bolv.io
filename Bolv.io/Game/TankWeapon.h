
#pragma once


// ============================================================
// 自機の射撃を管理する。
// ============================================================

class TankWeapon
{
public:

    void Update(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );


private:

    // ========================================================
    // 共通Bullet生成
    //
    // sideOffset
    //   0 = 中央
    //  -値 = 左
    //  +値 = 右
    //
    // angleOffsetDeg
    //    0 = 正面
    //  -90 = 左
    //   90 = 右
    //  180 = 後方
    //
    // bulletSizeMultiplier
    //   Bulletモデルサイズ倍率
    //
    // muzzleDistanceMultiplier
    //   砲口位置倍率
    //
    // damageMultiplier
    //   弾ダメージ倍率
    // ========================================================

    void Fire(
        const Vector3& playerPosition,
        const Vector3& aimDirection,
        float sideOffset,
        float angleOffsetDeg,
        float bulletSizeMultiplier = 1.0f,
        float muzzleDistanceMultiplier = 1.0f,
        float damageMultiplier = 1.0f
    );


    // ========================================================
    // Basic
    // ========================================================

    void FireBasic(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );


    // ========================================================
    // Twin
    // ========================================================

    void FireTwin(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetTwinReloadMultiplier() const;


    // ========================================================
    // Triple Shot
    // ========================================================

    void FireTripleShot(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetTripleShotReloadMultiplier() const;


    // ========================================================
    // Quad Tank
    // ========================================================

    void FireQuadTank(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );


    // ========================================================
    // Twin Flank
    // ========================================================

    void FireTwinFlank(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetTwinFlankReloadMultiplier() const;

    float GetTwinFlankBulletDamageMultiplier() const;

    float GetTwinFlankBulletPenetrationMultiplier() const;


    // ========================================================
    // Sniper
    // ========================================================

    void FireSniper(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetSniperReloadMultiplier() const;

    float GetSniperBulletSpeedMultiplier() const;

    float GetSniperMuzzleDistanceMultiplier() const;


    // ========================================================
    // Assassin
    // ========================================================

    void FireAssassin(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetAssassinReloadMultiplier() const;

    float GetAssassinBulletSpeedMultiplier() const;

    float GetAssassinMuzzleDistanceMultiplier() const;


    // ========================================================
    // Hunter
    // ========================================================

    void FireHunter(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    void FireHunterSecondShot(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetHunterSecondShotDelay() const;

    float GetHunterBulletSpeedMultiplier() const;

    float GetHunterBulletDamageMultiplier() const;


    // ========================================================
    // Trapper
    // ========================================================

    void FireTrapper(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetTrapperReloadInterval(
        int reloadLevel
    ) const;


    // ========================================================
    // Machine Gun
    // ========================================================

    void FireMachineGun(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetMachineGunReloadMultiplier() const;


    // ========================================================
    // Flank Guard
    // ========================================================

    void FireFlankGuard(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );


    // ========================================================
    // Sprayer
    //
    // 外側：高速・広範囲拡散
    // 内側：低威力・小型弾・狭範囲拡散
    //
    // 2本の砲身は独立した周期で射撃する。
    // ========================================================

    void UpdateSprayer(
        const Vector3& playerPosition,
        const Vector3& aimDirection,
        float deltaTime
    );

    void FireSprayerOuter(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    void FireSprayerInner(
        const Vector3& playerPosition,
        const Vector3& aimDirection
    );

    float GetSprayerOuterReloadInterval(
        int reloadLevel
    ) const;

    float GetSprayerInnerReloadInterval(
        int reloadLevel
    ) const;


private:

    // ========================================================
    // Basic
    // ========================================================

    float m_fireInterval =
        0.5f;

    float m_remainingTime =
        0.0f;


    // ========================================================
    // Twin
    //
    // 左右交互発射。
    // ========================================================

    bool m_twinFireLeft =
        true;


    // ========================================================
    // Twin Flank
    //
    // 前後のTwinを同じタイミングで撃ち、
    // 左右を交互に切り替える。
    // ========================================================

    bool m_twinFlankFireLeft =
        true;


    // ========================================================
    // Hunter
    //
    // 小さい弾のあとに大きい弾を発射。
    // ========================================================

    bool m_hunterSecondShotPending =
        false;

    float m_hunterSecondShotRemainingTime =
        0.0f;


    // ========================================================
    // Sprayer
    //
    // 砲身ごとに独立したリロード時間を管理。
    // ========================================================

    float m_sprayerOuterRemainingTime =
        0.0f;

    float m_sprayerInnerRemainingTime =
        0.0f;
};
