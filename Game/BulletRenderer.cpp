#include "stdafx.h"
#include "BulletRenderer.h"
#include "Bullet.h"

bool BulletRenderer::Start()
{
    // ============================================
    // 弾モデルをインスタンシング対応で初期化
    // ============================================

    m_modelRender.Init(
        "Assets/modelData/tank/Bullet.tkm",
        nullptr,
        0,
        EnModelUpAxis::enModelUpAxisY,
        true,
        MAX_INSTANCE,
        false
    );

    // 最初は全インスタンスを非表示。
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


void BulletRenderer::Render(
    RenderContext& rc)
{
    int instanceNo = 0;

    // ============================================
    // 現在存在している自機弾を集める
    // ============================================

    QueryGOs<Bullet>(
        "bullet",
        [&](Bullet* bullet)
        {
            if (bullet ==
                nullptr)
            {
                return true;
            }

            // 最大インスタンス数を超えたら終了。
            if (instanceNo >=
                MAX_INSTANCE)
            {
                return false;
            }


            // ====================================
            // 位置
            // ====================================

            const Vector3& position =
                bullet->GetPosition();


            // ====================================
            // 回転
            // ====================================

            // 現在の弾は球なので、
            // 回転させる必要はない。
            Quaternion rotation =
                Quaternion::Identity;


            // ====================================
            // サイズ
            // ====================================

            const float levelScale =
                bullet->GetLevelScale();

            const Vector3 scale(
                levelScale,
                levelScale,
                levelScale
            );


            // ====================================
            // インスタンス情報更新
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
    // 前フレームで使っていた余分な弾を消す
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
    // 全自機弾をまとめて描画
    // ============================================

    m_modelRender.Draw(
        rc
    );
}