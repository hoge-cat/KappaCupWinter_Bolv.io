#pragma once
#include <array>

class SettingsUI
{
public:
    void Start();
    void Open();
    bool IsOpen() const { return m_open; }
    bool HitGear(float x, float y) const;
    void SetGearSelected(bool selected);
    void Update();
    void Render(RenderContext& rc);
private:
    void Refresh();
    void Close();
    int HitRow(float x, float y) const;
    SpriteRender m_gear, m_overlay, m_panel;
    std::array<SpriteRender, 5> m_rows;
    std::array<SpriteRender, 3> m_tracks, m_fills, m_knobs;
    std::array<FontRender, 5> m_labels;
    std::array<FontRender, 3> m_values;
    FontRender m_heading, m_mode, m_hint, m_notice;
    bool m_open = false;
    bool m_saveFailureAcknowledged = false;
    bool m_wasMouse = false, m_wasConfirm = false, m_wasEscape = false;
    bool m_blocked = true;
    int m_selected = 0, m_drag = -1;
    int m_lastNavigation = 0, m_lastAdjustment = 0;
    float m_navigationTimer = 0, m_adjustmentTimer = 0;
    POINT m_lastMouse = {};
};
