#include "stdafx.h"
#include "WorldHealthBarRenderer.h"

#include "Enemy.h"
#include "ShapeBox.h"


namespace
{
    // ============================================
    // HPバー基本サイズ
    // ============================================

    const float BAR_HEIGHT =
        7.0f;


    // ============================================
    // 背景を少し大きくする
    // ============================================

    const float BACKGROUND_SCALE =
        1.12f;


    // ============================================
    // 横方向だけスクリーン座標補正
    //
    // 1.0では画面端で内側へずれる。
    // 1.5では外側へ行き過ぎた。
    //
    // まず1.15で確認。
    // ============================================

    const float SCREEN_POSITION_SCALE_X =
        1.12f;


    // ============================================
    // ShapeBoxの中心補正
    //
    // 現在の当たり判定・モデル形状に合わせて
    // Z方向へ少し補正。
    // ============================================

    const float SHAPE_CENTER_OFFSET_Z =
        21.0f;


    // ============================================
    // Shape HPバー高さ
    // ============================================

    const float SHAPE_HEALTH_BAR_HEIGHT =
        58.0f;


    // ============================================
    // Enemy HPバー高さ
    // ============================================

    const float ENEMY_HEALTH_BAR_HEIGHT =
        72.0f;
}


// ============================================================
// Start
// ============================================================

bool WorldHealthBarRenderer::Start()
{
    for (int i = 0;
        i < MAX_HEALTH_BARS;
        ++i)
    {
        // ========================================
        // 背景
        // ========================================

        m_healthBars[i]
            .background
            .Init(
                "Assets/sprite/White.dds",
                100.0f,
                BAR_HEIGHT
            );


        m_healthBars[i]
            .background
            .SetMulColor(
                Vector4(
                    0.15f,
                    0.15f,
                    0.15f,
                    0.90f
                )
            );


        // ========================================
        // HP部分
        // ========================================

        m_healthBars[i]
            .health
            .Init(
                "Assets/sprite/White.dds",
                100.0f,
                BAR_HEIGHT
            );


        m_healthBars[i]
            .health
            .SetMulColor(
                Vector4(
                    0.25f,
                    0.85f,
                    0.25f,
                    1.0f
                )
            );
    }


    return true;
}


// ============================================================
// DrawHealthBar
// ============================================================

bool WorldHealthBarRenderer::DrawHealthBar(
    RenderContext& rc,
    int index,
    const Vector3& worldPosition,
    int currentHP,
    int maxHP,
    float width)
{
    // ============================================
    // Index確認
    // ============================================

    if (index <
        0 ||
        index >=
        MAX_HEALTH_BARS)
    {
        return false;
    }


    // ============================================
    // HP確認
    // ============================================

    if (currentHP <=
        0)
    {
        return false;
    }


    if (maxHP <=
        0)
    {
        return false;
    }


    // ============================================
    // HP満タンなら非表示
    // ============================================

    if (currentHP >=
        maxHP)
    {
        return false;
    }


    // ============================================
    // カメラより後ろなら非表示
    // ============================================

    const Vector3 toObject =
        worldPosition -
        g_camera3D
        ->GetPosition();


    const Vector3 cameraForward =
        g_camera3D
        ->GetForward();


    const float forwardDot =
        toObject.x *
        cameraForward.x +
        toObject.y *
        cameraForward.y +
        toObject.z *
        cameraForward.z;


    if (forwardDot <=
        0.0f)
    {
        return false;
    }


    // ============================================
    // World → Screen
    // ============================================

    Vector2 screenPosition;


    g_camera3D
        ->CalcScreenPositionFromWorldPosition(
            screenPosition,
            worldPosition
        );


    // ============================================
    // 横方向だけ補正
    //
    // 縦方向はそのまま使う。
    // ============================================

    screenPosition.x *=
        SCREEN_POSITION_SCALE_X;


    // screenPosition.y は補正しない。


    // ============================================
    // HP割合
    // ============================================

    float hpRate =
        static_cast<float>(
            currentHP
            ) /
        static_cast<float>(
            maxHP
            );


    if (hpRate <
        0.0f)
    {
        hpRate =
            0.0f;
    }


    if (hpRate >
        1.0f)
    {
        hpRate =
            1.0f;
    }


    // ============================================
    // 背景
    // ============================================

    SpriteRender& background =
        m_healthBars[
            index
        ]
        .background;


    background.SetPosition(
        Vector3(
            screenPosition.x,
            screenPosition.y,
            0.0f
        )
    );


    background.SetScale(
        Vector3(
            width /
            100.0f *
            BACKGROUND_SCALE,

            BACKGROUND_SCALE,

            1.0f
        )
    );


    background.Update();


    background.Draw(
        rc
    );


    // ============================================
    // HP部分
    //
    // 左端を固定し、
    // HPが減ると右側から縮む。
    // ============================================

    const float healthWidth =
        width *
        hpRate;


    const float leftEdge =
        screenPosition.x -
        width *
        0.5f;


    const float healthCenterX =
        leftEdge +
        healthWidth *
        0.5f;


    SpriteRender& health =
        m_healthBars[
            index
        ]
        .health;


    health.SetPosition(
        Vector3(
            healthCenterX,
            screenPosition.y,
            0.0f
        )
    );


    health.SetScale(
        Vector3(
            healthWidth /
            100.0f,

            1.0f,

            1.0f
        )
    );


    health.Update();


    health.Draw(
        rc
    );


    return true;
}


// ============================================================
// Render
// ============================================================

void WorldHealthBarRenderer::Render(
    RenderContext& rc)
{
    int barIndex =
        0;


    // ========================================================
    // Enemy
    // ========================================================

    QueryGOs<Enemy>(
        "enemy",

        [&](Enemy* enemy)
        {
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


            if (barIndex >=
                MAX_HEALTH_BARS)
            {
                return false;
            }


            // ====================================
            // 満タンなら非表示
            // ====================================

            if (enemy
                ->GetHP() >=
                enemy
                ->GetMaxHP())
            {
                return true;
            }


            const float levelScale =
                enemy
                ->GetLevelScale();


            // ====================================
            // Enemy頭上
            // ====================================

            Vector3 barPosition =
                enemy
                ->GetPosition();


            barPosition.y +=
                ENEMY_HEALTH_BAR_HEIGHT *
                levelScale;


            // ====================================
            // 描画
            // ====================================

            const bool drawn =
                DrawHealthBar(
                    rc,
                    barIndex,
                    barPosition,

                    enemy
                    ->GetHP(),

                    enemy
                    ->GetMaxHP(),

                    72.0f *
                    levelScale
                );


            if (drawn)
            {
                ++barIndex;
            }


            return true;
        }
    );


    // ========================================================
    // ShapeBox
    // ========================================================

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


            if (barIndex >=
                MAX_HEALTH_BARS)
            {
                return false;
            }


            // ====================================
            // 満タンなら非表示
            // ====================================

            if (!shape
                ->IsDamaged())
            {
                return true;
            }


            // ====================================
            // Shapeの中心
            // ====================================

            Vector3 barPosition =
                shape
                ->GetPosition();


            // ====================================
            // ShapeBox中心補正
            // ====================================

            barPosition.z +=
                SHAPE_CENTER_OFFSET_Z;


            // ====================================
            // 頭上
            // ====================================

            barPosition.y +=
                SHAPE_HEALTH_BAR_HEIGHT;


            // ====================================
            // 描画
            // ====================================

            const bool drawn =
                DrawHealthBar(
                    rc,
                    barIndex,
                    barPosition,

                    shape
                    ->GetHP(),

                    shape
                    ->GetMaxHP(),

                    55.0f
                );


            if (drawn)
            {
                ++barIndex;
            }


            return true;
        }
    );
}