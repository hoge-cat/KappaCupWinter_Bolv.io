#include "stdafx.h"
#include "ClearResultUI.h"
#include "InGameUI.h"
#include "system/system.h"
#include <cmath>
#include <cwchar>

namespace
{
    constexpr float BUTTON_Y[] = { -80, -195 };
    bool Pressed(int key) { return (GetAsyncKeyState(key) & 0x8000) != 0; }
}

bool ClearResultUI::Start()
{
    g_isCursorVisible = true;
    ClipCursor(nullptr);
    while (ShowCursor(TRUE) < 0) {}
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    BolvUI::Rect(m_background, UI_SPACE_WIDTH, UI_SPACE_HEIGHT, 0, 0, Vector4(0.025f,0.045f,0.075f,1));
    BolvUI::Rect(m_panel, 760, 720, 0, 10, Vector4(0.045f,0.075f,0.11f,1));
    BolvUI::Center(m_heading, L"\u30b2\u30fc\u30e0\u30af\u30ea\u30a2", 0, 285, 1.5f);
    m_heading.SetColor(Vector4(0.30f,0.85f,1,1));
    wchar_t summary[128];
    swprintf_s(summary, 128, L"\u30b9\u30b3\u30a2\uff1a%d\u3000\u30ec\u30d9\u30eb\uff1a%d", m_score, m_level);
    BolvUI::Center(m_summary, summary, 0, 135, 0.85f);
    const wchar_t* labels[] = { L"\u3082\u3046\u4e00\u5ea6", L"\u30bf\u30a4\u30c8\u30eb\u3078" };
    for (int i = 0; i < 2; ++i)
    {
        BolvUI::Rect(m_borders[i], 484, 86, 0, BUTTON_Y[i], Vector4(1,1,1,1));
        BolvUI::Rect(m_fills[i], 480, 82, 0, BUTTON_Y[i], Vector4(1,1,1,1));
        BolvUI::Center(m_labels[i], labels[i], 0, BUTTON_Y[i], 0.95f);
    }
    BolvUI::Center(m_hint, L"WASD\u30fb\u4e0a\u4e0b\u30ad\u30fc\uff1a\u9078\u629e\u3000Enter\uff0fSpace\uff1a\u6c7a\u5b9a\u3000\u30af\u30ea\u30c3\u30af\u53ef\u80fd\nR\uff1a\u3082\u3046\u4e00\u5ea6\u3000T\uff1a\u30bf\u30a4\u30c8\u30eb\u3078", 0, -425, 0.48f);
    m_hint.SetColor(Vector4(0.50f,0.64f,0.73f,1));
    GetCursorPos(&m_lastMouse);
    m_wasMousePressed = Pressed(VK_LBUTTON);
    m_wasEnterPressed = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    m_wasRPressed = Pressed('R');
    m_wasTPressed = Pressed('T');
    m_navigationBlocked = Pressed(VK_UP) || Pressed('W') || Pressed('A') ||
        Pressed(VK_DOWN) || Pressed('S') || Pressed('D') || Pressed(VK_TAB);
    UpdateColors();
    return true;
}

int ClearResultUI::HitTest(float x, float y) const
{
    if (std::fabs(x) > 240) { return -1; }
    for (int i = 0; i < 2; ++i) { if (std::fabs(y - BUTTON_Y[i]) <= 41) { return i; } }
    return -1;
}

void ClearResultUI::UpdateColors()
{
    for (int i = 0; i < 2; ++i)
    {
        m_borders[i].SetMulColor(i == m_selected ? Vector4(0.08f,0.73f,0.93f,1) : Vector4(0.12f,0.20f,0.28f,1));
        m_fills[i].SetMulColor(i == m_selected ? Vector4(0.04f,0.20f,0.29f,1) : Vector4(0.045f,0.075f,0.11f,1));
    }
}

void ClearResultUI::Update()
{
    const bool active = GetForegroundWindow() == g_hWnd;
    const bool mousePressed = Pressed(VK_LBUTTON);
    const bool enterPressed = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    const bool rPressed = Pressed('R'), tPressed = Pressed('T');
    POINT mouse;
    int hovered = -1;
    if (active && GetCursorPos(&mouse))
    {
        const bool moved = mouse.x != m_lastMouse.x || mouse.y != m_lastMouse.y;
        m_lastMouse = mouse;
        RECT client;
        if (ScreenToClient(g_hWnd, &mouse) && GetClientRect(g_hWnd, &client) && client.right > client.left && client.bottom > client.top)
        {
            const float x = mouse.x * static_cast<float>(UI_SPACE_WIDTH) / (client.right - client.left) - UI_SPACE_WIDTH * 0.5f;
            const float y = UI_SPACE_HEIGHT * 0.5f - mouse.y * static_cast<float>(UI_SPACE_HEIGHT) / (client.bottom - client.top);
            hovered = HitTest(x, y);
            if (moved && hovered != -1) { m_selected = hovered; }
        }
    }
    UpdateNavigation(Pressed(VK_UP) || Pressed('W') || Pressed('A'),
        Pressed(VK_DOWN) || Pressed('S') || Pressed('D') || Pressed(VK_TAB), active, g_gameTime->GetFrameDeltaTime());
    const bool clicked = active && mousePressed && !m_wasMousePressed && hovered != -1;
    const bool confirmed = active && enterPressed && !m_wasEnterPressed;
    if (clicked) { m_selected = hovered; }
    if (m_action == Action::None && active)
    {
        if (tPressed && !m_wasTPressed) { m_action = Action::Title; }
        else if (rPressed && !m_wasRPressed) { m_action = Action::Retry; }
        else if (clicked || confirmed) { m_action = m_selected == 0 ? Action::Retry : Action::Title; }
    }
    m_wasMousePressed = mousePressed;
    m_wasEnterPressed = enterPressed;
    m_wasRPressed = rPressed;
    m_wasTPressed = tPressed;
    UpdateColors();
}

ClearResultUI::Action ClearResultUI::ConsumeAction()
{
    const Action action = m_action;
    m_action = Action::None;
    return action;
}

void ClearResultUI::Render(RenderContext& rc)
{
    m_background.Draw(rc); m_panel.Draw(rc);
    m_heading.Draw(rc); m_summary.Draw(rc);
    for (int i = 0; i < 2; ++i) { m_borders[i].Draw(rc); m_fills[i].Draw(rc); m_labels[i].Draw(rc); }
    m_hint.Draw(rc);
}
void ClearResultUI::UpdateNavigation(bool up, bool down, bool active, float deltaTime)
{
    constexpr float INPUT_INTERVAL = 0.20f;
    constexpr float FIRST_REPEAT_DELAY = 0.40f;
    m_navigationCooldown -= deltaTime;
    if (m_navigationCooldown < 0.0f) { m_navigationCooldown = 0.0f; }
    if (!active)
    {
        m_navigationBlocked = true;
    }
    if (m_navigationBlocked)
    {
        if (active && !up && !down) { m_navigationBlocked = false; }
        m_navigationDirection = 0;
        return;
    }
    const int direction = up == down ? 0 : (up ? -1 : 1);
    if (direction == 0)
    {
        m_navigationDirection = 0;
        return;
    }
    bool navigate = false;
    if (direction != m_navigationDirection)
    {
        m_navigationDirection = direction;
        m_navigationRepeat = FIRST_REPEAT_DELAY;
        navigate = m_navigationCooldown <= 0.000001f;
    }
    else
    {
        m_navigationRepeat -= deltaTime;
        navigate = m_navigationRepeat <= 0.000001f && m_navigationCooldown <= 0.000001f;
    }
    if (navigate)
    {
        // Share the cooldown across keys and directions; never catch up in a loop.
        m_selected = 1 - m_selected;
        m_navigationCooldown = INPUT_INTERVAL;
        if (m_navigationRepeat <= 0.000001f) { m_navigationRepeat = INPUT_INTERVAL; }
    }
}


