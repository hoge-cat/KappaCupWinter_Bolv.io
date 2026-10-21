#include "stdafx.h"
#include "EnemyContact.h"

#include "Enemy.h"
#include "Player.h"
#include "ShapeBox.h"

#include <functional>


namespace
{
    // ============================================
    // Body Damage
    //
    // Diep.ioは約0.04秒単位のtickで
    // 接触ダメージが発生する挙動に合わせる。
    // ============================================

    const float CONTACT_DAMAGE_INTERVAL =
        0.04f;


    // ============================================
    // Knockback
    //
    // 接触判定と同程度の周期。
    // ============================================

    const float CONTACT_KNOCKBACK_INTERVAL =
        0.04f;


    // ============================================
    // Tank同士の押し返し
    //
    // 正確な本家内部値ではなく、
    // Bolv.ioの移動速度に合わせた調整値。
    // ============================================

    const float PLAYER_KNOCKBACK_POWER =
        140.0f;


    const float ENEMY_KNOCKBACK_POWER =
        140.0f;


    // ============================================
    // Shape → Tank
    //
    // 現在はSquare等をまとめた暫定値。
    // ============================================

    const int SHAPE_BODY_DAMAGE =
        20;
}


// ============================================================
// Update
// ============================================================

void EnemyContact::Update(
    Enemy& enemy,
    const Vector3& enemyPosition,
    Player* player)
{
    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ============================================
    // Timer
    // ============================================

    m_damageTimer -=
        deltaTime;


    m_knockbackTimer -=
        deltaTime;


    if (enemy.IsDead())
    {
        return;
    }


    // ============================================
    // Enemy中心
    //
    // Lvによる大きさを反映。
    // ============================================

    Vector3 enemyCenter =
        enemyPosition;


    enemyCenter.y +=
        30.0f *
        enemy
        .GetLevelScale();


    // ============================================
    // Player
    // ============================================

    UpdatePlayerContact(
        enemy,
        enemyCenter,
        player
    );


    if (enemy.IsDead())
    {
        return;
    }


    // ============================================
    // Enemy
    // ============================================

    UpdateEnemyContact(
        enemy,
        enemyCenter
    );


    if (enemy.IsDead())
    {
        return;
    }


    // ============================================
    // Shape
    // ============================================

    UpdateShapeContact(
        enemy,
        enemyCenter
    );
}


// ============================================================
// Player Contact
// ============================================================

void EnemyContact::UpdatePlayerContact(
    Enemy& enemy,
    const Vector3& enemyCenter,
    Player* player)
{
    if (player ==
        nullptr)
    {
        return;
    }


    if (player
        ->GetHealth()
        .IsDead())
    {
        return;
    }


    // ============================================
    // Player中心
    // ============================================

    Vector3 playerCenter =
        player
        ->GetPosition();


    playerCenter.y +=
        player
        ->GetCollisionRadius();


    // ============================================
    // XZ距離
    // ============================================

    Vector3 difference =
        playerCenter -
        enemyCenter;


    difference.y =
        0.0f;


    const float distanceSquared =
        difference.x *
        difference.x +
        difference.z *
        difference.z;


    const float contactRadius =
        enemy
        .GetCollisionRadius() +
        player
        ->GetCollisionRadius();


    if (distanceSquared >
        contactRadius *
        contactRadius)
    {
        return;
    }


    // ============================================
    // Enemy → Player方向
    // ============================================

    Vector3 contactDirection =
        difference;


    const float directionLengthSquared =
        contactDirection.x *
        contactDirection.x +
        contactDirection.z *
        contactDirection.z;


    if (directionLengthSquared >
        0.0001f)
    {
        contactDirection.Normalize();
    }
    else
    {
        contactDirection =
            Vector3(
                1.0f,
                0.0f,
                0.0f
            );
    }


    // ============================================
    // Knockback
    // ============================================

    if (m_knockbackTimer <=
        0.0f)
    {
        player->AddKnockback(
            contactDirection,
            PLAYER_KNOCKBACK_POWER
        );


        enemy.AddKnockback(
            contactDirection *
            -1.0f,
            ENEMY_KNOCKBACK_POWER
        );


        m_knockbackTimer =
            CONTACT_KNOCKBACK_INTERVAL;
    }


    // ============================================
    // Body Damage
    // ============================================

    if (m_damageTimer <=
        0.0f)
    {
        // Enemy → Player
        player->TakeDamage(
            enemy
            .GetBodyDamageVsTank()
        );


        // Player → Enemy
        enemy.TakeDamage(
            player
            ->GetBodyDamageVsTank()
        );


        m_damageTimer =
            CONTACT_DAMAGE_INTERVAL;
    }
}


// ============================================================
// Enemy Contact
// ============================================================

void EnemyContact::UpdateEnemyContact(
    Enemy& enemy,
    const Vector3& enemyCenter)
{
    QueryGOs<Enemy>(
        "enemy",
        [&](Enemy* otherEnemy)
        {
            if (otherEnemy ==
                nullptr)
            {
                return true;
            }


            // ====================================
            // 自分自身
            // ====================================

            if (otherEnemy ==
                &enemy)
            {
                return true;
            }


            if (otherEnemy
                ->IsDead())
            {
                return true;
            }


            // ====================================
            // 同じ組み合わせを2回処理しない
            //
            // A→B
            // B→A
            //
            // の片方だけを通す。
            // ====================================

            std::less<Enemy*> pointerLess;


            if (!pointerLess(
                &enemy,
                otherEnemy
            ))
            {
                return true;
            }


            // ====================================
            // 相手Enemy中心
            // ====================================

            Vector3 otherCenter =
                otherEnemy
                ->GetPosition();


            otherCenter.y +=
                30.0f *
                otherEnemy
                ->GetLevelScale();


            // ====================================
            // XZ距離
            // ====================================

            Vector3 difference =
                otherCenter -
                enemyCenter;


            difference.y =
                0.0f;


            const float distanceSquared =
                difference.x *
                difference.x +
                difference.z *
                difference.z;


            const float contactRadius =
                enemy
                .GetCollisionRadius() +
                otherEnemy
                ->GetCollisionRadius();


            if (distanceSquared >
                contactRadius *
                contactRadius)
            {
                return true;
            }


            // ====================================
            // enemy → otherEnemy方向
            // ====================================

            Vector3 contactDirection =
                difference;


            const float directionLengthSquared =
                contactDirection.x *
                contactDirection.x +
                contactDirection.z *
                contactDirection.z;


            if (directionLengthSquared >
                0.0001f)
            {
                contactDirection.Normalize();
            }
            else
            {
                contactDirection =
                    Vector3(
                        1.0f,
                        0.0f,
                        0.0f
                    );
            }


            // ====================================
            // 相互Knockback
            // ====================================

            if (m_knockbackTimer <=
                0.0f)
            {
                // enemyからotherを押す
                otherEnemy
                    ->AddKnockback(
                        contactDirection,
                        ENEMY_KNOCKBACK_POWER
                    );


                // otherからenemyを押す
                enemy
                    .AddKnockback(
                        contactDirection *
                        -1.0f,
                        ENEMY_KNOCKBACK_POWER
                    );


                m_knockbackTimer =
                    CONTACT_KNOCKBACK_INTERVAL;
            }


            // ====================================
            // 相互Body Damage
            // ====================================

            if (m_damageTimer <=
                0.0f)
            {
                // enemy → otherEnemy
                otherEnemy
                    ->TakeDamage(
                        enemy
                        .GetBodyDamageVsTank(),
                        false
                    );


                // otherEnemy → enemy
                enemy
                    .TakeDamage(
                        otherEnemy
                        ->GetBodyDamageVsTank(),
                        false
                    );


                m_damageTimer =
                    CONTACT_DAMAGE_INTERVAL;
            }


            // ====================================
            // Enemyがここで死亡した場合、
            // これ以上別Enemyとの接触を見ない。
            // ====================================

            if (enemy.IsDead())
            {
                return false;
            }


            return true;
        }
    );
}


// ============================================================
// Shape Contact
// ============================================================

void EnemyContact::UpdateShapeContact(
    Enemy& enemy,
    const Vector3& enemyCenter)
{
    QueryGOs<ShapeBox>(
        "shapeBox",
        [&](ShapeBox* shape)
        {
            if (shape ==
                nullptr)
            {
                return true;
            }


            if (!shape
                ->IsAlive())
            {
                return true;
            }


            if (!shape
                ->IsTouch(
                    enemyCenter,
                    enemy
                    .GetCollisionRadius()
                ))
            {
                return true;
            }


            // ====================================
            // Body Damage
            // ====================================

            if (m_damageTimer <=
                0.0f)
            {
                // Enemy → Shape
                shape->TakeDamage(
                    enemy
                    .GetBodyDamageVsShape(),
                    false
                );


                // Shape → Enemy
                enemy.TakeDamage(
                    SHAPE_BODY_DAMAGE,
                    false
                );


                m_damageTimer =
                    CONTACT_DAMAGE_INTERVAL;
            }


            // 1つ当たれば終了。
            return false;
        }
    );
}