#include "stdafx.h"
#include "StageWall.h"

void StageWall::Init(
    const Vector3& position,
    const Vector3& scale)
{
    m_modelRender.Init(
        "Assets/modelData/stage/StageWall.tkm"
    );

    m_modelRender.SetPosition(position);
    m_modelRender.SetScale(scale);
    m_modelRender.Update();

    // 拡大・配置後のモデルから当たり判定を作成。
    m_phyStaticObject.CreateFromModel(
        m_modelRender.GetModel(),
        m_modelRender.GetModel().GetWorldMatrix()
    );
}

void StageWall::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);
}