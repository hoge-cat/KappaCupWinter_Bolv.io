#pragma once
#include "ShapeBox.h"

class ShapeBoxRenderer : public IGameObject
{
public:
    bool Start();
    void Render(RenderContext& rc);

private:
    static const int MAX_INSTANCE =
        200;

    static constexpr int TYPE_COUNT = static_cast<int>(ShapeBox::Type::Count);
    ModelRender m_modelRender[TYPE_COUNT];

    int m_previousInstanceCount[TYPE_COUNT] = {};
};