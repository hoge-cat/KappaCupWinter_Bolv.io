#include "stdafx.h"
#include "PlayerContact.h"
#include "Player.h"
#include "ShapeBox.h"


namespace
{
    // ============================================
    // 本家風連続接触
    // ============================================

    const float CONTACT_DAMAGE_INTERVAL =
        0.10f;


    // ============================================
    // Shapeとのノックバック
    // ============================================

    const float CONTACT_KNOCKBACK_INTERVAL =
        0.06f;


    const float SHAPE_KNOCKBACK_POWER =
        75.0f;


    // ============================================
    // Shape側Body Damage
    //
    // Shape種類別対応前の暫定値。
    // ============================================

    const int SHAPE_BODY_DAMAGE =
        20;
}


// ============================================================
// Update
// ============================================================

void PlayerContact::Update(
    Player& player)
{
    const float deltaTime =
        g_gameTime
        ->GetFrameDeltaTime();


    // ============================================
    // タイマー
    // ============================================

    m_damageTimer -=
        deltaTime;


    m_knockbackTimer -=
        deltaTime;


    if (player
        .GetHealth()
        .IsDead())
    {
        return;
    }


    // ============================================
    // Player中心
    // ============================================

    Vector3 center =
        player
        .GetPosition();


    center.y +=
        30.0f;


    const float collisionRadius =
        player
        .GetCollisionRadius();


    // ============================================
    // Shapeとの接触
    // ============================================

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
                    center,
                    collisionRadius
                ))
            {
                return true;
            }


            // ====================================
            // ノックバック
            // ====================================

            if (m_knockbackTimer <=
                0.0f)
            {
                Vector3 knockbackDirection =
                    player
                    .GetPosition() -
                    shape
                    ->GetPosition();


                knockbackDirection.y =
                    0.0f;


                player.AddKnockback(
                    knockbackDirection,
                    SHAPE_KNOCKBACK_POWER
                );


                m_knockbackTimer =
                    CONTACT_KNOCKBACK_INTERVAL;
            }


            // ====================================
            // Body Damage
            // ====================================

            if (m_damageTimer <=
                0.0f)
            {
                // Player → Shape
                //
                // 20 + 4 * BodyDamageLevel
                shape->TakeDamage(
                    player
                    .GetBodyDamageVsShape()
                );


                // Shape → Player
                player.TakeDamage(
                    SHAPE_BODY_DAMAGE
                );


                m_damageTimer =
                    CONTACT_DAMAGE_INTERVAL;
            }


            return false;
        }
    );
}