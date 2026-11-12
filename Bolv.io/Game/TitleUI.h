#pragma once
#include <array>

class TitleUI : public IGameObject
{
public:
    enum class Action { None, StartGame, ExitGame };
    bool Start();
    void Update();
    void Render(RenderContext& rc);
    Action ConsumeAction();

private:
    int HitTest(float x, float y) const;
    void UpdateColors();
    void UpdateNavigation(bool up, bool down, bool active, float deltaTime);
    SpriteRender m_background;
    std::array<SpriteRender, 3> m_buttonBorders;
    std::array<SpriteRender, 3> m_buttonFills;
    std::array<FontRender, 3> m_buttonLabels;
    SpriteRender m_logo;
    FontRender m_subtitleFont;
    FontRender m_comingSoonFont;
    FontRender m_hintFont;
    int m_selected = 0;
    Action m_action = Action::None;
    POINT m_lastMouse = {};
    bool m_wasMousePressed = false;
    bool m_wasEnterPressed = false;
    int m_navigationDirection = 0;
    float m_navigationCooldown = 0.0f;
    float m_navigationRepeat = 0.0f;
    bool m_navigationBlocked = false;
};