#include "stdafx.h"

#include "Trap.h"

#include "ShapeBox.h"
#include "Enemy.h"
#include "EnemyBullet.h"

#include <cmath>


namespace
{
    // ========================================================
    // Diep.io Trap
    // ========================================================

    // 通常TrapperのTrap寿命。
    const float TRAP_LIFE_TIME =
        24.0f;


    // ========================================================
    // Trap減速
    //
    // 発射直後は前へ飛び、
    // その後かなり急激に減速して停止する。
    // ========================================================

    const float TRAP_SPEED_REMAIN_PER_SECOND =
        0.01f;


    const float TRAP_STOP_SPEED =
        6.0f;


    // ========================================================
    // Enemy Bulletから押される強さ
    // ========================================================

    const float TRAP_BULLET_PUSH_POWER =
        55.0f;


    const float TRAP_PUSH_DAMPING_PER_SECOND =
        0.08f;


    // ========================================================
    // Tankへ毎フレーム連続Hitしないための間隔
    // ========================================================

    const float TRAP_TANK_HIT_INTERVAL =
        0.12f;


    // ========================================================
    // Bolv.io内部Penetrationスケール
    // ========================================================

    const int
        SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE =
        25;


    const int
        TANK_BODY_DAMAGE_TO_PENETRATION_SCALE =
        100;


    // ========================================================
    // Trapサイズ
    //
    // 3ds Max側:
    //
    // Bullet
    // Radius = 6
    //
    // TankBody
    // Radius = 30
    //
    // Trap
    // Outer Radius = 12
    //
    // そのためゲーム側の当たり判定も12へ合わせる。
    // ========================================================

    const float TRAP_RADIUS =
        12.0f;


    // ========================================================
    // Trapモデル描画倍率
    //
    // Trap.max自体を実寸
    // Outer Radius = 12
    //
    // で作ったため、追加縮小しない。
    // ========================================================

    const float TRAP_MODEL_SCALE =
        1.0f;


    const float PI =
        3.14159265f;
}


// ============================================================
// Init
// ============================================================

void Trap::Init(
    const Vector3& position,
    const Vector3& direction,
    int damage,
    float speed,
    int penetration,
    int collisionDamage,
    float levelScale
)
{
    // ========================================================
    // Position
    // ========================================================

    m_position =
        position;


    // ========================================================
    // Direction
    // ========================================================

    m_direction =
        direction;


    // 地面と平行にする。
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
    else
    {
        m_direction =
            Vector3(
                0.0f,
                0.0f,
                1.0f
            );
    }


    // ========================================================
    // Performance
    // ========================================================

    m_damage =
        damage;


    m_speed =
        speed;


    m_penetration =
        penetration;


    m_collisionDamage =
        collisionDamage;


    // ========================================================
    // Level Scale
    // ========================================================

    if (levelScale <
        0.1f)
    {
        levelScale =
            0.1f;
    }


    m_levelScale =
        levelScale;


    // ========================================================
    // Collision Radius
    //
    // Trap.maxの外半径12と一致させる。
    // ========================================================

    m_radius =
        TRAP_RADIUS *
        m_levelScale;


    // ========================================================
    // Life
    // ========================================================

    m_remainingTime =
        TRAP_LIFE_TIME;


    m_pushVelocity =
        Vector3::Zero;


    m_tankHitCooldown =
        0.0f;


    // ========================================================
    // Trapの向き
    // ========================================================

    const float yaw =
        std::atan2(
            m_direction.x,
            m_direction.z
        );


    m_rotation.SetRotationDeg(
        Vector3::AxisY,
        yaw *
        180.0f /
        PI +
        180.0f
    );
}


// ============================================================
// Start
// ============================================================

bool Trap::Start()
{
    // 描画はTrapRendererが担当する。
    return true;
}


// ============================================================
// Update
// ============================================================

void Trap::Update()
{
    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ========================================================
    // Life Time
    // ========================================================

    m_remainingTime -=
        deltaTime;


    if (m_remainingTime <=
        0.0f ||
        m_penetration <=
        0)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ========================================================
    // Tank Hit Cooldown
    // ========================================================

    if (m_tankHitCooldown >
        0.0f)
    {
        m_tankHitCooldown -=
            deltaTime;
    }


    // ========================================================
    // 移動前位置
    // ========================================================

    const Vector3 previousPosition =
        m_position;


    // ========================================================
    // 発射直後の前進
    // ========================================================

    if (m_speed >
        0.0f)
    {
        m_position +=
            m_direction *
            m_speed *
            deltaTime;


        // ====================================================
        // 急減速
        // ====================================================

        m_speed *=
            std::pow(
                TRAP_SPEED_REMAIN_PER_SECOND,
                deltaTime
            );


        // 十分遅くなったら完全停止。
        if (m_speed <
            TRAP_STOP_SPEED)
        {
            m_speed =
                0.0f;
        }
    }


    // ========================================================
    // Enemy Bulletから受けた押し
    // ========================================================

    m_position +=
        m_pushVelocity *
        deltaTime;


    m_pushVelocity *=
        std::pow(
            TRAP_PUSH_DAMPING_PER_SECOND,
            deltaTime
        );


    // ========================================================
    // 移動後位置
    // ========================================================

    const Vector3 nextPosition =
        m_position;


    // ========================================================
    // Enemy Bullet
    // ========================================================

    bool destroyedByEnemyBullet =
        false;


    QueryGOs<EnemyBullet>(
        "enemyBullet",

        [&](EnemyBullet* enemyBullet)
        {
            // ================================================
            // 無効
            // ================================================

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


            // ================================================
            // Hit判定
            // ================================================

            if (!enemyBullet
                ->IsHitByBullet(
                    previousPosition,
                    nextPosition,
                    m_radius
                ))
            {
                return true;
            }


            // ================================================
            // Enemy BulletへDamage
            // ================================================

            enemyBullet
                ->TakeCollisionDamage(
                    m_collisionDamage
                );


            // ================================================
            // Trap HP減少
            // ================================================

            m_penetration -=
                enemyBullet
                ->GetCollisionDamage();


            // ================================================
            // Enemy Bulletによる押し
            // ================================================

            Vector3 pushDirection =
                m_position -
                enemyBullet
                ->GetPosition();


            pushDirection.y =
                0.0f;


            const float pushLengthSquared =
                pushDirection.x *
                pushDirection.x +
                pushDirection.z *
                pushDirection.z;


            if (pushLengthSquared >
                0.0001f)
            {
                pushDirection.Normalize();


                m_pushVelocity +=
                    pushDirection *
                    TRAP_BULLET_PUSH_POWER;
            }


            // ================================================
            // Trap破壊
            // ================================================

            if (m_penetration <=
                0)
            {
                destroyedByEnemyBullet =
                    true;


                return false;
            }


            return true;
        }
    );


    if (destroyedByEnemyBullet)
    {
        DeleteGO(
            this
        );


        return;
    }


    // ========================================================
    // Enemy Tank
    // ========================================================

    if (m_tankHitCooldown <=
        0.0f)
    {
        bool hitTank =
            false;


        QueryGOs<Enemy>(
            "enemy",

            [&](Enemy* enemy)
            {
                // ============================================
                // 無効Enemy
                // ============================================

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


                // ============================================
                // Hit
                // ============================================

                if (!enemy
                    ->IsHit(
                        previousPosition,
                        nextPosition,
                        m_radius
                    ))
                {
                    return true;
                }


                // ============================================
                // Enemy TankのBody Damage
                // ============================================

                const int enemyBodyDamage =
                    enemy
                    ->GetBodyDamageVsProjectile();


                // ============================================
                // Bolv.io Penetrationスケールへ変換
                // ============================================

                const int penetrationDamage =
                    enemyBodyDamage *
                    TANK_BODY_DAMAGE_TO_PENETRATION_SCALE;


                // ============================================
                // 実際のDamage
                // ============================================

                int actualDamage =
                    m_damage;


                // ============================================
                // Trap HPが不足している場合
                // ============================================

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


                    if (actualDamage <
                        1)
                    {
                        actualDamage =
                            1;
                    }
                }


                // ============================================
                // EnemyへDamage
                // ============================================

                enemy
                    ->TakeDamage(
                        actualDamage
                    );


                // ============================================
                // Trap HP減少
                // ============================================

                m_penetration -=
                    penetrationDamage;


                hitTank =
                    true;


                // 1フレームで複数Tankへ当てない。
                return false;
            }
        );


        // ====================================================
        // Tankへ当たった
        // ====================================================

        if (hitTank)
        {
            m_tankHitCooldown =
                TRAP_TANK_HIT_INTERVAL;


            if (m_penetration <=
                0)
            {
                DeleteGO(
                    this
                );


                return;
            }
        }
    }


    // ========================================================
    // Shape
    // ========================================================

    bool hitShape =
        false;


    QueryGOs<ShapeBox>(
        "shapeBox",

        [&](ShapeBox* shape)
        {
            // ================================================
            // 無効
            // ================================================

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


            // ================================================
            // Hit
            // ================================================

            if (!shape
                ->IsHit(
                    previousPosition,
                    nextPosition,
                    m_radius
                ))
            {
                return true;
            }


            // ================================================
            // Shape Body Damage
            // ================================================

            const int shapeBodyDamage =
                shape
                ->GetBodyDamage();


            // ================================================
            // Penetration消費
            // ================================================

            const int penetrationDamage =
                shapeBodyDamage *
                SHAPE_BODY_DAMAGE_TO_PENETRATION_SCALE;


            int actualDamage =
                m_damage;


            // ================================================
            // Trap HPが少ない場合
            // ================================================

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


                if (actualDamage <
                    1)
                {
                    actualDamage =
                        1;
                }
            }


            // ================================================
            // ShapeへDamage
            // ================================================

            shape
                ->TakeDamage(
                    actualDamage
                );


            // ================================================
            // Trap HP減少
            // ================================================

            m_penetration -=
                penetrationDamage;


            hitShape =
                true;


            return false;
        }
    );


    // ========================================================
    // Shape接触で破壊された
    // ========================================================

    if (hitShape &&
        m_penetration <=
        0)
    {
        DeleteGO(
            this
        );


        return;
    }
}


// ============================================================
// Render
// ============================================================

void Trap::Render(
    RenderContext& rc
)
{
    // 描画はTrapRendererがまとめて行う。
    (void)rc;
}


// ============================================================
// IsPointNearSegmentXZ
// ============================================================

bool Trap::IsPointNearSegmentXZ(
    const Vector3& start,
    const Vector3& end,
    const Vector3& point,
    float radius
) const
{
    const float movementX =
        end.x -
        start.x;


    const float movementZ =
        end.z -
        start.z;


    const float toPointX =
        point.x -
        start.x;


    const float toPointZ =
        point.z -
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
                toPointX *
                movementX +
                toPointZ *
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
        point.x -
        closestX;


    const float differenceZ =
        point.z -
        closestZ;


    const float distanceSquared =
        differenceX *
        differenceX +
        differenceZ *
        differenceZ;


    return
        distanceSquared <=
        radius *
        radius;
}