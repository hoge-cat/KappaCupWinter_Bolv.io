#include "stdafx.h"
#include "ShapeSpawner.h"
#include "ShapeBox.h"
#include <random>

void ShapeSpawner::CreateShapes()
{
    Clear();

    std::mt19937 random(std::random_device{}());
    std::uniform_real_distribution<float> coordinate(
        -1400.0f, 1400.0f
    );

    std::vector<Vector3> positions;

    for (int i = 0; i < 30; ++i)
    {
        // 配置候補を最大100回探す。
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            const Vector3 position(
                coordinate(random),
                0.0f,
                coordinate(random)
            );

            // 自機の開始位置の周囲を空ける。
            if (position.x * position.x +
                position.z * position.z < 200.0f * 200.0f)
            {
                continue;
            }

            bool tooClose = false;

            for (const Vector3& other : positions)
            {
                const float dx = position.x - other.x;
                const float dz = position.z - other.z;

                if (dx * dx + dz * dz < 100.0f * 100.0f)
                {
                    tooClose = true;
                    break;
                }
            }

            if (tooClose)
            {
                continue;
            }

            ShapeBox* shape = NewGO<ShapeBox>(
                0, "shapeBox"
            );

            if (shape != nullptr)
            {
                shape->SetPosition(position);
                m_shapes.push_back(shape);
                positions.push_back(position);
            }

            break;
        }
    }
}

void ShapeSpawner::Clear()
{
    for (ShapeBox* shape : m_shapes)
    {
        DeleteGO(shape);
    }

    m_shapes.clear();
}