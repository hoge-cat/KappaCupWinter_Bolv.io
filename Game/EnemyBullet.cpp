#include "stdafx.h"
#include "EnemyBullet.h"

#include "Player.h"
#include "Enemy.h"
#include "ShapeBox.h"
#include "CollisionGrid.h"


namespace
{
    // ============================================
    // CollisionGrid検索時の余裕
    // ============================================

    const float COLLISION_QUERY_MARGIN =
        64.0f;


    // ============================================
    // Tank Body Damage
    // ↓
    // Bullet Penetration内部値
    // ============================================

    const int TANK_BODY_DAMAGE_TO_PENETRATION_SCALE =
        100;


    // ============================================
    // Shape Body Damage
    // ↓
    // Bullet Penetration内部値
    // ============================================

    const int SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE =
        25;
}


// ============================================================
// Init
// ============================================================

void EnemyBullet::Init(
    const Vector3& position,
    const Vector3& direction,
    int damage,
    float speed,
    int penetration,
    int collisionDamage,
    float levelScale,
    Enemy* owner
)
{
    // ============================================
    // Position
    // ============================================

    m_position =
        position;


    // ============================================
    // Owner
    // ============================================

    m_owner =
        owner;


    // ============================================
    // Direction
    // ============================================

    m_direction =
        direction;


    m_direction.y =
        0.0f;


    const float directionLengthSquared =
        m_direction.x *
        m_direction.x +
        m_direction.z *
        m_direction.z;


    if (directionLengthSquared >
        0.0001f)
    {
        m_direction.Normalize();
    }


    // ============================================
    // Damage
    // ============================================

    m_damage =
        damage;


    // ============================================
    // Speed
    // ============================================

    m_speed =
        speed;


    // ============================================
    // Penetration
    //
    // Bullet自身のHP
    // ============================================

    m_bulletHealth =
        penetration;


    // ============================================
    // Bullet Collision Damage
    // ============================================

    m_collisionDamage =
        collisionDamage;


    // ============================================
    // Size
    // ============================================

    if (levelScale <
        0.1f)
    {
        levelScale =
            0.1f;
    }


    m_levelScale =
        levelScale;


    m_radius =
        6.0f *
        m_levelScale;


    // ============================================
    // Life Time
    // ============================================

    m_remainingTime =
        3.0f;
}


// ============================================================
// Start
// ============================================================

bool EnemyBullet::Start()
{
    return true;
}


// ============================================================
// Update
// ============================================================

void EnemyBullet::Update()
{
    // ============================================
    // Bullet HP
    // ============================================

    if (IsDead())
    {
        DeleteGO(
            this
        );


        return;
    }


    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ============================================
    // Life Time
    // ============================================

    m_remainingTime -=
        deltaTime;


    if (m_remainingTime <=
        0.0f)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ============================================
    // 移動前後位置
    // ============================================

    const Vector3 previousPosition =
        m_position;


    const Vector3 nextPosition =
        m_position +
        m_direction *
        m_speed *
        deltaTime;


    // ========================================================
    // Player
    // ========================================================

    Player* player =
        FindGO<Player>(
            "player"
        );


    if (player !=
        nullptr &&
        !player
        ->GetHealth()
        .IsDead() &&
        IsHitPlayer(
            previousPosition,
            nextPosition,
            player
            ->GetPosition(),
            player
            ->GetCollisionRadius()
        ))
    {
        // ========================================
        // Player Body Damage
        // ========================================

        const int playerBodyDamage =
            player
            ->GetBodyDamageVsProjectile();


        const int penetrationDamage =
            playerBodyDamage *
            TANK_BODY_DAMAGE_TO_PENETRATION_SCALE;


        // ========================================
        // 実Damage
        // ========================================

        int actualDamage =
            m_damage;


        if (m_bulletHealth <
            penetrationDamage)
        {
            const float damageRate =
                static_cast<float>(
                    m_bulletHealth
                    ) /
                static_cast<float>(
                    penetrationDamage
                    );


            actualDamage =
                static_cast<int>(
                    static_cast<float>(
                        m_damage
                        ) *
                    damageRate
                    );


            if (actualDamage <
                1)
            {
                actualDamage =
                    1;
            }
        }


        // ========================================
        // PlayerへDamage
        // ========================================

        player
            ->TakeDamage(
                actualDamage
            );


        // ========================================
        // Bullet HP減少
        // ========================================

        m_bulletHealth -=
            penetrationDamage;


        if (m_bulletHealth <=
            0)
        {
            DeleteGO(
                this
            );


            return;
        }
    }


    // ========================================================
    // Enemy
    // ========================================================

    bool destroyedByEnemy =
        false;


    // ============================================
    // Enemy判定
    //
    // CollisionGridでもQueryGOsでも
    // 共通で使える。
    // ============================================

    auto enemyCheck =
        [&](Enemy* enemy)
        {
            if (enemy ==
                nullptr)
            {
                return true;
            }


            // ====================================
            // 発射した本人
            // ====================================

            if (enemy ==
                m_owner)
            {
                return true;
            }


            if (enemy
                ->IsDead())
            {
                return true;
            }


            // ====================================
            // Hit
            // ====================================

            if (!enemy
                ->IsHit(
                    previousPosition,
                    nextPosition,
                    m_radius
                ))
            {
                return true;
            }


            // ====================================
            // Enemy Body Damage
            // ====================================

            const int enemyBodyDamage =
                enemy
                ->GetBodyDamageVsProjectile();


            const int penetrationDamage =
                enemyBodyDamage *
                TANK_BODY_DAMAGE_TO_PENETRATION_SCALE;


            // ====================================
            // 実Damage
            // ====================================

            int actualDamage =
                m_damage;


            if (m_bulletHealth <
                penetrationDamage)
            {
                const float damageRate =
                    static_cast<float>(
                        m_bulletHealth
                        ) /
                    static_cast<float>(
                        penetrationDamage
                        );


                actualDamage =
                    static_cast<int>(
                        static_cast<float>(
                            m_damage
                            ) *
                        damageRate
                        );


                if (actualDamage <
                    1)
                {
                    actualDamage =
                        1;
                }
            }


            // ====================================
            // EnemyへDamage
            //
            // 敵同士なので報酬なし
            // ====================================

            enemy
                ->TakeDamage(
                    actualDamage,
                    false
                );


            // ====================================
            // Bullet HP減少
            // ====================================

            m_bulletHealth -=
                penetrationDamage;


            if (m_bulletHealth <=
                0)
            {
                destroyedByEnemy =
                    true;


                return false;
            }


            return true;
        };


    // ============================================
    // CollisionGrid
    //
    // 近くのEnemyだけ調べる。
    // ============================================

    if (g_collisionGrid !=
        nullptr)
    {
        g_collisionGrid
            ->QueryEnemies(
                previousPosition,
                nextPosition,
                COLLISION_QUERY_MARGIN,
                enemyCheck
            );
    }
    else
    {
        // ========================================
        // Gridがない場合の保険
        // ========================================

        QueryGOs<Enemy>(
            "enemy",
            enemyCheck
        );
    }


    if (destroyedByEnemy)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ========================================================
    // Shape
    // ========================================================

    bool destroyedByShape =
        false;


    auto shapeCheck =
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


            // ====================================
            // Hit
            // ====================================

            if (!shape
                ->IsHit(
                    previousPosition,
                    nextPosition,
                    m_radius
                ))
            {
                return true;
            }


            // ====================================
            // Shape Body Damage
            // ====================================

            const int shapeBodyDamage =
                shape
                ->GetBodyDamage();


            const int penetrationDamage =
                shapeBodyDamage *
                SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE;


            // ====================================
            // 実Damage
            // ====================================

            int actualDamage =
                m_damage;


            if (m_bulletHealth <
                penetrationDamage)
            {
                const float damageRate =
                    static_cast<float>(
                        m_bulletHealth
                        ) /
                    static_cast<float>(
                        penetrationDamage
                        );


                actualDamage =
                    static_cast<int>(
                        static_cast<float>(
                            m_damage
                            ) *
                        damageRate
                        );


                if (actualDamage <
                    1)
                {
                    actualDamage =
                        1;
                }
            }


            // ====================================
            // ShapeへDamage
            //
            // 敵弾なので報酬なし
            // ====================================

            shape
                ->TakeDamage(
                    actualDamage,
                    false
                );


            // ====================================
            // Bullet HP減少
            // ====================================

            m_bulletHealth -=
                penetrationDamage;


            if (m_bulletHealth <=
                0)
            {
                destroyedByShape =
                    true;


                return false;
            }


            return true;
        };


    // ============================================
    // CollisionGrid
    // ============================================

    if (g_collisionGrid !=
        nullptr)
    {
        g_collisionGrid
            ->QueryShapes(
                previousPosition,
                nextPosition,
                COLLISION_QUERY_MARGIN,
                shapeCheck
            );
    }
    else
    {
        QueryGOs<ShapeBox>(
            "shapeBox",
            shapeCheck
        );
    }


    if (destroyedByShape)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ========================================================
    // 移動確定
    // ========================================================

    m_position =
        nextPosition;
}


// ============================================================
// Render
// ============================================================

void EnemyBullet::Render(
    RenderContext& rc)
{
    // EnemyBulletRenderer側で
    // インスタンシング描画。

    (void)rc;
}


// ============================================================
// Playerとの衝突
// ============================================================

bool EnemyBullet::IsHitPlayer(
    const Vector3& start,
    const Vector3& end,
    const Vector3& playerPosition,
    float playerRadius
) const
{
    Vector3 center =
        playerPosition;


    center.y +=
        playerRadius;


    const Vector3 movement =
        end -
        start;


    const Vector3 toCenter =
        center -
        start;


    const float lengthSquared =
        movement.x *
        movement.x +
        movement.y *
        movement.y +
        movement.z *
        movement.z;


    float t =
        0.0f;


    if (lengthSquared >
        0.0001f)
    {
        t =
            (
                toCenter.x *
                movement.x +
                toCenter.y *
                movement.y +
                toCenter.z *
                movement.z
                ) /
            lengthSquared;


        if (t <
            0.0f)
        {
            t =
                0.0f;
        }


        if (t >
            1.0f)
        {
            t =
                1.0f;
        }
    }


    const Vector3 closest =
        start +
        movement *
        t;


    const Vector3 difference =
        center -
        closest;


    const float distanceSquared =
        difference.x *
        difference.x +
        difference.y *
        difference.y +
        difference.z *
        difference.z;


    const float hitRadius =
        playerRadius +
        m_radius;


    return
        distanceSquared <=
        hitRadius *
        hitRadius;
}


// ============================================================
// PlayerBulletとの衝突
//
// XZ平面上で判定。
// ============================================================

bool EnemyBullet::IsHitByBullet(
    const Vector3& start,
    const Vector3& end,
    float bulletRadius
) const
{
    if (IsDead())
    {
        return false;
    }


    const float movementX =
        end.x -
        start.x;


    const float movementZ =
        end.z -
        start.z;


    const float toCenterX =
        m_position.x -
        start.x;


    const float toCenterZ =
        m_position.z -
        start.z;


    const float lengthSquared =
        movementX *
        movementX +
        movementZ *
        movementZ;


    float t =
        0.0f;


    if (lengthSquared >
        0.0001f)
    {
        t =
            (
                toCenterX *
                movementX +
                toCenterZ *
                movementZ
                ) /
            lengthSquared;


        if (t <
            0.0f)
        {
            t =
                0.0f;
        }


        if (t >
            1.0f)
        {
            t =
                1.0f;
        }
    }


    const float closestX =
        start.x +
        movementX *
        t;


    const float closestZ =
        start.z +
        movementZ *
        t;


    const float differenceX =
        m_position.x -
        closestX;


    const float differenceZ =
        m_position.z -
        closestZ;


    const float distanceSquared =
        differenceX *
        differenceX +
        differenceZ *
        differenceZ;


    const float hitRadius =
        m_radius +
        bulletRadius;


    return
        distanceSquared <=
        hitRadius *
        hitRadius;
}