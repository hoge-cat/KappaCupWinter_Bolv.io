#pragma once

#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstdint>


class ShapeBox;
class Enemy;
class EnemyBullet;


// ============================================================
// 衝突判定用の空間分割
//
// XZ平面を一定サイズのマスに分割し、
// 近くのオブジェクトだけ検索できるようにする。
// ============================================================

class CollisionGrid
{
public:

    // 毎フレーム1回呼んで、
    // 現在の敵・敵弾・図形を登録し直す。
    void Rebuild();


    // ゲーム終了時用。
    void Clear();


    // ========================================================
    // 図形検索
    // ========================================================

    template<class Func>
    bool QueryShapes(
        const Vector3& start,
        const Vector3& end,
        float margin,
        Func&& func
    ) const
    {
        // Windowsのmin/maxマクロとの衝突を避けるため、
        // std::min / std::maxは使用しない。

        const float minWorldX =
            (
                start.x < end.x
                ? start.x
                : end.x
                ) -
            margin;


        const float maxWorldX =
            (
                start.x > end.x
                ? start.x
                : end.x
                ) +
            margin;


        const float minWorldZ =
            (
                start.z < end.z
                ? start.z
                : end.z
                ) -
            margin;


        const float maxWorldZ =
            (
                start.z > end.z
                ? start.z
                : end.z
                ) +
            margin;


        const int minCellX =
            ToCell(
                minWorldX
            );


        const int maxCellX =
            ToCell(
                maxWorldX
            );


        const int minCellZ =
            ToCell(
                minWorldZ
            );


        const int maxCellZ =
            ToCell(
                maxWorldZ
            );


        for (int cellZ = minCellZ;
            cellZ <= maxCellZ;
            ++cellZ)
        {
            for (int cellX = minCellX;
                cellX <= maxCellX;
                ++cellX)
            {
                const Cell* cell =
                    FindCell(
                        cellX,
                        cellZ
                    );


                if (cell ==
                    nullptr)
                {
                    continue;
                }


                for (ShapeBox* shape :
                    cell->shapes)
                {
                    if (!func(shape))
                    {
                        return false;
                    }
                }
            }
        }


        return true;
    }


    // ========================================================
    // 敵戦車検索
    // ========================================================

    template<class Func>
    bool QueryEnemies(
        const Vector3& start,
        const Vector3& end,
        float margin,
        Func&& func
    ) const
    {
        const float minWorldX =
            (
                start.x < end.x
                ? start.x
                : end.x
                ) -
            margin;


        const float maxWorldX =
            (
                start.x > end.x
                ? start.x
                : end.x
                ) +
            margin;


        const float minWorldZ =
            (
                start.z < end.z
                ? start.z
                : end.z
                ) -
            margin;


        const float maxWorldZ =
            (
                start.z > end.z
                ? start.z
                : end.z
                ) +
            margin;


        const int minCellX =
            ToCell(
                minWorldX
            );


        const int maxCellX =
            ToCell(
                maxWorldX
            );


        const int minCellZ =
            ToCell(
                minWorldZ
            );


        const int maxCellZ =
            ToCell(
                maxWorldZ
            );


        for (int cellZ = minCellZ;
            cellZ <= maxCellZ;
            ++cellZ)
        {
            for (int cellX = minCellX;
                cellX <= maxCellX;
                ++cellX)
            {
                const Cell* cell =
                    FindCell(
                        cellX,
                        cellZ
                    );


                if (cell ==
                    nullptr)
                {
                    continue;
                }


                for (Enemy* enemy :
                    cell->enemies)
                {
                    if (!func(enemy))
                    {
                        return false;
                    }
                }
            }
        }


        return true;
    }


    // ========================================================
    // 敵弾検索
    // ========================================================

    template<class Func>
    bool QueryEnemyBullets(
        const Vector3& start,
        const Vector3& end,
        float margin,
        Func&& func
    ) const
    {
        const float minWorldX =
            (
                start.x < end.x
                ? start.x
                : end.x
                ) -
            margin;


        const float maxWorldX =
            (
                start.x > end.x
                ? start.x
                : end.x
                ) +
            margin;


        const float minWorldZ =
            (
                start.z < end.z
                ? start.z
                : end.z
                ) -
            margin;


        const float maxWorldZ =
            (
                start.z > end.z
                ? start.z
                : end.z
                ) +
            margin;


        const int minCellX =
            ToCell(
                minWorldX
            );


        const int maxCellX =
            ToCell(
                maxWorldX
            );


        const int minCellZ =
            ToCell(
                minWorldZ
            );


        const int maxCellZ =
            ToCell(
                maxWorldZ
            );


        for (int cellZ = minCellZ;
            cellZ <= maxCellZ;
            ++cellZ)
        {
            for (int cellX = minCellX;
                cellX <= maxCellX;
                ++cellX)
            {
                const Cell* cell =
                    FindCell(
                        cellX,
                        cellZ
                    );


                if (cell ==
                    nullptr)
                {
                    continue;
                }


                for (EnemyBullet* bullet :
                    cell->enemyBullets)
                {
                    if (!func(bullet))
                    {
                        return false;
                    }
                }
            }
        }


        return true;
    }


private:

    // ========================================================
    // 1マス
    // ========================================================

    struct Cell
    {
        std::vector<ShapeBox*>
            shapes;

        std::vector<Enemy*>
            enemies;

        std::vector<EnemyBullet*>
            enemyBullets;
    };


    // ========================================================
    // 1マスの大きさ
    // ========================================================

    static float GetCellSize()
    {
        return 256.0f;
    }


    // ========================================================
    // ワールド座標 → マス番号
    // ========================================================

    static int ToCell(
        float position
    )
    {
        return static_cast<int>(
            std::floor(
                position /
                GetCellSize()
            )
            );
    }


    // ========================================================
    // Cellのキー生成
    // ========================================================

    static std::int64_t MakeKey(
        int cellX,
        int cellZ
    )
    {
        const std::uint64_t x =
            static_cast<std::uint32_t>(
                cellX
                );


        const std::uint64_t z =
            static_cast<std::uint32_t>(
                cellZ
                );


        return static_cast<std::int64_t>(
            (x << 32) |
            z
            );
    }


    // ========================================================
    // 指定Cellを取得
    // ========================================================

    const Cell* FindCell(
        int cellX,
        int cellZ
    ) const
    {
        const auto it =
            m_cells.find(
                MakeKey(
                    cellX,
                    cellZ
                )
            );


        if (it ==
            m_cells.end())
        {
            return nullptr;
        }


        return
            &it->second;
    }


    // ========================================================
    // オブジェクト登録
    // ========================================================

    void AddShape(
        ShapeBox* shape
    );


    void AddEnemy(
        Enemy* enemy
    );


    void AddEnemyBullet(
        EnemyBullet* bullet
    );


    // ========================================================
    // Cell一覧
    // ========================================================

    std::unordered_map<
        std::int64_t,
        Cell
    > m_cells;
};


// ゲーム中に使用するCollisionGrid。
extern CollisionGrid* g_collisionGrid;