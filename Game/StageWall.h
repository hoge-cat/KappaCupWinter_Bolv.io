#pragma once

class StageWall
{
public:
    void Init(const Vector3& position, const Vector3& scale);
    void Render(RenderContext& rc);

private:
    ModelRender m_modelRender;
    PhysicsStaticObject m_phyStaticObject;
};