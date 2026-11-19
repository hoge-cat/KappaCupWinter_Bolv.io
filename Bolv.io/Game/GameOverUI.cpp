#include "stdafx.h"
#include "GameOverUI.h"
#include "Player.h"
#include "AudioManager.h"
#include <cwchar>
#include "InGameUI.h"

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

    BolvUI::Rect(m_panelBorder, 784, 244, 0, 0, Vector4(0.18f,0.18f,0.18f,0.9f));
    BolvUI::Rect(m_panel, 776, 236, 0, 0, Vector4(0.62f,0.62f,0.62f,0.95f));
    BolvUI::Center(m_fontRender, L"ゲームオーバー", 0, 55, 1.25f);

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
        L"スコア：%d　レベル：%d　[R] 再挑戦　[T] タイトルへ",
        m_player->GetScore(),
        progress.GetLevel()
    );

    m_resultFontRender.SetText(text);
    BolvUI::Center(m_resultFontRender, text, 0, -38, 0.60f);
    const Vector2 size = g_graphicsEngine->GetFontEngine().MeasureText(text);
    m_resultFontRender.SetPivot(size.x * 0.5f, 1.0f - size.y * 0.5f);
    m_resultFontRender.SetPosition(Vector3(0.0f, -38.0f, 0.0f));

    // ゲーム終了時の結果を一度だけ記録する。
    m_resultFixed = true;

    if (auto* audio = FindGO<AudioManager>("audioManager"))
    {
        audio->PlayGameOverSE();
    }
}

void GameOverUI::Render(RenderContext& rc)
{
    if (!m_resultFixed)
    {
        return;
    }

    m_panelBorder.Draw(rc);
    m_panel.Draw(rc);
    m_fontRender.Draw(rc);
    m_resultFontRender.Draw(rc);
}