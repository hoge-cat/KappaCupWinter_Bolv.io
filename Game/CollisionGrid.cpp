#include "stdafx.h"
#include "CollisionGrid.h"

#include "ShapeBox.h"
#include "Enemy.h"
#include "EnemyBullet.h"


CollisionGrid* g_collisionGrid =
nullptr;


// ============================================================
// 全登録を更新
// ============================================================

void CollisionGrid::Rebuild()
{
    // ============================================
    // 既存のCell自体は残す。
    //
    // unordered_mapを毎フレームclearすると、
    // 再確保が発生しやすいため、
    // 中身のvectorだけ空にする。
    // ============================================

    for (auto& pair :
        m_cells)
    {
        pair.second.shapes.clear();

        pair.second.enemies.clear();

        pair.second.enemyBullets.clear();
    }


    // ============================================
    // 図形
    // ============================================

    QueryGOs<ShapeBox>(
        "shapeBox",
        [&](ShapeBox* shape)
        {
            if (shape ==
                nullptr)
            {
                return true;
            }

            if (!shape->IsAlive())
            {
                return true;
            }

            AddShape(
                shape
            );

            return true;
        }
    );


    // ============================================
    // 敵戦車
    // ============================================

    QueryGOs<Enemy>(
        "enemy",
        [&](Enemy* enemy)
        {
            if (enemy ==
                nullptr)
            {
                return true;
            }

            if (enemy->IsDead())
            {
                return true;
            }

            AddEnemy(
                enemy
            );

            return true;
        }
    );


    // ============================================
    // 敵弾
    // ============================================

    QueryGOs<EnemyBullet>(
        "enemyBullet",
        [&](EnemyBullet* bullet)
        {
            if (bullet ==
                nullptr)
            {
                return true;
            }

            if (bullet->IsDead())
            {
                return true;
            }

            AddEnemyBullet(
                bullet
            );

            return true;
        }
    );
}


// ============================================================
// Clear
// ============================================================

void CollisionGrid::Clear()
{
    m_cells.clear();
}


// ============================================================
// Shape登録
// ============================================================

void CollisionGrid::AddShape(
    ShapeBox* shape)
{
    if (shape ==
        nullptr)
    {
        return;
    }


    const Vector3& position =
        shape->GetPosition();


    const int cellX =
        ToCell(
            position.x
        );

    const int cellZ =
        ToCell(
            position.z
        );


    m_cells[
        MakeKey(
            cellX,
            cellZ
        )
    ].shapes.push_back(
        shape
    );
}


// ============================================================
// Enemy登録
// ============================================================

void CollisionGrid::AddEnemy(
    Enemy* enemy)
{
    if (enemy ==
        nullptr)
    {
        return;
    }


    const Vector3& position =
        enemy->GetPosition();


    const int cellX =
        ToCell(
            position.x
        );

    const int cellZ =
        ToCell(
            position.z
        );


    m_cells[
        MakeKey(
            cellX,
            cellZ
        )
    ].enemies.push_back(
        enemy
    );
}


// ============================================================
// EnemyBullet登録
// ============================================================

void CollisionGrid::AddEnemyBullet(
    EnemyBullet* bullet)
{
    if (bullet ==
        nullptr)
    {
        return;
    }


    const Vector3& position =
        bullet->GetPosition();


    const int cellX =
        ToCell(
            position.x
        );

    const int cellZ =
        ToCell(
            position.z
        );


    m_cells[
        MakeKey(
            cellX,
            cellZ
        )
    ].enemyBullets.push_back(
        bullet
    );
}