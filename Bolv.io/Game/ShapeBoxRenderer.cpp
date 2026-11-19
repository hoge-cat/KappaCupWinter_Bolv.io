#include "stdafx.h"
#include "ShapeBoxRenderer.h"
#include "ShapeBox.h"

bool ShapeBoxRenderer::Start()
{
    m_modelRender.Init(
        "Assets/modelData/shape/ShapeBox.tkm",
        nullptr,
        0,
        EnModelUpAxis::enModelUpAxisY,
        true,
        MAX_INSTANCE,
        false
    );

    // 最初は全インスタンスを非表示にする。
    for (int i = 0;
        i < MAX_INSTANCE;
        ++i)
    {
        m_modelRender.RemoveInstance(i);
    }

    return true;
}

void ShapeBoxRenderer::Render(RenderContext& rc)
{
    int instanceNo = 0;

    QueryGOs<ShapeBox>(
        "shapeBox",
        [&](ShapeBox* shapeBox)
        {
            if (shapeBox == nullptr)
            {
                return true;
            }

            // 壊れている図形は描画しない。
            if (!shapeBox->IsAlive())
            {
                return true;
            }

            // 最大数を超えない。
            if (instanceNo >= MAX_INSTANCE)
            {
                return false;
            }

            const Vector3 renderPosition = shapeBox->GetRenderPosition();
            const Quaternion rotation = shapeBox->GetRotation();

            m_modelRender.UpdateInstancingData(
                instanceNo,
                renderPosition,
                rotation,
                Vector3(
                    1.6f,
                    1.6f,
                    1.6f
                )
            );

            ++instanceNo;

            return true;
        }
    );

    // 今フレームで使っていない
    // インスタンスを消す。
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

    m_modelRender.Draw(rc);
}