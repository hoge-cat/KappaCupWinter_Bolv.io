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
    SpriteRender m_xpOutline, m_xpTrack, m_xpFill;
    SpriteRender m_hpTrack, m_hpFill;
    SpriteRender m_scorePanel;
    SpriteRender m_mapBorder, m_mapBackground, m_mapPlayer;
    SpriteRender m_mapEnemies[32];
    FontRender m_classFont, m_scoreHeading, m_mapHeading;

    // Lv / EXP。
    FontRender m_fontRender;

    // HP。
    FontRender m_hpFontRender;

    // SCORE。
    FontRender m_scoreFontRender;

    // 強化ポイント。
    FontRender m_upgradePointFontRender;
};