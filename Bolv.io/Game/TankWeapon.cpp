#include "stdafx.h"
#include "TankWeapon.h"
#include "Bullet.h"
#include "system/system.h"
#include "Player.h"

#include <cmath>
#include <cstdlib>


namespace
{
    const float PI =
        3.14159265f;


    // ========================================================
    // Hunter Reload
    //
    // Reload Lv:
    // 0  1  2  3  4  5  6  7
    //
    // Tick:
    // 38 35 32 29 27 24 22 20
    //
    // 1tick = 0.04秒
    // ========================================================

    const int HUNTER_RELOAD_TICKS[8] =
    {
        38,
        35,
        32,
        29,
        27,
        24,
        22,
        20
    };


    const float DIEP_TICK_SECONDS =
        0.04f;
}


// ============================================================
// Update
// ============================================================

void TankWeapon::Update(
    const Vector3& playerPosition,
    const Vector3& aimDirection)
{
    // ============================================
    // 別アプリ操作中は撃たない
    // ============================================

    if (GetForegroundWindow() !=
        g_hWnd)
    {
        m_hunterSecondShotPending =
            false;


        m_hunterSecondShotRemainingTime =
            0.0f;


        return;
    }


    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ============================================
    // リロード
    // ============================================

    m_remainingTime -=
        deltaTime;


    // ============================================
    // マウス位置取得
    // ============================================

    POINT mousePosition;


    GetCursorPos(
        &mousePosition
    );


    ScreenToClient(
        g_hWnd,
        &mousePosition
    );


    RECT clientRect;


    GetClientRect(
        g_hWnd,
        &clientRect
    );


    const float width =
        static_cast<float>(
            clientRect.right -
            clientRect.left
            );


    const float height =
        static_cast<float>(
            clientRect.bottom -
            clientRect.top
            );


    const float mouseX =
        static_cast<float>(
            mousePosition.x
            ) -
        width *
        0.5f;


    const float mouseY =
        height *
        0.5f -
        static_cast<float>(
            mousePosition.y
            );


    // ============================================
    // 進化UI上では射撃しない
    // ============================================

    const bool mouseOnEvolutionUI =
        mouseX >=
        300.0f &&
        mouseX <=
        630.0f &&
        mouseY <=
        -190.0f &&
        mouseY >=
        -520.0f;


    if (mouseOnEvolutionUI)
    {
        m_hunterSecondShotPending =
            false;


        m_hunterSecondShotRemainingTime =
            0.0f;


        return;
    }


    // ============================================
    // 左クリック
    // ============================================

    const bool leftMouse =
        (
            GetAsyncKeyState(
                VK_LBUTTON
            ) &
            0x8000
            ) !=
        0;


    // ============================================
    // Hunter 2発目
    //
    // 小さい弾のあと、
    // 少し遅れて大きい弾を撃つ。
    //
    // 左クリックを離したら
    // 2発目をキャンセル。
    // ============================================

    if (m_hunterSecondShotPending)
    {
        if (!leftMouse)
        {
            m_hunterSecondShotPending =
                false;


            m_hunterSecondShotRemainingTime =
                0.0f;
        }
        else
        {
            m_hunterSecondShotRemainingTime -=
                deltaTime;


            if (m_hunterSecondShotRemainingTime <=
                0.0f)
            {
                FireHunterSecondShot(
                    playerPosition,
                    aimDirection
                );


                m_hunterSecondShotPending =
                    false;


                m_hunterSecondShotRemainingTime =
                    0.0f;
            }
        }
    }


    if (!leftMouse)
    {
        return;
    }


    // ============================================
    // リロード中
    // ============================================

    if (m_remainingTime >
        0.0f)
    {
        return;
    }


    // ============================================
    // 照準方向確認
    // ============================================

    const float lengthSquared =
        aimDirection.x *
        aimDirection.x +
        aimDirection.y *
        aimDirection.y +
        aimDirection.z *
        aimDirection.z;


    if (lengthSquared <=
        0.0001f)
    {
        return;
    }


    // ============================================
    // Player取得
    // ============================================

    Player* player =
        FindGO<Player>(
            "player"
        );


    PlayerEvolution::TankClass tankClass =
        PlayerEvolution::Basic;


    if (player !=
        nullptr)
    {
        tankClass =
            player
            ->GetEvolution()
            .GetCurrentClass();
    }


    // ========================================================
    // Smasher
    // ========================================================

    if (tankClass ==
        PlayerEvolution::Smasher)
    {
        m_twinFireLeft =
            true;


        return;
    }


    // ========================================================
    // Basic
    // ========================================================

    if (tankClass ==
        PlayerEvolution::Basic)
    {
        FireBasic(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Twin
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::Twin)
    {
        FireTwin(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Triple Shot
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::TripleShot)
    {
        FireTripleShot(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Quad Tank
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::QuadTank)
    {
        FireQuadTank(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Twin Flank
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::TwinFlank)
    {
        FireTwinFlank(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Sniper
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::Sniper)
    {
        FireSniper(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Assassin
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::Assassin)
    {
        FireAssassin(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Hunter
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::Hunter)
    {
        FireHunter(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Trapper
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::Trapper)
    {
        FireTrapper(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Machine Gun
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::MachineGun)
    {
        FireMachineGun(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // Flank Guard
    // ========================================================

    else if (
        tankClass ==
        PlayerEvolution::FlankGuard)
    {
        FireFlankGuard(
            playerPosition,
            aimDirection
        );
    }


    // ========================================================
    // その他
    // ========================================================

    else
    {
        FireBasic(
            playerPosition,
            aimDirection
        );
    }


    // ============================================
    // 次の発射までの時間
    // ============================================

    if (player !=
        nullptr)
    {
        float reloadInterval =
            player
            ->GetReloadInterval();


        // ========================================
        // Twin
        // ========================================

        if (tankClass ==
            PlayerEvolution::Twin)
        {
            reloadInterval *=
                GetTwinReloadMultiplier();
        }


        // ========================================
        // Triple Shot
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::TripleShot)
        {
            reloadInterval *=
                GetTripleShotReloadMultiplier();
        }


        // ========================================
        // Twin Flank
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::TwinFlank)
        {
            reloadInterval *=
                GetTwinFlankReloadMultiplier();
        }


        // ========================================
        // Machine Gun
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::MachineGun)
        {
            reloadInterval *=
                GetMachineGunReloadMultiplier();
        }


        // ========================================
        // Sniper
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Sniper)
        {
            reloadInterval *=
                GetSniperReloadMultiplier();
        }


        // ========================================
        // Assassin
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Assassin)
        {
            reloadInterval *=
                GetAssassinReloadMultiplier();
        }


        // ========================================
        // Hunter
        //
        // Reload Lv0～7
        //
        // 38,35,32,29,27,24,22,20 tick
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Hunter)
        {
            int reloadLevel =
                player
                ->GetStats()
                .GetLevel(
                    PlayerStats::Reload
                );


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


            reloadInterval =
                static_cast<float>(
                    HUNTER_RELOAD_TICKS[
                        reloadLevel
                    ]
                    ) *
                DIEP_TICK_SECONDS;
        }


        // ========================================
        // Trapper
        //
        // Reload Lv0～7
        //
        // 23,21,19,18,16,15,14,12 tick
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Trapper)
        {
            int reloadLevel =
                player
                ->GetStats()
                .GetLevel(
                    PlayerStats::Reload
                );


            reloadInterval =
                GetTrapperReloadInterval(
                    reloadLevel
                );
        }


        m_remainingTime =
            reloadInterval;
    }
    else
    {
        m_remainingTime =
            m_fireInterval;
    }
}


// ============================================================
// Fire
// ============================================================

void TankWeapon::Fire(
    const Vector3& playerPosition,
    const Vector3& aimDirection,
    float sideOffset,
    float angleOffsetDeg,
    float bulletSizeMultiplier,
    float muzzleDistanceMultiplier)
{
    // ============================================
    // 基準射撃方向
    // ============================================

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


    // ============================================
    // 角度オフセット
    // ============================================

    if (angleOffsetDeg !=
        0.0f)
    {
        const float angleRad =
            angleOffsetDeg *
            PI /
            180.0f;


        const float cosValue =
            std::cos(
                angleRad
            );


        const float sinValue =
            std::sin(
                angleRad
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


    // ============================================
    // Player取得
    // ============================================

    Player* player =
        FindGO<Player>(
            "player"
        );


    // ============================================
    // 弾性能
    // ============================================

    int damage =
        1;


    float speed =
        500.0f;


    int penetration =
        100;


    int collisionDamage =
        100;


    float levelScale =
        1.0f;


    PlayerEvolution::TankClass tankClass =
        PlayerEvolution::Basic;


    if (player !=
        nullptr)
    {
        tankClass =
            player
            ->GetEvolution()
            .GetCurrentClass();


        damage =
            player
            ->GetBulletDamage();


        speed =
            player
            ->GetBulletSpeed();


        penetration =
            player
            ->GetBulletPenetration();


        collisionDamage =
            player
            ->GetBulletCollisionDamage();


        // ========================================
        // Quad Tank
        // ========================================

        if (tankClass ==
            PlayerEvolution::QuadTank)
        {
            damage =
                static_cast<int>(
                    static_cast<float>(
                        damage
                        ) *
                    0.75f +
                    0.5f
                    );


            if (damage <
                1)
            {
                damage =
                    1;
            }
        }


        // ========================================
        // Twin Flank
        //
        // Damage:
        // Basicの60%
        //
        // Penetration:
        // Basicの90%
        // ========================================

        if (tankClass ==
            PlayerEvolution::TwinFlank)
        {
            // ====================================
            // Bullet Damage
            // ====================================

            damage =
                static_cast<int>(
                    static_cast<float>(
                        damage
                        ) *
                    GetTwinFlankBulletDamageMultiplier() +
                    0.5f
                    );


            if (damage <
                1)
            {
                damage =
                    1;
            }


            // ====================================
            // Bullet Penetration
            // ====================================

            penetration =
                static_cast<int>(
                    static_cast<float>(
                        penetration
                        ) *
                    GetTwinFlankBulletPenetrationMultiplier() +
                    0.5f
                    );


            if (penetration <
                1)
            {
                penetration =
                    1;
            }


            // ====================================
            // Bullet vs Bullet Damage
            // ====================================

            collisionDamage =
                static_cast<int>(
                    static_cast<float>(
                        collisionDamage
                        ) *
                    GetTwinFlankBulletDamageMultiplier() +
                    0.5f
                    );


            if (collisionDamage <
                1)
            {
                collisionDamage =
                    1;
            }
        }


        // ========================================
        // Hunter
        //
        // Damage 75%
        // ========================================

        if (tankClass ==
            PlayerEvolution::Hunter)
        {
            damage =
                static_cast<int>(
                    static_cast<float>(
                        damage
                        ) *
                    GetHunterBulletDamageMultiplier() +
                    0.5f
                    );


            collisionDamage =
                static_cast<int>(
                    static_cast<float>(
                        collisionDamage
                        ) *
                    GetHunterBulletDamageMultiplier() +
                    0.5f
                    );


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
        }


        // ========================================
        // Level Scale
        // ========================================

        const int currentLevel =
            player
            ->GetProgress()
            .GetLevel();


        levelScale =
            std::pow(
                1.01f,
                static_cast<float>(
                    currentLevel -
                    1
                    )
            );


        // ========================================
        // Sniper
        // ========================================

        if (tankClass ==
            PlayerEvolution::Sniper)
        {
            speed *=
                GetSniperBulletSpeedMultiplier();
        }


        // ========================================
        // Assassin
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Assassin)
        {
            speed *=
                GetAssassinBulletSpeedMultiplier();
        }


        // ========================================
        // Hunter
        // ========================================

        else if (
            tankClass ==
            PlayerEvolution::Hunter)
        {
            speed *=
                GetHunterBulletSpeedMultiplier();
        }
    }


    // ============================================
    // 発射位置
    // ============================================

    Vector3 firePosition =
        playerPosition;


    firePosition.y +=
        30.0f *
        levelScale;


    // ============================================
    // 右方向
    // ============================================

    Vector3 right(
        direction.z,
        0.0f,
        -direction.x
    );


    const float rightLengthSquared =
        right.x *
        right.x +
        right.z *
        right.z;


    if (rightLengthSquared >
        0.0001f)
    {
        right.Normalize();
    }


    // ============================================
    // 左右砲身位置
    // ============================================

    firePosition +=
        right *
        (
            sideOffset *
            levelScale
            );


    // ============================================
    // 砲口まで前へ移動
    // ============================================

    float muzzleDistance =
        56.0f;


    // ============================================
    // Sniper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sniper)
    {
        muzzleDistance *=
            GetSniperMuzzleDistanceMultiplier();
    }


    // ============================================
    // Assassin
    // ============================================

    else if (
        tankClass ==
        PlayerEvolution::Assassin)
    {
        muzzleDistance *=
            GetAssassinMuzzleDistanceMultiplier();
    }


    // ============================================
    // Hunter
    //
    // 1発目・2発目で砲口位置を変える。
    // ============================================

    else if (
        tankClass ==
        PlayerEvolution::Hunter)
    {
        muzzleDistance *=
            muzzleDistanceMultiplier;
    }


    // ============================================
    // 前方へ移動
    // ============================================

    firePosition +=
        direction *
        (
            muzzleDistance *
            levelScale
            );


    // ============================================
    // 弾生成
    // ============================================

    Bullet* bullet =
        NewGO<Bullet>(
            0,
            "bullet"
        );


    if (bullet ==
        nullptr)
    {
        return;
    }


    // ============================================
    // Bulletサイズ
    //
    // Hunterの小さい1発目などで使用。
    // ============================================

    const float bulletScale =
        levelScale *
        bulletSizeMultiplier;


    // ============================================
    // Init
    // ============================================

    bullet->Init(
        firePosition,
        direction,
        damage,
        speed,
        penetration,
        collisionDamage,
        bulletScale
    );
}