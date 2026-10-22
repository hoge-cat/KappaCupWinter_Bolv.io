#pragma once

#include <vector>

class ShapeBox;

class ShapeSpawner
{
public:
    void CreateShapes();
    void Clear();

private:
    std::vector<ShapeBox*> m_shapes;
};