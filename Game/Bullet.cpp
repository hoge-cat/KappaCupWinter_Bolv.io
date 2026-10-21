#include "stdafx.h"

#include "Bullet.h"

#include "ShapeBox.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "CollisionGrid.h"


namespace
{
    // ============================================
    // CollisionGrid検索時の余裕
    //
    // 弾の移動線分の周囲64まで検索する。
    //
    // Enemy / Shape / EnemyBulletの
    // 当たり判定サイズを十分含められる値。
    // ============================================

    const float COLLISION_QUERY_MARGIN =
        64.0f;


    // ============================================
    // ShapeのBody Damageを
    // Bullet Penetration内部値へ変換する倍率。
    //
    // 現在のSquare調整用。
    //
    // Square:
    //
    // Body Damage = 8
    //
    // 8 × 25
    // = 200
    //
    // 無強化Bullet Penetrationも200なので、
    // 無強化弾はSquareへ1回接触すると消滅する。
    //
    // ※これはBolv.io側でSquareの耐久感を
    // 本家風に合わせるための内部スケール。
    // ============================================

    const int SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE =
        25;


    // ============================================
    // TankのBody Damageを
    // Bullet Penetration内部値へ変換する倍率。
    //
    // Body Damage 0:
    // ProjectileへのDamage = 5
    //
    // Bolv.ioのBullet Penetration:
    //
    // 200, 350, 500...
    //
    // 100倍スケールなので
    // Tank側も100倍して比較する。
    // ============================================

    const int TANK_BODY_DAMAGE_TO_PENETRATION_SCALE =
        100;
}


// ============================================================
// Init
// ============================================================

void Bullet::Init(
    const Vector3& position,
    const Vector3& direction,
    int damage,
    float speed,
    int penetration,
    int collisionDamage,
    float levelScale)
{
    // ============================================
    // Position
    // ============================================

    m_position =
        position;


    // ============================================
    // Direction
    // ============================================

    m_direction =
        direction;


    const float directionLengthSquared =
        m_direction.x *
        m_direction.x +
        m_direction.y *
        m_direction.y +
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
    // BulletそのもののHP。
    // ============================================

    m_penetration =
        penetration;


    // ============================================
    // Bullet Collision Damage
    // ============================================

    m_collisionDamage =
        collisionDamage;


    // ============================================
    // Level Scale
    // ============================================

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

bool Bullet::Start()
{
    return true;
}


// ============================================================
// Update
// ============================================================

void Bullet::Update()
{
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
    // Penetrationが0なら消滅
    // ============================================

    if (m_penetration <=
        0)
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
    // Enemy Bullet
    // ========================================================

    bool destroyedByEnemyBullet =
        false;


    // ============================================
    // EnemyBullet判定処理
    //
    // CollisionGridでもQueryGOsでも
    // 同じ処理を使えるようLambda化。
    // ============================================

    auto enemyBulletCheck =
        [&](EnemyBullet* enemyBullet)
        {
            // ====================================
            // 無効
            // ====================================

            if (enemyBullet ==
                nullptr)
            {
                return true;
            }


            if (enemyBullet
                ->IsDead())
            {
                return true;
            }


            // ====================================
            // Hit
            // ====================================

            if (!enemyBullet
                ->IsHitByBullet(
                    previousPosition,
                    nextPosition,
                    m_radius
                ))
            {
                return true;
            }


            // ====================================
            // Enemy BulletへDamage
            // ====================================

            enemyBullet
                ->TakeCollisionDamage(
                    m_collisionDamage
                );


            // ====================================
            // Enemy BulletからこちらもDamage
            // ====================================

            m_penetration -=
                enemyBullet
                ->GetCollisionDamage();


            // ====================================
            // Bullet HP消滅
            // ====================================

            if (m_penetration <=
                0)
            {
                destroyedByEnemyBullet =
                    true;


                return false;
            }


            return true;
        };


    // ============================================
    // CollisionGridがある場合
    //
    // 近くのEnemyBulletだけ検索。
    // ============================================

    if (g_collisionGrid !=
        nullptr)
    {
        g_collisionGrid
            ->QueryEnemyBullets(
                previousPosition,
                nextPosition,
                COLLISION_QUERY_MARGIN,
                enemyBulletCheck
            );
    }
    else
    {
        // ========================================
        // CollisionGridがない場合の保険
        // ========================================

        QueryGOs<EnemyBullet>(
            "enemyBullet",
            enemyBulletCheck
        );
    }


    if (destroyedByEnemyBullet)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ========================================================
    // Enemy Tank
    //
    // Diep.io風:
    //
    // Bullet Penetration
    // = BulletそのもののHP
    //
    // TankのBody Damageによって
    // Bullet HPも減少する。
    //
    // Bullet HPが相手Tankの
    // Body Damageより少ない場合は、
    // 残っているHP割合に応じて
    // 実際のBullet Damageも低下する。
    // ========================================================

    bool destroyedByEnemy =
        false;


    auto enemyCheck =
        [&](Enemy* enemy)
        {
            // ====================================
            // 無効Enemy
            // ====================================

            if (enemy ==
                nullptr)
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
            // Enemy Tankの
            // Projectileに対するBody Damage
            // ====================================

            const int enemyBodyDamage =
                enemy
                ->GetBodyDamageVsProjectile();


            // ====================================
            // Bullet HPスケールへ変換
            //
            // Body Damage 0:
            //
            // 5 × 100
            // = 500
            // ====================================

            const int penetrationDamage =
                enemyBodyDamage *
                TANK_BODY_DAMAGE_TO_PENETRATION_SCALE;


            // ====================================
            // 実際に与えるDamage
            // ====================================

            int actualDamage =
                m_damage;


            // ====================================
            // Bullet HPが
            // Tank Body Damageより少ない場合
            //
            // 残っているBullet HPの割合だけ
            // Damageも低下。
            // ====================================

            if (m_penetration <
                penetrationDamage)
            {
                const float damageRate =
                    static_cast<float>(
                        m_penetration
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


                // =================================
                // 最低1Damage
                // =================================

                if (actualDamage <
                    1)
                {
                    actualDamage =
                        1;
                }
            }


            // ====================================
            // EnemyへDamage
            // ====================================

            enemy
                ->TakeDamage(
                    actualDamage
                );


            // ====================================
            // Tank Body Damageによって
            // Bullet HP減少
            // ====================================

            m_penetration -=
                penetrationDamage;


            // ====================================
            // Bullet HPがなくなった
            // ====================================

            if (m_penetration <=
                0)
            {
                destroyedByEnemy =
                    true;


                return false;
            }


            // ====================================
            // Bullet HPが残っている
            //
            // 高Penetration Bulletなら
            // Tankとの接触後も残る。
            // ====================================

            return true;
        };


    // ============================================
    // CollisionGridで近くのEnemyのみ検索
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
        [&](ShapeBox* shapeBox)
        {
            // ====================================
            // 無効Shape
            // ====================================

            if (shapeBox ==
                nullptr)
            {
                return true;
            }


            if (!shapeBox
                ->IsAlive())
            {
                return true;
            }


            // ====================================
            // Hit
            // ====================================

            if (!shapeBox
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
                shapeBox
                ->GetBodyDamage();


            // ====================================
            // Shape Body Damageを
            // Bullet Penetration内部値へ変換
            // ====================================

            const int penetrationDamage =
                shapeBodyDamage *
                SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE;


            // ====================================
            // 本家風
            //
            // Bullet HPがShape Damageより
            // 少ない場合、
            //
            // 残っているBullet HPの割合だけ
            // Damageを減らす。
            //
            // 現在のSquare:
            //
            // HP = 10
            // Body Damage = 8
            //
            // 無強化Bullet:
            //
            // Damage = 7
            // Penetration = 200
            //
            // 8 × 25 = 200
            //
            // 1回の接触でBulletは消滅。
            // ====================================

            int actualDamage =
                m_damage;


            if (m_penetration <
                penetrationDamage)
            {
                const float damageRate =
                    static_cast<float>(
                        m_penetration
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


                // =================================
                // 最低1Damage
                // =================================

                if (actualDamage <
                    1)
                {
                    actualDamage =
                        1;
                }
            }


            // ====================================
            // ShapeへDamage
            // ====================================

            shapeBox
                ->TakeDamage(
                    actualDamage
                );


            // ====================================
            // Bullet HP減少
            // ====================================

            m_penetration -=
                penetrationDamage;


            // ====================================
            // Penetrationがなくなった
            // ====================================

            if (m_penetration <=
                0)
            {
                destroyedByShape =
                    true;


                return false;
            }


            // ====================================
            // Bullet HPが残っていれば
            // そのまま存在する。
            // ====================================

            return true;
        };


    // ============================================
    // CollisionGridで近くのShapeだけ検索
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

void Bullet::Render(
    RenderContext& rc)
{
    // BulletRenderer側で
    // インスタンシング描画するため、
    // 個別描画はしない。

    (void)rc;
}