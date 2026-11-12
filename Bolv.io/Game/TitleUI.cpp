#include "stdafx.h"
#include "TitleUI.h"
#include "system/system.h"
#include <cmath>

namespace
{
    constexpr float BUTTON_WIDTH = 480.0f;
    constexpr float BUTTON_HEIGHT = 82.0f;
    constexpr float BUTTON_Y[] = { 40.0f, -75.0f, -300.0f };
    bool Pressed(int key) { return (GetAsyncKeyState(key) & 0x8000) != 0; }

    void PlaceSprite(SpriteRender& sprite, float y)
    {
        // Render2D uses a fixed UI target; the engine scales the completed image.
        sprite.SetScale(Vector3(1.0f, 1.0f, 1.0f));
        sprite.SetPosition(Vector3(0.0f, y, 0.0f));
    }

    void CenterText(FontRender& font, const wchar_t* text, float scale, float y)
    {
        const Vector2 size = g_graphicsEngine->GetFontEngine().MeasureText(text);
        font.SetText(text);
        font.SetScale(scale);
        font.SetPosition(Vector3(0.0f, y, 0.0f));
        // SpriteFont uses a pixel origin; Font::Draw flips the Y origin.
        font.SetPivot(size.x * 0.5f, 1.0f - size.y * 0.5f);
        font.SetShadowParam(false, 0.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    }
}

bool TitleUI::Start()
{
    g_isCursorVisible = true;
    ClipCursor(nullptr);
    while (ShowCursor(TRUE) < 0) {}
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    m_background.Init("Assets/sprite/White.dds", UI_SPACE_WIDTH, UI_SPACE_HEIGHT);
    m_background.SetMulColor(Vector4(0.025f, 0.045f, 0.075f, 1.0f));
    PlaceSprite(m_background, 0.0f);
    m_background.Update();
    m_logo.Init("Assets/sprite/BolvLogo.DDS", 880.0f, 880.0f * 793.0f / 1983.0f);
    PlaceSprite(m_logo, 315.0f);
    m_logo.Update();
    CenterText(m_subtitleFont, L"3D\u6226\u8eca\u30b7\u30e5\u30fc\u30c6\u30a3\u30f3\u30b0", 0.8f, 180.0f);
    m_subtitleFont.SetColor(Vector4(0.48f, 0.62f, 0.73f, 1.0f));
    const wchar_t* labels[] = {
        L"\u30b2\u30fc\u30e0\u30b9\u30bf\u30fc\u30c8",
        L"\u6226\u8eca\u56f3\u9451",
        L"\u30b2\u30fc\u30e0\u7d42\u4e86"
    };
    for (int i = 0; i < 3; ++i)
    {
        m_buttonBorders[i].Init("Assets/sprite/White.dds", BUTTON_WIDTH + 4.0f, BUTTON_HEIGHT + 4.0f);
        PlaceSprite(m_buttonBorders[i], BUTTON_Y[i]);
        m_buttonBorders[i].Update();
        m_buttonFills[i].Init("Assets/sprite/White.dds", BUTTON_WIDTH, BUTTON_HEIGHT);
        PlaceSprite(m_buttonFills[i], BUTTON_Y[i]);
        m_buttonFills[i].Update();
        CenterText(m_buttonLabels[i], labels[i], 1.0f, BUTTON_Y[i] + (i == 1 ? 12.0f : 0.0f));
    }
    CenterText(m_comingSoonFont, L"\u6e96\u5099\u4e2d", 0.55f, BUTTON_Y[1] - 22.0f);
    m_comingSoonFont.SetColor(Vector4(0.36f, 0.44f, 0.52f, 1.0f));
    CenterText(m_hintFont, L"WASD\u30fb\u4e0a\u4e0b\u30ad\u30fc\u30fbTab\uff1a\u9078\u629e\u3000Enter\uff0fSpace\uff1a\u6c7a\u5b9a\u3000\u5de6\u30af\u30ea\u30c3\u30af\u3067\u3082\u64cd\u4f5c\u53ef\u80fd", 0.55f, -465.0f);
    m_hintFont.SetColor(Vector4(0.35f, 0.46f, 0.57f, 1.0f));
    GetCursorPos(&m_lastMouse);
    m_wasMousePressed = Pressed(VK_LBUTTON);
    m_wasEnterPressed = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    m_navigationBlocked = Pressed(VK_UP) || Pressed('W') || Pressed('A') ||
        Pressed(VK_DOWN) || Pressed(VK_TAB) || Pressed('S') || Pressed('D');
    UpdateColors();
    return true;
}

int TitleUI::HitTest(float x, float y) const
{
    if (std::fabs(x) > BUTTON_WIDTH * 0.5f) { return -1; }
    for (int i = 0; i < 3; ++i)
    {
        if (std::fabs(y - BUTTON_Y[i]) <= BUTTON_HEIGHT * 0.5f) { return i; }
    }
    return -1;
}

void TitleUI::UpdateColors()
{
    for (int i = 0; i < 3; ++i)
    {
        const bool selected = i == m_selected;
        m_buttonBorders[i].SetMulColor(selected ? Vector4(0.08f, 0.73f, 0.93f, 1.0f) : Vector4(0.12f, 0.20f, 0.28f, 1.0f));
        m_buttonFills[i].SetMulColor(selected ? Vector4(0.04f, 0.20f, 0.29f, 1.0f) : Vector4(0.045f, 0.075f, 0.11f, 1.0f));
        m_buttonLabels[i].SetColor(i == 1 ? Vector4(0.39f, 0.47f, 0.55f, 1.0f) : Vector4(0.90f, 0.96f, 1.0f, 1.0f));
    }
}

void TitleUI::UpdateNavigation(bool up, bool down, bool active, float deltaTime)
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
        m_selected = m_selected == 0 ? 2 : 0;
        m_navigationCooldown = INPUT_INTERVAL;
        if (m_navigationRepeat <= 0.000001f) { m_navigationRepeat = INPUT_INTERVAL; }
    }
}

void TitleUI::Update()
{
    const bool active = GetForegroundWindow() == g_hWnd;
    const bool mousePressed = Pressed(VK_LBUTTON);
    const bool enterPressed = Pressed(VK_RETURN) || Pressed(VK_SPACE);
    const bool upPressed = Pressed(VK_UP) || Pressed('W') || Pressed('A');
    const bool downPressed = Pressed(VK_DOWN) || Pressed(VK_TAB) || Pressed('S') || Pressed('D');
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
            if (moved && hovered != -1 && hovered != 1) { m_selected = hovered; }
        }
    }
    UpdateNavigation(upPressed, downPressed, active, g_gameTime->GetFrameDeltaTime());
    const bool clicked = active && mousePressed && !m_wasMousePressed && hovered != -1 && hovered != 1;
    const bool confirmed = active && enterPressed && !m_wasEnterPressed;
    if (clicked) { m_selected = hovered; }
    if (m_action == Action::None && (clicked || confirmed))
    {
        m_action = m_selected == 0 ? Action::StartGame : Action::ExitGame;
    }
    m_wasMousePressed = mousePressed;
    m_wasEnterPressed = enterPressed;
    UpdateColors();
}

TitleUI::Action TitleUI::ConsumeAction()
{
    const Action action = m_action;
    m_action = Action::None;
    return action;
}

void TitleUI::Render(RenderContext& rc)
{
    m_background.Draw(rc);
    for (int i = 0; i < 3; ++i)
    {
        m_buttonBorders[i].Draw(rc);
        m_buttonFills[i].Draw(rc);
    }
    m_logo.Draw(rc);
    m_subtitleFont.Draw(rc);
    for (auto& label : m_buttonLabels) { label.Draw(rc); }
    m_comingSoonFont.Draw(rc);
    m_hintFont.Draw(rc);
}