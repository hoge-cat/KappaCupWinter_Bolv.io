#include "stdafx.h"
#include "ShapeBox.h"
#include "Player.h"

#include <cmath>


namespace
{
    // ============================================
    // ShapeBoxの当たり判定範囲
    // ============================================

    // X方向。
    // 左右が少しめり込んでいたので
    // 少しだけ広げる。
    const float COLLISION_HALF_X =
        32.0f;


    // 高さ。
    const float COLLISION_HEIGHT =
        48.0f;


    // ============================================
    // Z方向
    // ============================================

    // 初期カメラ側(-Z側)。
    const float COLLISION_MIN_Z =
        -14.0f;


    // 反対側(+Z側)。
    // 56.0fで見た目と合ったので固定。
    const float COLLISION_MAX_Z =
        56.0f;
}


// ============================================================
// Start
// ============================================================

bool ShapeBox::Start()
{
    return true;
}


// ============================================================
// Render
// ============================================================

void ShapeBox::Render(
    RenderContext& rc)
{}


// ============================================================
// 弾との衝突
// ============================================================

bool ShapeBox::IsHit(
    const Vector3& start,
    const Vector3& end,
    float bulletRadius
) const
{
    if (m_hp <= 0)
    {
        return false;
    }


    // ============================================
    // 箱の判定範囲
    // ============================================

    const float minBounds[3] =
    {
        m_position.x -
            COLLISION_HALF_X -
            bulletRadius,

        m_position.y -
            bulletRadius,

        m_position.z +
            COLLISION_MIN_Z -
            bulletRadius
    };


    const float maxBounds[3] =
    {
        m_position.x +
            COLLISION_HALF_X +
            bulletRadius,

        m_position.y +
            COLLISION_HEIGHT +
            bulletRadius,

        m_position.z +
            COLLISION_MAX_Z +
            bulletRadius
    };


    const float startValues[3] =
    {
        start.x,
        start.y,
        start.z
    };


    const float movement[3] =
    {
        end.x - start.x,
        end.y - start.y,
        end.z - start.z
    };


    float enterTime =
        0.0f;

    float exitTime =
        1.0f;


    // ============================================
    // 線分とAABBの判定
    // ============================================

    for (int axis = 0;
        axis < 3;
        ++axis)
    {
        if (std::fabs(
            movement[axis]) <
            0.0001f)
        {
            if (
                startValues[axis] <
                minBounds[axis] ||
                startValues[axis] >
                maxBounds[axis])
            {
                return false;
            }


            continue;
        }


        float nearTime =
            (
                minBounds[axis] -
                startValues[axis]
                ) /
            movement[axis];


        float farTime =
            (
                maxBounds[axis] -
                startValues[axis]
                ) /
            movement[axis];


        if (nearTime >
            farTime)
        {
            const float temporary =
                nearTime;

            nearTime =
                farTime;

            farTime =
                temporary;
        }


        if (nearTime >
            enterTime)
        {
            enterTime =
                nearTime;
        }


        if (farTime <
            exitTime)
        {
            exitTime =
                farTime;
        }


        if (enterTime >
            exitTime)
        {
            return false;
        }
    }


    return true;
}


// ============================================================
// ダメージ
// ============================================================

void ShapeBox::TakeDamage(
    int damage,
    bool rewardPlayer)
{
    if (m_hp <= 0 ||
        damage <= 0)
    {
        return;
    }


    m_hp -=
        damage;


    if (m_hp <= 0)
    {
        // 5秒後に復活。
        m_respawnTimer =
            5.0f;


        // ========================================
        // 報酬
        // ========================================

        Player* player =
            FindGO<Player>(
                "player"
            );


        if (player != nullptr &&
            rewardPlayer)
        {
            player->AddExperience(
                10
            );


            player->AddScore(
                100
            );
        }
    }
}


// ============================================================
// Update
// ============================================================

void ShapeBox::Update()
{
    if (m_hp > 0)
    {
        return;
    }


    m_respawnTimer -=
        g_gameTime
        ->GetFrameDeltaTime();


    if (m_respawnTimer <=
        0.0f)
    {
        m_hp =
            MAX_HP;


        m_respawnTimer =
            0.0f;
    }
}


// ============================================================
// 自機などとの接触
// ============================================================

bool ShapeBox::IsTouch(
    const Vector3& center,
    float radius
) const
{
    if (m_hp <= 0)
    {
        return false;
    }


    // ============================================
    // 箱の判定範囲
    //
    // X方向は左右に少し広げる。
    // Z方向はモデルの見た目に合わせて非対称。
    // ============================================

    const float minBounds[3] =
    {
        m_position.x -
            COLLISION_HALF_X,

        m_position.y,

        m_position.z +
            COLLISION_MIN_Z
    };


    const float maxBounds[3] =
    {
        m_position.x +
            COLLISION_HALF_X,

        m_position.y +
            COLLISION_HEIGHT,

        m_position.z +
            COLLISION_MAX_Z
    };


    const float values[3] =
    {
        center.x,
        center.y,
        center.z
    };


    float distanceSquared =
        0.0f;


    // ============================================
    // 球の中心から箱の最も近い点を求める
    // ============================================

    for (int axis = 0;
        axis < 3;
        ++axis)
    {
        float closest =
            values[axis];


        if (closest <
            minBounds[axis])
        {
            closest =
                minBounds[axis];
        }
        else if (
            closest >
            maxBounds[axis])
        {
            closest =
                maxBounds[axis];
        }


        const float difference =
            values[axis] -
            closest;


        distanceSquared +=
            difference *
            difference;
    }


    // ============================================
    // 球と箱が接触しているか
    // ============================================

    return
        distanceSquared <=
        radius *
        radius;
}