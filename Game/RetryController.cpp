#include "stdafx.h"
#include "RetryController.h"
#include "Game.h"
#include "Player.h"
#include "TitleUI.h"
#include "system/system.h"

void RetryController::Start()
{
    NewGO<TitleUI>(2, "titleUI");
    m_state = State::Title;
}

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

    const bool enterTriggered =
        enterPressed && !m_wasEnterPressed;
    const bool rTriggered =
        rPressed && !m_wasRPressed;
    const bool tTriggered =
        tPressed && !m_wasTPressed;

    m_wasEnterPressed = enterPressed;
    m_wasRPressed = rPressed;
    m_wasTPressed = tPressed;

    // 削除完了後に、次の画面を作る。
    if (m_state == State::WaitingForDeletion)
    {
        if (!AreOldObjectsDeleted())
        {
            return;
        }

        if (m_nextState == State::Title)
        {
            NewGO<TitleUI>(2, "titleUI");
        }
        else
        {
            NewGO<Game>(0, "game");
        }

        m_state = m_nextState;
        return;
    }

    // タイトルではEnterで開始する。
    if (m_state == State::Title)
    {
        if (enterTriggered)
        {
            m_nextState = State::Playing;
            ClearCurrentScene();
        }

        return;
    }

    Player* player = FindGO<Player>("player");

    if (player == nullptr ||
        !player->GetHealth().IsDead())
    {
        return;
    }

    // ゲームオーバー時だけR・Tを受け付ける。
    if (tTriggered)
    {
        m_nextState = State::Title;
        ClearCurrentScene();
    }
    else if (rTriggered)
    {
        m_nextState = State::Playing;
        ClearCurrentScene();
    }
}

void RetryController::ClearCurrentScene()
{
    TitleUI* title = FindGO<TitleUI>("titleUI");
    DeleteGO(title);

    Game* game = FindGO<Game>("game");

    if (game != nullptr)
    {
        game->ClearGameObjects();
        DeleteGO(game);
    }

    m_state = State::WaitingForDeletion;
}

bool RetryController::AreOldObjectsDeleted() const
{
    const char* names[] =
    {
        "titleUI",
        "game",
        "background",
        "player",
        "gameCamera",
        "shapeBox",
        "playerHUD",
        "enemy",
        "gameOverUI",
        "bullet",
        "enemyBullet"
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