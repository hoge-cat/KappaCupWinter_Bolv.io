#pragma once
#include <array>

class PauseUI : public IGameObject
{
public:
    enum class Action { None, Resume, Restart, Title };
    bool Start();
    bool RunsWhilePaused() const override { return true; }
    void Update();
    void Render(RenderContext& rc);
    Action ConsumeAction();
private:
    void UpdateColors();
    int HitTest(float x, float y) const;
    void UpdateNavigation(bool up, bool down, bool active, float deltaTime);
    SpriteRender m_background;
    SpriteRender m_panel;
    std::array<SpriteRender, 3> m_borders;
    std::array<SpriteRender, 3> m_fills;
    std::array<FontRender, 3> m_labels;
    FontRender m_heading;
    FontRender m_hint;
    int m_selected = 0;
    Action m_action = Action::None;
    POINT m_lastMouse = {};
    bool m_wasMousePressed = false;
    bool m_wasEnterPressed = false;
    int m_navigationDirection = 0;
    float m_navigationCooldown = 0;
    float m_navigationRepeat = 0;
    bool m_navigationBlocked = false;
};
