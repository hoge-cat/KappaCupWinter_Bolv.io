
#include "stdafx.h"
#include "RetryController.h"
#include "Game.h"
#include "Player.h"
#include "TitleUI.h"
#include "LoadingScreen.h"
#include "AudioManager.h"
#include "system/system.h"

// ============================================
// 開始
// ============================================
void RetryController::Start()
{
    CreateTitleScene();
    m_state = State::Title;
}

// ============================================
// 更新
// ============================================
void RetryController::Update()
{
    const bool isActive =
        GetForegroundWindow() == g_hWnd;

    const bool enterPressed =
        isActive &&
        (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;

    const bool rPressed =
        isActive &&
        (GetAsyncKeyState('R') & 0x8000) != 0;

    const bool tPressed =
        isActive &&
        (GetAsyncKeyState('T') & 0x8000) != 0;

    const bool rTriggered =
        rPressed && !m_wasRPressed;

    const bool tTriggered =
        tPressed && !m_wasTPressed;

    m_wasEnterPressed = enterPressed;
    m_wasRPressed = rPressed;
    m_wasTPressed = tPressed;

    // ========================================
    // 古いシーンの削除待ち
    // ========================================
    if (m_state == State::WaitingForDeletion)
    {
        if (!AreOldObjectsDeleted())
        {
            return;
        }

        if (m_nextState == State::Title)
        {
            CreateTitleScene();
            m_state = State::Title;
        }
        else
        {
            // ロード画面を生成
            NewGO<LoadingScreen>(2, "loadingScreen");

            m_loadingFrames = 0;
            m_state = State::Loading;
        }

        return;
    }

    // ========================================
// ロード画面
// ========================================
    if (m_state == State::Loading)
    {
        ++m_loadingFrames;

        if (m_loadingFrames < 2)
        {
            return;
        }

        // ゲームを生成
        NewGO<Game>(0, "game");

        // ゲームの初期化はエンジン側で行われるため、
        // 次のフレームで完了確認する
        m_state = State::WaitingForGameReady;

        return;
    }

    // ========================================
    // ゲームの初期化完了待ち
    // ========================================
    if (m_state == State::WaitingForGameReady)
    {
        Game* game = FindGO<Game>("game");

        if (game == nullptr || !game->IsReady())
        {
            return;
        }

        // BGM開始
        game->StartGameBGM();

        // ロード画面を削除
        LoadingScreen* loading =
            FindGO<LoadingScreen>("loadingScreen");

        if (loading != nullptr)
        {
            DeleteGO(loading);
        }

        m_state = State::Playing;
        return;
    }

    // ========================================
    // タイトル画面
    // ========================================
    if (m_state == State::Title)
    {
        TitleUI* title = FindGO<TitleUI>("titleUI");
        if (title == nullptr || !title->IsStart()) { return; }
        const TitleUI::Action action = title->ConsumeAction();
        if (action == TitleUI::Action::StartGame)
        {
            AudioManager::PlaySelectSE();
            m_nextState = State::Playing;
            ClearCurrentScene();
        }
        else if (action == TitleUI::Action::ExitGame)
        {
            AudioManager::PlaySelectSE();
            PostMessage(g_hWnd, WM_CLOSE, 0, 0);
        }

        return;
    }

    // ========================================
    // インゲーム
    // ========================================
    if (m_state != State::Playing)
    {
        return;
    }

    Player* player = FindGO<Player>("player");

    if (player == nullptr ||
        !player->GetHealth().IsDead())
    {
        return;
    }

    // ========================================
    // ゲームオーバー時の入力
    // ========================================
    if (tTriggered)
    {
        AudioManager::PlaySelectSE();
        m_nextState = State::Title;
        ClearCurrentScene();
    }
    else if (rTriggered)
    {
        AudioManager::PlaySelectSE();
        m_nextState = State::Playing;
        ClearCurrentScene();
    }
}

void RetryController::CreateTitleScene()
{
    NewGO<TitleUI>(2, "titleUI");
    NewGO<AudioManager>(0, "titleAudioManager");
}

// ============================================
// 現在のシーンを削除
// ============================================
void RetryController::ClearCurrentScene()
{
    if (auto* audio = FindGO<AudioManager>("titleAudioManager"))
    {
        audio->StopBGM();
        DeleteGO(audio);
    }

    TitleUI* title = FindGO<TitleUI>("titleUI");

    if (title != nullptr)
    {
        DeleteGO(title);
    }

    Game* game = FindGO<Game>("game");

    if (game != nullptr)
    {
        game->ClearGameObjects();
        DeleteGO(game);
    }

    m_state = State::WaitingForDeletion;
}

// ============================================
// 古いオブジェクトの削除確認
// ============================================
bool RetryController::AreOldObjectsDeleted() const
{
    const char* names[] =
    {
        "titleUI",
        "titleAudioManager",
        "game",
        "background",
        "player",
        "gameCamera",
        "shapeBox",
        "playerHUD",
        "enemy",
        "gameOverUI",
        "bullet",
        "enemyBullet",
        "trap",
        "playerUpgradeUI",
        "playerEvolutionUI",
        "shapeBoxRenderer",
        "bulletRenderer",
        "enemyBulletRenderer",
        "trapRenderer",
        "enemyRenderer",
        "worldHealthBarRenderer",
        "audioManager"
    };

    for (const char* name : names)
    {
        if (FindGO<IGameObject>(name) != nullptr)
        {
            return false;
        }
    }

    return true;
}
