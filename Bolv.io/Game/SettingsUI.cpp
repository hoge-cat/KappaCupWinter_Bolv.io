#include "stdafx.h"
#include "SettingsUI.h"
#include "GameSettings.h"
#include "AudioManager.h"
#include "InGameUI.h"
#include "system/system.h"
#include <cmath>
#include <cwchar>

namespace
{
    constexpr float ROW_Y[] = { 210, 100, -10, -130, -290 };
    constexpr float SLIDER_LEFT = -50, SLIDER_WIDTH = 400;
    bool Pressed(int key) { return (GetAsyncKeyState(key) & 0x8000) != 0; }
    bool Repeat(int direction, int& last, float& timer, float deltaTime)
    {
        timer -= deltaTime;
        if (!direction) { last = 0; timer = 0; return false; }
        if (direction != last) { last = direction; timer = 0.40f; return true; }
        if (timer > 0) { return false; }
        timer = 0.16f;
        return true;
    }
}
void SettingsUI::Start()
{
    m_gear.Init("Assets/sprite/SettingsGear.dds", 76, 76);
    m_gear.SetPosition(Vector3(865, 455, 0));
    m_gear.Update();
    SetGearSelected(false);
    BolvUI::Rect(m_overlay, UI_SPACE_WIDTH, UI_SPACE_HEIGHT, 0, 0, Vector4(0,0,0,0.75f));
    BolvUI::Rect(m_panel, 1080, 800, 0, 0, Vector4(0.04f,0.07f,0.10f,1));
    BolvUI::Center(m_heading, L"設定", 0, 330, 1.1f);
    const wchar_t* labels[] = { L"マスターボリューム", L"SE音量", L"BGM音量", L"画面モード", L"閉じる" };
    for (int i = 0; i < 5; ++i)
    {
        BolvUI::Rect(m_rows[i], 960, i == 4 ? 72 : 84, 0, ROW_Y[i], Vector4(1,1,1,1));
        BolvUI::Center(m_labels[i], labels[i], i == 4 ? 0 : -300, ROW_Y[i], 0.68f);
    }
    for (int i = 0; i < 3; ++i)
    {
        BolvUI::Rect(m_tracks[i], SLIDER_WIDTH, 12, 150, ROW_Y[i], Vector4(0.16f,0.24f,0.31f,1));
        BolvUI::Rect(m_fills[i], SLIDER_WIDTH, 12, 150, ROW_Y[i], Vector4(0.08f,0.73f,0.93f,1));
        BolvUI::Rect(m_knobs[i], 20, 32, 150, ROW_Y[i], Vector4(0.85f,0.96f,1,1));
    }
    BolvUI::Center(m_hint, L"上下・W/S：選択　左右・A/D：調整　Enter：決定　Esc：閉じる　F11：画面切替", 0, -360, 0.40f);
    BolvUI::Center(m_notice, L"", 0, -225, 0.43f);
    Refresh();
}
bool SettingsUI::HitGear(float x, float y) const
{
    const float dx = x - 865, dy = y - 455;
    return dx * dx + dy * dy <= 42 * 42;
}
void SettingsUI::SetGearSelected(bool selected)
{
    m_gear.SetMulColor(selected ? Vector4(0.15f,0.82f,1,1) : Vector4(0.7f,0.8f,0.87f,1));
}
void SettingsUI::Open()
{
    m_open = true; m_selected = 0; m_drag = -1;
    m_saveFailureAcknowledged = false;
    m_blocked = true;
    m_lastNavigation = m_lastAdjustment = 0;
    m_navigationTimer = m_adjustmentTimer = 0;
    m_wasMouse = Pressed(VK_LBUTTON);
    m_wasConfirm = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    m_wasEscape = Pressed(VK_ESCAPE);
    GetCursorPos(&m_lastMouse);
    BolvUI::Center(m_notice, L"", 0, -225, 0.43f);
    Refresh();
}
int SettingsUI::HitRow(float x, float y) const
{
    if (std::fabs(x) > 480) { return -1; }
    for (int i = 0; i < 5; ++i) { if (std::fabs(y - ROW_Y[i]) <= 38) { return i; } }
    return -1;
}
void SettingsUI::Close()
{
    m_drag = -1;
    if (!GameSettings::Save() && !m_saveFailureAcknowledged)
    {
        BolvUI::Center(m_notice, L"保存できませんでした。この起動中は設定が有効です。", 0, -225, 0.43f);
        // 警告を表示し、次の閉じる操作では保存失敗時もタイトルへ戻れる。
        m_saveFailureAcknowledged = true;
        return;
    }
    m_open = false;
    AudioManager::PlaySelectSE();
}
void SettingsUI::Refresh()
{
    for (int i = 0; i < 5; ++i)
    {
        m_rows[i].SetMulColor(i == m_selected ? Vector4(0.06f,0.22f,0.30f,1) : Vector4(0.05f,0.09f,0.13f,1));
    }
    for (int i = 0; i < 3; ++i)
    {
        const int percent = GameSettings::GetVolume(i);
        const float fraction = percent / 100.0f;
        m_fills[i].SetScale(Vector3(fraction, 1, 1));
        m_fills[i].SetPosition(Vector3(SLIDER_LEFT + SLIDER_WIDTH * fraction * 0.5f, ROW_Y[i], 0));
        m_fills[i].Update();
        m_knobs[i].SetPosition(Vector3(SLIDER_LEFT + SLIDER_WIDTH * fraction, ROW_Y[i], 0));
        m_knobs[i].Update();
        wchar_t value[16]; swprintf_s(value, L"%d％", percent);
        BolvUI::Center(m_values[i], value, 430, ROW_Y[i], 0.65f);
    }
    BolvUI::Center(m_mode, GameSettings::GetFullscreen() ? L"フルスクリーン" : L"ウィンドウ", 150, ROW_Y[3], 0.68f);
}
void SettingsUI::Update()
{
    if (!m_open) { return; }
    const bool active = GetForegroundWindow() == g_hWnd;
    const bool mouse = Pressed(VK_LBUTTON);
    const bool confirm = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    const bool escape = Pressed(VK_ESCAPE);
    const bool up = Pressed(VK_UP) || Pressed('W'), down = Pressed(VK_DOWN) || Pressed('S') || Pressed(VK_TAB);
    const bool left = Pressed(VK_LEFT) || Pressed('A'), right = Pressed(VK_RIGHT) || Pressed('D');
    const float deltaTime = g_gameTime->GetFrameDeltaTime();
    if (!active) { m_blocked = true; m_drag = -1; }
    int hovered = -1;
    float x = 0, y = 0;
    POINT point = {};
    bool validMouse = false;
    if (active && GetCursorPos(&point))
    {
        const bool moved = point.x != m_lastMouse.x || point.y != m_lastMouse.y;
        m_lastMouse = point;
        RECT client;
        if (ScreenToClient(g_hWnd, &point) && GetClientRect(g_hWnd, &client) && client.right > 0 && client.bottom > 0)
        {
            x = point.x * static_cast<float>(UI_SPACE_WIDTH) / client.right - UI_SPACE_WIDTH * 0.5f;
            y = UI_SPACE_HEIGHT * 0.5f - point.y * static_cast<float>(UI_SPACE_HEIGHT) / client.bottom;
            validMouse = point.x >= 0 && point.y >= 0 && point.x < client.right && point.y < client.bottom;
            hovered = validMouse ? HitRow(x, y) : -1;
            if (moved && hovered >= 0 && m_drag < 0) { m_selected = hovered; }
        }
    }
    const bool clicked = active && mouse && !m_wasMouse;
    const bool accepted = active && confirm && !m_wasConfirm;
    if (active && ((escape && !m_wasEscape) || (accepted && m_selected == 4) || (clicked && hovered == 4)))
    {
        Close();
    }
    else if (active)
    {
        if (clicked && hovered >= 0)
        {
            m_selected = hovered;
            if (hovered < 3 && x >= SLIDER_LEFT - 12 && x <= SLIDER_LEFT + SLIDER_WIDTH + 12) { m_drag = hovered; }
        }
        if (m_drag >= 0 && mouse && validMouse)
        {
            GameSettings::SetVolume(m_drag, static_cast<int>(std::round((x - SLIDER_LEFT) / SLIDER_WIDTH * 100)));
        }
        if (!mouse && m_drag >= 0) { m_drag = -1; AudioManager::PlaySelectSE(); }
        if (m_blocked)
        {
            if (!up && !down && !left && !right) { m_blocked = false; }
        }
        else if (m_drag < 0)
        {
            if (Repeat(up == down ? 0 : (up ? -1 : 1), m_lastNavigation, m_navigationTimer, deltaTime))
            {
                m_selected = (m_selected + (up ? -1 : 1) + 5) % 5;
            }
            if (m_selected < 3 && Repeat(left == right ? 0 : (left ? -1 : 1), m_lastAdjustment, m_adjustmentTimer, deltaTime))
            {
                GameSettings::SetVolume(m_selected, GameSettings::GetVolume(m_selected) + (left ? -5 : 5));
                AudioManager::PlaySelectSE();
            }
        }
        if (m_selected == 3 && ((clicked && hovered == 3) || accepted))
        {
            if (!GameSettings::SetFullscreen(!GameSettings::GetFullscreen()))
            {
                BolvUI::Center(m_notice, L"画面モードを変更できませんでした。", 0, -225, 0.43f);
            }
            else { AudioManager::PlaySelectSE(); }
        }
        Refresh();
    }
    m_wasMouse = mouse; m_wasConfirm = confirm; m_wasEscape = escape;
}
void SettingsUI::Render(RenderContext& rc)
{
    m_gear.Draw(rc);
    if (!m_open) { return; }
    m_overlay.Draw(rc); m_panel.Draw(rc); m_heading.Draw(rc);
    for (auto& row : m_rows) { row.Draw(rc); }
    for (int i = 0; i < 3; ++i) { m_tracks[i].Draw(rc); m_fills[i].Draw(rc); m_knobs[i].Draw(rc); }
    for (auto& label : m_labels) { label.Draw(rc); }
    for (auto& value : m_values) { value.Draw(rc); }
    m_mode.Draw(rc); m_hint.Draw(rc); m_notice.Draw(rc);
}
