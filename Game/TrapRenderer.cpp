#include "stdafx.h"

#include "TrapRenderer.h"
#include "Trap.h"


// ============================================================
// Start
// ============================================================

bool TrapRenderer::Start()
{
    // ========================================================
    // Trapモデルをインスタンシング対応で初期化
    // ========================================================

    m_modelRender.Init(
        "Assets/modelData/tank/Trap.tkm",
        nullptr,
        0,
        EnModelUpAxis::enModelUpAxisY,
        true,
        MAX_INSTANCE,
        false
    );


    // ========================================================
    // 最初は全インスタンスを非表示
    // ========================================================

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

void TrapRenderer::Render(
    RenderContext& rc
)
{
    int instanceNo =
        0;


    // ========================================================
    // 現在存在しているTrapを収集
    // ========================================================

    QueryGOs<Trap>(
        "trap",

        [&](Trap* trap)
        {
            // ================================================
            // 無効Trap
            // ================================================

            if (trap ==
                nullptr)
            {
                return true;
            }


            if (trap
                ->IsDead())
            {
                return true;
            }


            // ================================================
            // 最大数
            // ================================================

            if (instanceNo >=
                MAX_INSTANCE)
            {
                return false;
            }


            // ================================================
            // Position
            // ================================================

            const Vector3& position =
                trap
                ->GetPosition();


            // ================================================
            // Trap本来の回転
            // ================================================

            const Quaternion& trapRotation =
                trap->GetRotation();


            // ================================================
            // モデル姿勢補正
            // 3ds Max(Z-up) → ゲーム側で地面に寝かせる
            // ================================================

            Quaternion modelCorrection;
            modelCorrection.SetRotationDeg(
                Vector3::AxisX,
                90.0f
            );


            // ================================================
            // 最終回転
            // 掛け順が重要
            // ================================================

            Quaternion rotation =
                trapRotation *
                modelCorrection;


            // ================================================
            // Scale
            //
            // Trap.max自体が
            // Outer Radius = 12
            //
            // なので追加縮小せず、
            // LevelScaleだけ適用。
            // ================================================

            const float levelScale =
                trap
                ->GetLevelScale();


            const Vector3 scale(
                levelScale,
                levelScale,
                levelScale
            );


            // ================================================
            // Instance更新
            // ================================================

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


    // ========================================================
    // 前フレームでは存在したが
    // 今フレームでは使わないInstanceを消す
    // ========================================================

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


    // ========================================================
    // 全Trapを1回で描画
    // ========================================================

    m_modelRender.Draw(
        rc
    );
}