#include "stdafx.h"
#include "EnemyBulletRenderer.h"
#include "EnemyBullet.h"


// ============================================================
// Start
// ============================================================

bool EnemyBulletRenderer::Start()
{
    // ============================================
    // 敵弾モデル
    //
    // インスタンシング描画。
    // ============================================

    m_modelRender.Init(
        "Assets/modelData/tank/EnemyBullet.tkm",
        nullptr,
        0,
        EnModelUpAxis::enModelUpAxisY,
        true,
        MAX_INSTANCE,
        false
    );


    // ============================================
    // 最初は全Instanceを非表示
    // ============================================

    for (int i = 0;
        i < MAX_INSTANCE;
        ++i)
    {
        m_modelRender.RemoveInstance(
            i
        );
    }


    return true;
}


// ============================================================
// Render
// ============================================================

void EnemyBulletRenderer::Render(
    RenderContext& rc)
{
    int instanceNo =
        0;


    // ============================================
    // 現在存在するEnemyBulletを収集
    // ============================================

    QueryGOs<EnemyBullet>(
        "enemyBullet",
        [&](EnemyBullet* bullet)
        {
            if (bullet ==
                nullptr)
            {
                return true;
            }


            if (bullet
                ->IsDead())
            {
                return true;
            }


            if (instanceNo >=
                MAX_INSTANCE)
            {
                return false;
            }


            // ====================================
            // 位置
            // ====================================

            const Vector3& position =
                bullet
                ->GetPosition();


            // ====================================
            // 回転
            //
            // 弾は球なので不要。
            // ====================================

            const Quaternion rotation =
                Quaternion::Identity;


            // ====================================
            // レベルによるサイズ
            //
            // EnemyBullet::Init()で
            // 当たり判定にも同じLevelScaleが
            // 使用されている。
            // ====================================

            const float levelScale =
                bullet
                ->GetLevelScale();


            const Vector3 scale(
                levelScale,
                levelScale,
                levelScale
            );


            // ====================================
            // Instance更新
            // ====================================

            m_modelRender.UpdateInstancingData(
                instanceNo,
                position,
                rotation,
                scale
            );


            ++instanceNo;


            return true;
        }
    );


    // ============================================
    // 前フレームより弾数が減った場合
    //
    // 余っているInstanceだけ削除。
    // ============================================

    for (int i = instanceNo;
        i < m_previousInstanceCount;
        ++i)
    {
        m_modelRender.RemoveInstance(
            i
        );
    }


    m_previousInstanceCount =
        instanceNo;


    // ============================================
    // 一括描画
    // ============================================

    m_modelRender.Draw(
        rc
    );
}