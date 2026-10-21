#pragma once

#include <vector>
#include <random>

#include "EnemyEvolution.h"


class Enemy;
class Player;


class EnemySpawner
{
public:

    void Update(
        const Player& player
    );


    void Clear();


private:

    bool Spawn(
        const Player& player
    );


    void RemoveDeadEnemies();


    int GenerateEnemyLevel(
        int playerLevel
    );


    EnemyEvolution::BuildType GenerateBuildType();


private:

    std::vector<Enemy*> m_enemies;


    std::mt19937 m_random{
        std::random_device{}()
    };


    float m_spawnTimer =
        5.0f;


    float m_spawnInterval =
        5.0f;


    static constexpr int MAX_ENEMIES =
        3;
};