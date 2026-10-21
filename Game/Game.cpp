#include "stdafx.h"
#include "Game.h"

#include "Background.h"
#include "Player.h"
#include "GameCamera.h"
#include "ShapeBox.h"
#include "PlayerHUD.h"
#include "Enemy.h"
#include "GameOverUI.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "PlayerUpgradeUI.h"
#include "ShapeBoxRenderer.h"
#include "PlayerEvolutionUI.h"
#include "BulletRenderer.h"
#include "EnemyBulletRenderer.h"
#include "EnemyRenderer.h"
#include "WorldHealthBarRenderer.h"
#include "Trap.h"
#include "TrapRenderer.h"


Game::~Game()
{
    ClearGameObjects();
}


bool Game::Start()
{
    // ============================================
    // CollisionGridを使用可能にする
    // ============================================

    g_collisionGrid =
        &m_collisionGrid;


    // ============================================
    // ゲームオブジェクト生成
    // ============================================

    m_background =
        NewGO<Background>(
            0,
            "background"
        );


    m_player =
        NewGO<Player>(
            0,
            "player"
        );


    m_gameCamera =
        NewGO<GameCamera>(
            0,
            "gameCamera"
        );

    m_worldHealthBarRenderer =
        NewGO<WorldHealthBarRenderer>(
            10,
            "worldHealthBarRenderer"
        );

    // ============================================
    // 図形
    // ============================================

    m_shapeSpawner.CreateShapes();


    m_shapeBoxRenderer =
        NewGO<ShapeBoxRenderer>(
            0,
            "shapeBoxRenderer"
        );


    // ============================================
    // 自機弾Renderer
    // ============================================

    m_bulletRenderer =
        NewGO<BulletRenderer>(
            0,
            "bulletRenderer"
        );


    // ============================================
    // 敵弾Renderer
    // ============================================

    m_enemyBulletRenderer =
        NewGO<EnemyBulletRenderer>(
            0,
            "enemyBulletRenderer"
        );


    // ============================================
    // Trap Renderer
    // ============================================

    m_trapRenderer =
        NewGO<TrapRenderer>(
            0,
            "trapRenderer"
        );


    m_enemyRenderer =
        NewGO<EnemyRenderer>(
            0,
            "enemyRenderer"
        );


    // ============================================
    // 敵Renderer
    // ============================================

    m_enemyRenderer =
        NewGO<EnemyRenderer>(
            0,
            "enemyRenderer"
        );


    // ============================================
    // UI
    // ============================================

    m_playerHUD =
        NewGO<PlayerHUD>(
            1,
            "playerHUD"
        );


    m_playerEvolutionUI =
        NewGO<PlayerEvolutionUI>(
            1,
            "playerEvolutionUI"
        );


    m_playerUpgradeUI =
        NewGO<PlayerUpgradeUI>(
            1,
            "playerUpgradeUI"
        );


    m_gameOverUI =
        NewGO<GameOverUI>(
            2,
            "gameOverUI"
        );


    // 最初のGridを作る。
    m_collisionGrid.Rebuild();


    return true;
}


void Game::Update()
{
    // ============================================
    // 敵生成
    // ============================================

    if (m_player !=
        nullptr)
    {
        m_enemySpawner.Update(
            *m_player
        );
    }


    // ============================================
    // 1フレームに1回だけ
    // 衝突Gridを更新
    // ============================================

    m_collisionGrid.Rebuild();
}


void Game::Render(
    RenderContext& rc)
{}


void Game::ClearGameObjects()
{
    // ============================================
    // 発射済み自機弾
    // ============================================

    QueryGOs<Bullet>(
        "bullet",
        [](Bullet* bullet)
        {
            DeleteGO(
                bullet
            );

            return true;
        }
    );


    // ============================================
    // 発射済み敵弾
    // ============================================

    QueryGOs<EnemyBullet>(
        "enemyBullet",
        [](EnemyBullet* bullet)
        {
            DeleteGO(
                bullet
            );

            return true;
        }
    );


    // ============================================
    // 発射済みTrap
    // ============================================

    QueryGOs<Trap>(
        "trap",
        [](Trap* trap)
        {
            DeleteGO(
                trap
            );

            return true;
        }
    );


    // ============================================
    // UI
    // ============================================

    DeleteGO(
        m_gameOverUI
    );

    m_gameOverUI =
        nullptr;


    DeleteGO(
        m_playerUpgradeUI
    );

    m_playerUpgradeUI =
        nullptr;


    DeleteGO(
        m_playerEvolutionUI
    );

    m_playerEvolutionUI =
        nullptr;


    DeleteGO(
        m_playerHUD
    );

    m_playerHUD =
        nullptr;


    // ============================================
    // 敵
    // ============================================

    m_enemySpawner.Clear();


    DeleteGO(
        m_enemyRenderer
    );

    m_enemyRenderer =
        nullptr;


    // ============================================
    // カメラ
    // ============================================

    DeleteGO(
        m_gameCamera
    );

    m_gameCamera =
        nullptr;


    // ============================================
    // 弾Renderer
    // ============================================

    DeleteGO(
        m_enemyBulletRenderer
    );

    m_enemyBulletRenderer =
        nullptr;


    DeleteGO(
        m_trapRenderer
    );

    m_trapRenderer =
        nullptr;


    DeleteGO(
        m_bulletRenderer
    );

    m_bulletRenderer =
        nullptr;


    // ============================================
    // 図形
    // ============================================

    DeleteGO(
        m_shapeBoxRenderer
    );

    m_shapeBoxRenderer =
        nullptr;


    m_shapeSpawner.Clear();


    // ============================================
    // Player
    // ============================================

    DeleteGO(
        m_player
    );

    m_player =
        nullptr;


    // ============================================
    // 背景
    // ============================================

    DeleteGO(
        m_background
    );

    m_background =
        nullptr;


    // ============================================
    // CollisionGrid
    // ============================================

    m_collisionGrid.Clear();

    g_collisionGrid =
        nullptr;
}