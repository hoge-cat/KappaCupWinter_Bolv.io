#include "stdafx.h"
#include "ShapeBoxRenderer.h"

bool ShapeBoxRenderer::Start()
{
    const char* models[TYPE_COUNT] =
    {
        "Assets/modelData/shape/ShapeBox.tkm",
        "Assets/modelData/shape/ShapeTriangle.tkm",
        "Assets/modelData/shape/ShapePentagon.tkm"
    };
    // 種類ごとにまとめて描画し、図形数が増えても描画呼び出しを抑える。
    for (int type = 0; type < TYPE_COUNT; ++type)
    {
        m_modelRender[type].Init(models[type], nullptr, 0,
            EnModelUpAxis::enModelUpAxisY, true, MAX_INSTANCE, false);
        for (int i = 0; i < MAX_INSTANCE; ++i) { m_modelRender[type].RemoveInstance(i); }
    }
    return true;
}

void ShapeBoxRenderer::Render(RenderContext& rc)
{
    int counts[TYPE_COUNT] = {};
    QueryGOs<ShapeBox>("shapeBox", [&](ShapeBox* shape)
    {
        if (!shape || !shape->IsAlive()) { return true; }
        const int type = static_cast<int>(shape->GetType());
        if (counts[type] >= MAX_INSTANCE) { return true; }
        const float scale = shape->GetRenderScale();
        m_modelRender[type].UpdateInstancingData(counts[type]++,
            shape->GetRenderPosition(), shape->GetRotation(), Vector3(scale, scale, scale));
        return true;
    });
    for (int type = 0; type < TYPE_COUNT; ++type)
    {
        // 破壊された図形など、今フレームで使わないインスタンスを消す。
        for (int i = counts[type]; i < m_previousInstanceCount[type]; ++i)
        {
            m_modelRender[type].RemoveInstance(i);
        }
        m_previousInstanceCount[type] = counts[type];
        m_modelRender[type].Draw(rc);
    }
}
