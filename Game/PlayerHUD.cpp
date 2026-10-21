#include "stdafx.h"
#include "PlayerHUD.h"
#include "Player.h"
#include <cwchar>

bool PlayerHUD::Start()
{
    m_player = FindGO<Player>("player");

    // Lv / EXP。
    m_fontRender.SetPosition(
        Vector3(-500.0f, 300.0f, 0.0f)
    );

    m_fontRender.SetScale(1.5f);

    m_fontRender.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    // HP。
    m_hpFontRender.SetPosition(
        Vector3(-500.0f, 250.0f, 0.0f)
    );

    m_hpFontRender.SetScale(1.5f);

    m_hpFontRender.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    // SCORE。
    m_scoreFontRender.SetPosition(
        Vector3(-500.0f, 200.0f, 0.0f)
    );

    m_scoreFontRender.SetScale(1.5f);

    m_scoreFontRender.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    // 強化ポイント。
    m_upgradePointFontRender.SetPosition(
        Vector3(-500.0f, 150.0f, 0.0f)
    );

    m_upgradePointFontRender.SetScale(1.5f);

    m_upgradePointFontRender.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    Update();

    return true;
}

void PlayerHUD::Update()
{
    if (m_player == nullptr)
    {
        return;
    }

    const PlayerProgress& progress =
        m_player->GetProgress();

    // Lv / EXP。
    wchar_t levelText[256];

    if (progress.GetLevel() >= 45)
    {
        swprintf_s(
            levelText,
            256,
            L"Lv.45 MAX"
        );
    }
    else
    {
        swprintf_s(
            levelText,
            256,
            L"Lv.%d EXP: %d / %d",
            progress.GetLevel(),
            progress.GetCurrentExperience(),
            progress.GetRequiredExperience()
        );
    }

    m_fontRender.SetText(levelText);

    // HP。
    const PlayerHealth& health =
        m_player->GetHealth();

    wchar_t hpText[256];

    swprintf_s(
        hpText,
        256,
        L"HP: %d / %d",
        health.GetHP(),
        health.GetMaxHP()
    );

    m_hpFontRender.SetText(hpText);

    // SCORE。
    wchar_t scoreText[256];

    swprintf_s(
        scoreText,
        256,
        L"SCORE: %d",
        m_player->GetScore()
    );

    m_scoreFontRender.SetText(scoreText);

    // 強化ポイント。
    wchar_t pointText[256];

    swprintf_s(
        pointText,
        256,
        L"POINTS: %d",
        progress.GetUpgradePoints()
    );

    m_upgradePointFontRender.SetText(
        pointText
    );
}

void PlayerHUD::Render(RenderContext& rc)
{
    if (m_player == nullptr)
    {
        return;
    }

    m_fontRender.Draw(rc);
    m_hpFontRender.Draw(rc);
    m_scoreFontRender.Draw(rc);
    m_upgradePointFontRender.Draw(rc);
}