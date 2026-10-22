#include "stdafx.h"
#include "EnemySpawner.h"
#include "Enemy.h"
#include "Player.h"


// ============================================================
// Update
// ============================================================

void EnemySpawner::Update(
    const Player& player)
{
    RemoveDeadEnemies();


    if (player
        .GetHealth()
        .IsDead())
    {
        return;
    }


    m_spawnTimer -=
        g_gameTime
        ->GetFrameDeltaTime();


    if (m_spawnTimer >
        0.0f)
    {
        return;
    }


    m_spawnTimer =
        0.0f;


    if (m_enemies.size() >=
        MAX_ENEMIES)
    {
        return;
    }


    if (Spawn(
        player))
    {
        m_spawnTimer =
            m_spawnInterval;
    }
    else
    {
        m_spawnTimer =
            1.0f;
    }
}


// ============================================================
// GenerateEnemyLevel
// ============================================================

int EnemySpawner::GenerateEnemyLevel(
    int playerLevel)
{
    // ============================================
    // ±5を2回引いて平均。
    //
    // 0付近が出やすい分布になる。
    // ============================================

    std::uniform_int_distribution<int> offsetRandom(
        -5,
        5
    );


    const int offsetA =
        offsetRandom(
            m_random
        );


    const int offsetB =
        offsetRandom(
            m_random
        );


    int offset =
        (
            offsetA +
            offsetB
            ) /
        2;


    // ============================================
    // 約10%で少し強い/弱い敵
    // ============================================

    std::uniform_int_distribution<int> rareRandom(
        0,
        99
    );


    if (rareRandom(
        m_random) <
        10)
    {
        std::uniform_int_distribution<int> rareOffset(
            -8,
            8
        );


        offset =
            rareOffset(
                m_random
            );
    }


    int level =
        playerLevel +
        offset;


    if (level <
        1)
    {
        level =
            1;
    }


    if (level >
        45)
    {
        level =
            45;
    }


    return level;
}


// ============================================================
// GenerateBuildType
// ============================================================

EnemyEvolution::BuildType
EnemySpawner::GenerateBuildType()
{
    // ============================================
    // 0～99
    // ============================================

    std::uniform_int_distribution<int> buildRandom(
        0,
        99
    );


    const int value =
        buildRandom(
            m_random
        );


    // ============================================
    // 出現率
    //
    // Balanced      15%
    // Twin          17%
    // Sniper        17%
    // Rapid         18%
    // Flank         16%
    // Rammer        17%
    // ============================================

    if (value <
        15)
    {
        return
            EnemyEvolution::Balanced;
    }


    if (value <
        32)
    {
        return
            EnemyEvolution::TwinShooter;
    }


    if (value <
        49)
    {
        return
            EnemyEvolution::SniperShooter;
    }


    if (value <
        67)
    {
        return
            EnemyEvolution::RapidShooter;
    }


    if (value <
        83)
    {
        return
            EnemyEvolution::FlankShooter;
    }


    return
        EnemyEvolution::Rammer;
}


// ============================================================
// Spawn
// ============================================================

bool EnemySpawner::Spawn(
    const Player& player)
{
    std::uniform_real_distribution<float> coordinate(
        -1300.0f,
        1300.0f
    );


    for (int attempt = 0;
        attempt <
        100;
        ++attempt)
    {
        const Vector3 position(
            coordinate(
                m_random
            ),
            0.0f,
            coordinate(
                m_random
            )
        );


        const Vector3 difference =
            position -
            player
            .GetPosition();


        const float distanceSquared =
            difference.x *
            difference.x +
            difference.z *
            difference.z;


        // プレイヤーから500以上離す。
        if (distanceSquared <
            500.0f *
            500.0f)
        {
            continue;
        }


        Enemy* enemy =
            NewGO<Enemy>(
                0,
                "enemy"
            );


        if (enemy ==
            nullptr)
        {
            return false;
        }


        // ========================================
        // 位置
        // ========================================

        enemy->SetPosition(
            position
        );


        // ========================================
        // Enemy Level
        // ========================================

        const int enemyLevel =
            GenerateEnemyLevel(
                player
                .GetProgress()
                .GetLevel()
            );


        // ========================================
        // Build
        // ========================================

        const EnemyEvolution::BuildType buildType =
            GenerateBuildType();


        // ========================================
        // レベル・進化・Stat・AI設定
        // ========================================

        enemy->Configure(
            enemyLevel,
            buildType
        );


        // ========================================
        // Orbit方向
        // ========================================

        std::uniform_int_distribution<int> orbitRandom(
            0,
            1
        );


        enemy->SetOrbitDirection(
            orbitRandom(
                m_random
            ) ==
            0
        );


        m_enemies
            .push_back(
                enemy
            );


        return true;
    }


    return false;
}


// ============================================================
// RemoveDeadEnemies
// ============================================================

void EnemySpawner::RemoveDeadEnemies()
{
    auto iterator =
        m_enemies.begin();


    while (
        iterator !=
        m_enemies.end())
    {
        Enemy* enemy =
            *iterator;


        if (enemy
            ->IsDead())
        {
            DeleteGO(
                enemy
            );


            iterator =
                m_enemies.erase(
                    iterator
                );
        }
        else
        {
            ++iterator;
        }
    }
}


// ============================================================
// Clear
// ============================================================

void EnemySpawner::Clear()
{
    for (
        Enemy* enemy :
        m_enemies)
    {
        DeleteGO(
            enemy
        );
    }


    m_enemies.clear();


    m_spawnTimer =
        m_spawnInterval;
}