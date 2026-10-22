#pragma once

class EnemyBulletRenderer : public IGameObject
{
public:
    bool Start();
    void Render(RenderContext& rc);

private:
    static const int MAX_INSTANCE =
        2000;

    ModelRender m_modelRender;

    int m_previousInstanceCount =
        0;
};