#pragma once

#include "ShapeSpawner.h"
#include "EnemySpawner.h"
#include "CollisionGrid.h"


class Background;
class Player;
class GameCamera;
class ShapeBox;
class PlayerHUD;
class Enemy;
class GameOverUI;
class PlayerUpgradeUI;
class ShapeBoxRenderer;
class PlayerEvolutionUI;
class BulletRenderer;
class EnemyBulletRenderer;
class EnemyRenderer;
class WorldHealthBarRenderer;
class TrapRenderer;


class Game : public IGameObject
{
public:
    Game()
    {}

    ~Game();

    bool Start();
    void Update();
    void Render(RenderContext& rc);

    void ClearGameObjects();


private:
    Background* m_background =
        nullptr;

    Player* m_player =
        nullptr;

    GameCamera* m_gameCamera =
        nullptr;

    PlayerHUD* m_playerHUD =
        nullptr;

    GameOverUI* m_gameOverUI =
        nullptr;

    PlayerUpgradeUI* m_playerUpgradeUI =
        nullptr;

    ShapeBoxRenderer* m_shapeBoxRenderer =
        nullptr;

    PlayerEvolutionUI* m_playerEvolutionUI =
        nullptr;

    BulletRenderer* m_bulletRenderer =
        nullptr;

    EnemyBulletRenderer* m_enemyBulletRenderer =
        nullptr;

    TrapRenderer* m_trapRenderer =
        nullptr;

    EnemyRenderer* m_enemyRenderer =
        nullptr;

    WorldHealthBarRenderer*
        m_worldHealthBarRenderer =
        nullptr;


    // ============================================
    // 衝突判定用Uniform Grid
    // ============================================

    CollisionGrid m_collisionGrid;


    ShapeSpawner m_shapeSpawner;
    EnemySpawner m_enemySpawner;
};