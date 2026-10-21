#pragma once

#include "StageWall.h"

class Background : public IGameObject
{
public:
    Background();
    ~Background();

    bool Start();
    void Render(RenderContext& rc);

private:
    void CreateWalls();

    ModelRender m_modelRender;
    PhysicsStaticObject m_phyStaticObject;

    StageWall m_walls[4];
};