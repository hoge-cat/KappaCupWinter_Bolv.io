#include "stdafx.h"
#include "PlayerHUD.h"
#include "Player.h"
#include <cwchar>
#include "InGameUI.h"
#include "Enemy.h"

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

    BolvUI::Rect(m_xpOutline, 488, 34, 0, -475, Vector4(0.18f,0.18f,0.18f,0.9f));
    BolvUI::Rect(m_xpTrack, 480, 26, 0, -475, Vector4(0.45f,0.45f,0.45f,0.8f));
    BolvUI::Rect(m_xpFill, 480, 26, -240, -475, Vector4(0.96f,0.78f,0.27f,1));
    m_xpFill.SetPivot(Vector2(0,0.5f));
    BolvUI::Rect(m_hpTrack, 204, 12, 0, -395, Vector4(0.18f,0.18f,0.18f,0.8f));
    BolvUI::Rect(m_hpFill, 196, 6, -98, -395, Vector4(0.40f,0.83f,0.36f,1));
    m_hpFill.SetPivot(Vector2(0,0.5f));
    BolvUI::Rect(m_scorePanel, 320, 100, 755, 445, Vector4(0.25f,0.25f,0.25f,0.65f));
    BolvUI::Rect(m_mapBorder, 184, 184, 825, -410, Vector4(0.20f,0.20f,0.20f,0.85f));
    BolvUI::Rect(m_mapBackground, 176, 176, 825, -410, Vector4(0.75f,0.75f,0.75f,0.65f));
    BolvUI::Rect(m_mapPlayer, 8, 8, 825, -410, Vector4(0.10f,0.72f,0.93f,1));
    for(auto& dot : m_mapEnemies) { BolvUI::Rect(dot, 5, 5, 0, 0, Vector4(0.92f,0.29f,0.34f,1)); }
    BolvUI::Center(m_scoreHeading, L"\u6226\u95d8\u30b9\u30b3\u30a2", 755, 475, 0.65f);
    BolvUI::Center(m_mapHeading, L"\u30de\u30c3\u30d7", 825, -302, 0.5f);
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
            L"\u30ec\u30d9\u30eb45\u3000\u6700\u5927\u30ec\u30d9\u30eb"
        );
    }
    else
    {
        swprintf_s(
            levelText,
            256,
            L"\u30ec\u30d9\u30eb%d\u3000\u7d4c\u9a13\u5024 %d / %d",
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
        L"\u8010\u4e45\u529b %d / %d",
        health.GetHP(),
        health.GetMaxHP()
    );

    m_hpFontRender.SetText(hpText);

    // SCORE。
    wchar_t scoreText[256];

    swprintf_s(
        scoreText,
        256,
        L"\u3042\u306a\u305f\uff1a%d",
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
    BolvUI::Center(m_fontRender, levelText, 0, -475, 0.58f);
    BolvUI::Center(m_hpFontRender, hpText, 0, -371, 0.50f);
    BolvUI::Center(m_scoreFontRender, scoreText, 755, 433, 0.80f);
    BolvUI::Center(m_classFont, BolvUI::TankName(m_player->GetEvolution().GetCurrentClass()), 0, -435, 0.80f);
}

void PlayerHUD::Render(RenderContext& rc)
{
    if (m_player == nullptr)
    {
        return;
    }

    if (m_player->GetHealth().IsDead()) { return; }
    const auto& progress = m_player->GetProgress();
    const auto& health = m_player->GetHealth();
    const float xp = progress.GetLevel() >= 45 ? 1.0f :
        BolvUI::Clamp01(static_cast<float>(progress.GetCurrentExperience()) /
            (progress.GetRequiredExperience() > 0 ? progress.GetRequiredExperience() : 1));
    const float hp = BolvUI::Clamp01(static_cast<float>(health.GetHP()) / (health.GetMaxHP() > 0 ? health.GetMaxHP() : 1));
    m_xpOutline.Draw(rc); m_xpTrack.Draw(rc);
    m_xpFill.SetScale(Vector3(xp,1,1)); m_xpFill.Update(); if(xp > 0) { m_xpFill.Draw(rc); }
    m_hpTrack.Draw(rc); m_hpFill.SetScale(Vector3(hp,1,1)); m_hpFill.Update(); if(hp > 0) { m_hpFill.Draw(rc); }
    m_scorePanel.Draw(rc); m_scoreHeading.Draw(rc); m_classFont.Draw(rc);
    m_mapBorder.Draw(rc); m_mapBackground.Draw(rc); m_mapHeading.Draw(rc);
    const auto mapPosition = [](const Vector3& p)
    {
        return Vector3(825 + (BolvUI::Clamp01((p.x + 1500) / 3000) - 0.5f) * 168,
            -410 + (BolvUI::Clamp01((p.z + 1500) / 3000) - 0.5f) * 168, 0);
    };
    int dotCount = 0;
    QueryGOs<Enemy>("enemy", [&](Enemy* enemy)
    {
        if(enemy->IsDead()) { return true; }
        if(dotCount >= 32) { return false; }
        auto& dot = m_mapEnemies[dotCount++];
        dot.SetPosition(mapPosition(enemy->GetPosition())); dot.Update(); dot.Draw(rc);
        return true;
    });
    m_mapPlayer.SetPosition(mapPosition(m_player->GetPosition())); m_mapPlayer.Update(); m_mapPlayer.Draw(rc);
    m_fontRender.Draw(rc);
    m_hpFontRender.Draw(rc);
    m_scoreFontRender.Draw(rc);
}