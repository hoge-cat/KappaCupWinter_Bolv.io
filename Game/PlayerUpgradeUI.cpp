#include "stdafx.h"
#include "PlayerUpgradeUI.h"
#include "Player.h"
#include "PlayerStats.h"

#include <cwchar>


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
        -60.0f;


    const float intervalY =
        35.0f;


    for (int i = 0;
        i < STAT_COUNT;
        ++i)
    {
        m_statFontRender[i]
            .SetPosition(
                Vector3(
                    -620.0f,
                    startY -
                    intervalY *
                    i,
                    0.0f
                )
            );


        m_statFontRender[i]
            .SetScale(
                0.95f
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
                -620.0f,
                -20.0f,
                0.0f
            )
        );


    m_pointFontRender
        .SetScale(
            1.1f
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
            L"[1] HP Regen       %d / 10",
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
            L"[2] Max Health     %d / 10",
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
            L"[3] Body Damage    %d / 10",
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
            L"[4] Move Speed     %d / 10",
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
            L"[1] HP Regen       %d / 7",
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
            L"[2] Max Health     %d / 7",
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
            L"[3] Body Damage    %d / 7",
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
            L"[4] Bullet Speed   %d / 7",
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
            L"[5] Penetration    %d / 7",
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
            L"[6] Bullet Damage  %d / 7",
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
            L"[7] Reload         %d / 7",
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
            L"[8] Move Speed     %d / 7",
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
        L"Upgrade Points: %d",
        progress.GetUpgradePoints()
    );


    m_pointFontRender
        .SetText(
            text
        );
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