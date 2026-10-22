#pragma once

class ShapeBoxRenderer : public IGameObject
{
public:
    bool Start();
    void Render(RenderContext& rc);

private:
    static const int MAX_INSTANCE =
        200;

    ModelRender m_modelRender;

    int m_previousInstanceCount =
        0;
};