#pragma once
#include <array>

class ClearResultUI : public IGameObject
{
public:
    enum class Action { None, Retry, Title };
    void SetResult(int score, int level) { m_score = score; m_level = level; }
    bool Start();
    void Update();
    void Render(RenderContext& rc);
    Action ConsumeAction();
private:
    void UpdateColors();
    int HitTest(float x, float y) const;
    void UpdateNavigation(bool up, bool down, bool active, float deltaTime);
    SpriteRender m_background;
    SpriteRender m_panel;
    std::array<SpriteRender, 2> m_borders;
    std::array<SpriteRender, 2> m_fills;
    std::array<FontRender, 2> m_labels;
    FontRender m_heading;
    FontRender m_summary;
    FontRender m_hint;
    int m_score = 0;
    int m_level = 1;
    int m_selected = 0;
    Action m_action = Action::None;
    POINT m_lastMouse = {};
    bool m_wasMousePressed = false;
    bool m_wasEnterPressed = false;
    bool m_wasRPressed = false;
    bool m_wasTPressed = false;
    int m_navigationDirection = 0;
    float m_navigationCooldown = 0;
    float m_navigationRepeat = 0;
    bool m_navigationBlocked = false;
};
