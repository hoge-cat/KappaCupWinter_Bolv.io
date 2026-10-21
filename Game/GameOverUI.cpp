#include "stdafx.h"
#include "GameOverUI.h"
#include "Player.h"
#include <cwchar>

bool GameOverUI::Start()
{
    m_player = FindGO<Player>("player");

    m_fontRender.SetText(L"GAME OVER");
    m_fontRender.SetPosition(
        Vector3(-250.0f, 0.0f, 0.0f)
    );
    m_fontRender.SetScale(3.0f);
    m_fontRender.SetColor(
        { 1.0f, 0.2f, 0.2f, 1.0f }
    );

    m_resultFontRender.SetPosition(
        Vector3(-400.0f, -130.0f, 0.0f)
    );
    m_resultFontRender.SetScale(1.2f);
    m_resultFontRender.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    return true;
}

void GameOverUI::Update()
{
    if (m_player == nullptr ||
        !m_player->GetHealth().IsDead() ||
        m_resultFixed)
    {
        return;
    }

    const PlayerProgress& progress =
        m_player->GetProgress();

    wchar_t text[256];

    swprintf_s(
        text,
        256,
        L"SCORE: %d   LEVEL: %d   [R] RETRY   [T] TITLE",
        m_player->GetScore(),
        progress.GetLevel()
    );

    m_resultFontRender.SetText(text);

    // ゲーム終了時の結果を一度だけ記録する。
    m_resultFixed = true;
}

void GameOverUI::Render(RenderContext& rc)
{
    if (!m_resultFixed)
    {
        return;
    }

    m_fontRender.Draw(rc);
    m_resultFontRender.Draw(rc);
}