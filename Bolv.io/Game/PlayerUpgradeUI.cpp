#include "stdafx.h"
#include "PlayerUpgradeUI.h"
#include "Player.h"
#include "PlayerStats.h"

#include <cwchar>
#include "InGameUI.h"


bool PlayerUpgradeUI::Start()
{
    m_player =
        FindGO<Player>(
            "player"
        );


    // ============================================
    // 強化項目
    // ============================================

    const float startY =
        -210.0f;


    const float intervalY =
        34.0f;


    for (int i = 0;
        i < STAT_COUNT;
        ++i)
    {
        m_statFontRender[i]
            .SetPosition(
                Vector3(
                    -914.0f,
                    startY -
                    intervalY *
                    i,
                    0.0f
                )
            );


        m_statFontRender[i]
            .SetScale(
                0.58f
            );


        m_statFontRender[i]
            .SetColor(
                {
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                }
            );
    }


    // ============================================
    // 残りポイント
    // ============================================

    m_pointFontRender
        .SetPosition(
            Vector3(
                -914.0f,
                -173.0f,
                0.0f
            )
        );


    m_pointFontRender
        .SetScale(
            0.64f
        );


    m_pointFontRender
        .SetColor(
            {
                1.0f,
                1.0f,
                0.0f,
                1.0f
            }
        );


    for (int i = 0; i < STAT_COUNT; ++i)
    {
        const float y = -220.0f - i * 34.0f;
        BolvUI::Rect(m_statBackground[i], 364, 28, -730, y, Vector4(0.20f,0.20f,0.20f,0.80f));
        BolvUI::Rect(m_statFill[i], 356, 22, -908, y, BolvUI::StatColor(i));
        m_statFill[i].SetPivot(Vector2(0,0.5f));
    }
    UpdateText();


    return true;
}


// ============================================================
// Update
// ============================================================

void PlayerUpgradeUI::Update()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    const bool isSmasher =
        m_player
        ->GetEvolution()
        .GetCurrentClass() ==
        PlayerEvolution::Smasher;


    // ========================================================
    // Smasher
    //
    // 1 HP Regen
    // 2 Max Health
    // 3 Body Damage
    // 4 Movement Speed
    // ========================================================

    if (isSmasher)
    {
        if (GetAsyncKeyState(
            '1') &
            1)
        {
            m_player
                ->UpgradeStat(
                    PlayerStats::HealthRegen
                );
        }


        if (GetAsyncKeyState(
            '2') &
            1)
        {
            m_player
                ->UpgradeStat(
                    PlayerStats::MaxHealth
                );
        }


        if (GetAsyncKeyState(
            '3') &
            1)
        {
            m_player
                ->UpgradeStat(
                    PlayerStats::BodyDamage
                );
        }


        if (GetAsyncKeyState(
            '4') &
            1)
        {
            m_player
                ->UpgradeStat(
                    PlayerStats::MovementSpeed
                );
        }


        UpdateText();


        return;
    }


    // ========================================================
    // 通常戦車
    // ========================================================

    // 1キー
    if (GetAsyncKeyState(
        '1') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::HealthRegen
            );
    }


    // 2キー
    if (GetAsyncKeyState(
        '2') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::MaxHealth
            );
    }


    // 3キー
    if (GetAsyncKeyState(
        '3') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::BodyDamage
            );
    }


    // 4キー
    if (GetAsyncKeyState(
        '4') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::BulletSpeed
            );
    }


    // 5キー
    if (GetAsyncKeyState(
        '5') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::BulletPenetration
            );
    }


    // 6キー
    if (GetAsyncKeyState(
        '6') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::BulletDamage
            );
    }


    // 7キー
    if (GetAsyncKeyState(
        '7') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::Reload
            );
    }


    // 8キー
    if (GetAsyncKeyState(
        '8') &
        1)
    {
        m_player
            ->UpgradeStat(
                PlayerStats::MovementSpeed
            );
    }


    UpdateText();
}


// ============================================================
// UpdateText
// ============================================================

void PlayerUpgradeUI::UpdateText()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    const PlayerStats& stats =
        m_player
        ->GetStats();


    const PlayerProgress& progress =
        m_player
        ->GetProgress();


    const bool isSmasher =
        m_player
        ->GetEvolution()
        .GetCurrentClass() ==
        PlayerEvolution::Smasher;


    wchar_t text[256];


    // ========================================================
    // Smasher
    // ========================================================

    if (isSmasher)
    {
        // ========================================
        // 1 HP Regen
        // ========================================

        swprintf_s(
            text,
            256,
            L"[1] \u81ea\u52d5\u56de\u5fa9  %d / 10",
            stats.GetLevel(
                PlayerStats::HealthRegen
            )
        );


        m_statFontRender[0]
            .SetText(
                text
            );


        // ========================================
        // 2 Max Health
        // ========================================

        swprintf_s(
            text,
            256,
            L"[2] \u6700\u5927\u8010\u4e45\u529b  %d / 10",
            stats.GetLevel(
                PlayerStats::MaxHealth
            )
        );


        m_statFontRender[1]
            .SetText(
                text
            );


        // ========================================
        // 3 Body Damage
        // ========================================

        swprintf_s(
            text,
            256,
            L"[3] \u4f53\u5f53\u305f\u308a\u5a01\u529b  %d / 10",
            stats.GetLevel(
                PlayerStats::BodyDamage
            )
        );


        m_statFontRender[2]
            .SetText(
                text
            );


        // ========================================
        // 4 Movement Speed
        // ========================================

        swprintf_s(
            text,
            256,
            L"[4] \u79fb\u52d5\u901f\u5ea6  %d / 10",
            stats.GetLevel(
                PlayerStats::MovementSpeed
            )
        );


        m_statFontRender[3]
            .SetText(
                text
            );


        // ========================================
        // 残り4行は非表示
        // ========================================

        m_statFontRender[4]
            .SetText(
                L""
            );


        m_statFontRender[5]
            .SetText(
                L""
            );


        m_statFontRender[6]
            .SetText(
                L""
            );


        m_statFontRender[7]
            .SetText(
                L""
            );
    }


    // ========================================================
    // 通常戦車
    // ========================================================

    else
    {
        // ========================================
        // 1 HP Regen
        // ========================================

        swprintf_s(
            text,
            256,
            L"[1] \u81ea\u52d5\u56de\u5fa9  %d / 7",
            stats.GetLevel(
                PlayerStats::HealthRegen
            )
        );


        m_statFontRender[0]
            .SetText(
                text
            );


        // ========================================
        // 2 Max Health
        // ========================================

        swprintf_s(
            text,
            256,
            L"[2] \u6700\u5927\u8010\u4e45\u529b  %d / 7",
            stats.GetLevel(
                PlayerStats::MaxHealth
            )
        );


        m_statFontRender[1]
            .SetText(
                text
            );


        // ========================================
        // 3 Body Damage
        // ========================================

        swprintf_s(
            text,
            256,
            L"[3] \u4f53\u5f53\u305f\u308a\u5a01\u529b  %d / 7",
            stats.GetLevel(
                PlayerStats::BodyDamage
            )
        );


        m_statFontRender[2]
            .SetText(
                text
            );


        // ========================================
        // 4 Bullet Speed
        // ========================================

        swprintf_s(
            text,
            256,
            L"[4] \u5f3e\u901f  %d / 7",
            stats.GetLevel(
                PlayerStats::BulletSpeed
            )
        );


        m_statFontRender[3]
            .SetText(
                text
            );


        // ========================================
        // 5 Penetration
        // ========================================

        swprintf_s(
            text,
            256,
            L"[5] \u5f3e\u306e\u8cab\u901a\u529b  %d / 7",
            stats.GetLevel(
                PlayerStats::BulletPenetration
            )
        );


        m_statFontRender[4]
            .SetText(
                text
            );


        // ========================================
        // 6 Bullet Damage
        // ========================================

        swprintf_s(
            text,
            256,
            L"[6] \u5f3e\u306e\u5a01\u529b  %d / 7",
            stats.GetLevel(
                PlayerStats::BulletDamage
            )
        );


        m_statFontRender[5]
            .SetText(
                text
            );


        // ========================================
        // 7 Reload
        // ========================================

        swprintf_s(
            text,
            256,
            L"[7] \u9023\u5c04\u901f\u5ea6  %d / 7",
            stats.GetLevel(
                PlayerStats::Reload
            )
        );


        m_statFontRender[6]
            .SetText(
                text
            );


        // ========================================
        // 8 Movement Speed
        // ========================================

        swprintf_s(
            text,
            256,
            L"[8] \u79fb\u52d5\u901f\u5ea6  %d / 7",
            stats.GetLevel(
                PlayerStats::MovementSpeed
            )
        );


        m_statFontRender[7]
            .SetText(
                text
            );
    }


    // ============================================
    // 残りポイント
    // ============================================

    swprintf_s(
        text,
        256,
        L"\u5f37\u5316\u30dd\u30a4\u30f3\u30c8\uff1a%d\u3000\u6570\u5b57\u30ad\u30fc\u3067\u5f37\u5316",
        progress.GetUpgradePoints()
    );


    m_pointFontRender
        .SetText(
            text
        );
    for (int i = 0; i < STAT_COUNT; ++i)
    {
        BolvUI::Center(m_statFontRender[i], m_statFontRender[i].GetText(), -730, -220.0f - i * 34, 0.58f);
    }
    BolvUI::Center(m_pointFontRender, m_pointFontRender.GetText(), -730, -173, 0.48f);
}


// ============================================================
// Render
// ============================================================

void PlayerUpgradeUI::Render(
    RenderContext& rc)
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    if (m_player->GetHealth().IsDead()) { return; }
    const bool smasher = m_player->GetEvolution().GetCurrentClass() == PlayerEvolution::Smasher;
    const PlayerStats::StatType types[] = { PlayerStats::HealthRegen, PlayerStats::MaxHealth,
        PlayerStats::BodyDamage, PlayerStats::BulletSpeed, PlayerStats::BulletPenetration,
        PlayerStats::BulletDamage, PlayerStats::Reload, PlayerStats::MovementSpeed };
    for (int i = 0; i < (smasher ? 4 : STAT_COUNT); ++i)
    {
        const auto type = smasher && i == 3 ? PlayerStats::MovementSpeed : types[i];
        const int maximum = m_player->GetStats().GetMaxLevel(type, smasher);
        const float ratio = BolvUI::Clamp01(static_cast<float>(m_player->GetStats().GetLevel(type)) / (maximum > 0 ? maximum : 1));
        m_statBackground[i].Draw(rc);
        m_statFill[i].SetMulColor(BolvUI::StatColor(smasher && i == 3 ? 7 : i));
        m_statFill[i].SetScale(Vector3(ratio,1,1)); m_statFill[i].Update();
        if (ratio > 0) { m_statFill[i].Draw(rc); }
    }
    m_pointFontRender.Draw(
        rc
    );


    for (int i = 0;
        i < STAT_COUNT;
        ++i)
    {
        m_statFontRender[i]
            .Draw(
                rc
            );
    }
}