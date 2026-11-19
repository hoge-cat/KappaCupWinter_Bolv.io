#include "stdafx.h"
#include "PlayerUpgradeUI.h"
#include "Player.h"
#include "PlayerStats.h"

#include <cwchar>
#include "InGameUI.h"
#include "Game.h"
#include "system/system.h"
#include <cmath>


PlayerUpgradeUI* PlayerUpgradeUI::s_inputTarget = nullptr;

namespace
{
    constexpr float ROW_Y = -220.0f;
    constexpr float ROW_SPACING = 34.0f;
    constexpr float PLUS_X = -566.0f;

    bool ClientToUpgradeSpace(int clientX, int clientY, float& x, float& y)
    {
        RECT client;
        if (!GetClientRect(g_hWnd, &client) || client.right <= client.left || client.bottom <= client.top) { return false; }
        x = clientX * static_cast<float>(UI_SPACE_WIDTH) / (client.right - client.left) - UI_SPACE_WIDTH * 0.5f;
        y = UI_SPACE_HEIGHT * 0.5f - clientY * static_cast<float>(UI_SPACE_HEIGHT) / (client.bottom - client.top);
        return true;
    }

    bool UpgradePointer(float& x, float& y)
    {
        POINT mouse;
        return GetCursorPos(&mouse) && ScreenToClient(g_hWnd, &mouse) &&
            ClientToUpgradeSpace(mouse.x, mouse.y, x, y);
    }
}

PlayerUpgradeUI::~PlayerUpgradeUI()
{
    if (s_inputTarget == this) { s_inputTarget = nullptr; }
}

bool PlayerUpgradeUI::CanAcceptInput() const
{
    const auto* game = FindGO<Game>("game");
    return GetForegroundWindow() == g_hWnd && m_player && !m_player->GetHealth().IsDead() && game && game->IsReady() && !game->IsPaused();
}

int PlayerUpgradeUI::GetRowCount() const
{
    return m_player && m_player->GetEvolution().GetCurrentClass() == PlayerEvolution::Smasher ? 4 : STAT_COUNT;
}

PlayerStats::StatType PlayerUpgradeUI::GetStatType(int row) const
{
    const PlayerStats::StatType types[] = { PlayerStats::HealthRegen, PlayerStats::MaxHealth,
        PlayerStats::BodyDamage, PlayerStats::BulletSpeed, PlayerStats::BulletPenetration,
        PlayerStats::BulletDamage, PlayerStats::Reload, PlayerStats::MovementSpeed };
    return GetRowCount() == 4 && row == 3 ? PlayerStats::MovementSpeed : types[row];
}

bool PlayerUpgradeUI::CanUpgradeRow(int row) const
{
    if (!m_player || row < 0 || row >= GetRowCount() || m_player->GetHealth().IsDead() ||
        m_player->GetProgress().GetUpgradePoints() <= 0) { return false; }
    const auto type = GetStatType(row);
    return m_player->GetStats().GetLevel(type) < m_player->GetStats().GetMaxLevel(type, GetRowCount() == 4);
}

int PlayerUpgradeUI::HitTest(float x, float y, bool plusOnly) const
{
    const bool outsideX = plusOnly ? std::fabs(x - PLUS_X) > 14.0f : (x < -912.0f || x > -548.0f);
    if (outsideX) { return -1; }
    const float halfHeight = plusOnly ? 12.0f : 14.0f;
    for (int i = 0; i < GetRowCount(); ++i)
    {
        if (std::fabs(y - (ROW_Y - i * ROW_SPACING)) <= halfHeight) { return i; }
    }
    return -1;
}

void PlayerUpgradeUI::QueueUpgrade(int row)
{
    if (row >= 0 && row < GetRowCount() && m_requestCount < static_cast<int>(m_requests.size()))
    {
        // 行番号ではなく強化対象を保存し、途中で進化しても別の能力へ振り替わらないようにする。
        m_requests[m_requestCount++] = GetStatType(row);
    }
}

void PlayerUpgradeUI::HandleWindowMessage(UINT message, WPARAM key, LPARAM parameter)
{
    auto* ui = s_inputTarget;
    if (!ui) { return; }
    if (message == WM_KILLFOCUS || (message == WM_ACTIVATEAPP && !key))
    {
        ui->m_requestCount = 0;
        ui->m_clickCaptured = false;
        return;
    }
    if (message == WM_LBUTTONUP) { ui->m_clickCaptured = false; return; }
    if (message != WM_KEYDOWN && message != WM_LBUTTONDOWN) { return; }
    if (!ui->CanAcceptInput()) { return; }
    if (message == WM_KEYDOWN && (parameter & (static_cast<LPARAM>(1) << 30)) == 0)
    {
        // 最下位ビットの押下履歴に依存せず、キー押下の通知をフレーム更新まで保持する。
        if (key >= '1' && key <= '8') { ui->QueueUpgrade(static_cast<int>(key - '1')); }
        else if (key >= VK_NUMPAD1 && key <= VK_NUMPAD8) { ui->QueueUpgrade(static_cast<int>(key - VK_NUMPAD1)); }
    }
    if (message == WM_LBUTTONDOWN && g_isCursorVisible)
    {
        float x, y;
        const int clientX = static_cast<short>(LOWORD(parameter));
        const int clientY = static_cast<short>(HIWORD(parameter));
        if (ClientToUpgradeSpace(clientX, clientY, x, y))
        {
            ui->m_clickCaptured = ui->HitTest(x, y, false) >= 0;
            const int row = ui->HitTest(x, y, true);
            if (ui->CanUpgradeRow(row)) { ui->QueueUpgrade(row); }
        }
    }
}

bool PlayerUpgradeUI::PointerBlocksShooting()
{
    auto* ui = s_inputTarget;
    if (!ui || !ui->CanAcceptInput()) { return false; }
    if (ui->m_clickCaptured)
    {
        // ウィンドウ外で離してボタンアップ通知が届かなくても、射撃抑制を残さない。
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) { return true; }
        ui->m_clickCaptured = false;
    }
    float x, y;
    return g_isCursorVisible && UpgradePointer(x, y) && ui->HitTest(x, y, false) >= 0;
}

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
        BolvUI::Rect(m_plusBackground[i], 28, 24, PLUS_X, y, Vector4(0.12f,0.16f,0.20f,1));
        BolvUI::Center(m_plusFont[i], L"＋", PLUS_X, y, 0.52f);
    }
    s_inputTarget = this;
    UpdateText();


    return true;
}


// ============================================================
// Update
// ============================================================

void PlayerUpgradeUI::Update()
{
    if (CanAcceptInput())
    {
        for (int i = 0; i < m_requestCount; ++i)
        {
            // 数字キー・クリックともに既存の成功判定、ポイント消費、強化SEを利用する。
            m_player->UpgradeStat(m_requests[i]);
        }
    }
    m_requestCount = 0;
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
            L"[1] 自動回復  %d / 10",
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
            L"[2] 最大耐久力  %d / 10",
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
            L"[3] 体当たり威力  %d / 10",
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
            L"[4] 移動速度  %d / 10",
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
            L"[1] 自動回復  %d / 7",
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
            L"[2] 最大耐久力  %d / 7",
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
            L"[3] 体当たり威力  %d / 7",
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
            L"[4] 弾速  %d / 7",
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
            L"[5] 弾の貫通力  %d / 7",
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
            L"[6] 弾の威力  %d / 7",
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
            L"[7] 連射速度  %d / 7",
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
            L"[8] 移動速度  %d / 7",
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
        L"強化ポイント：%d",
        progress.GetUpgradePoints()
    );


    m_pointFontRender
        .SetText(
            text
        );
    for (int i = 0; i < STAT_COUNT; ++i)
    {
        BolvUI::Center(m_statFontRender[i], m_statFontRender[i].GetText(), -748, ROW_Y - i * ROW_SPACING, 0.54f);
    }
    BolvUI::Center(m_pointFontRender, m_pointFontRender.GetText(), -730, -164, 0.60f);
    const wchar_t* hint = isSmasher ? L"数字1～4／テンキー・右クリック長押し＋［＋］で強化" :
        L"数字1～8／テンキー・右クリック長押し＋［＋］で強化";
    BolvUI::Center(m_hintFontRender, hint, -730, -193, 0.32f);
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
    float mouseX = 0, mouseY = 0;
    const int hovered = g_isCursorVisible && UpgradePointer(mouseX, mouseY) ? HitTest(mouseX, mouseY, true) : -1;
    for (int i = 0; i < (smasher ? 4 : STAT_COUNT); ++i)
    {
        const auto type = GetStatType(i);
        const int maximum = m_player->GetStats().GetMaxLevel(type, smasher);
        const float ratio = BolvUI::Clamp01(static_cast<float>(m_player->GetStats().GetLevel(type)) / (maximum > 0 ? maximum : 1));
        m_statBackground[i].Draw(rc);
        m_statFill[i].SetMulColor(BolvUI::StatColor(smasher && i == 3 ? 7 : i));
        m_statFill[i].SetScale(Vector3(ratio,1,1)); m_statFill[i].Update();
        if (ratio > 0) { m_statFill[i].Draw(rc); }
        const bool enabled = CanUpgradeRow(i);
        m_plusBackground[i].SetMulColor(enabled ? (hovered == i ? Vector4(0.12f,0.65f,0.82f,1) : Vector4(0.12f,0.32f,0.42f,1)) : Vector4(0.16f,0.18f,0.20f,1));
        m_plusFont[i].SetColor(enabled ? Vector4(1,1,1,1) : Vector4(0.42f,0.44f,0.46f,1));
        m_plusBackground[i].Draw(rc);
        m_plusFont[i].Draw(rc);
    }
    m_pointFontRender.Draw(
        rc
    );
    m_hintFontRender.Draw(rc);


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