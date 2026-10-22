#pragma once

class Player;

class PlayerHUD : public IGameObject
{
public:
    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:
    Player* m_player = nullptr;

    // Lv / EXP。
    FontRender m_fontRender;

    // HP。
    FontRender m_hpFontRender;

    // SCORE。
    FontRender m_scoreFontRender;

    // 強化ポイント。
    FontRender m_upgradePointFontRender;
};