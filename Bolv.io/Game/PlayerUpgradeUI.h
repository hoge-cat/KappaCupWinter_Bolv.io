#pragma once

class Player;

class PlayerUpgradeUI : public IGameObject
{
public:
    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:
    // 表示を更新する。
    void UpdateText();

private:
    Player* m_player = nullptr;

    static const int STAT_COUNT = 8;

    FontRender m_statFontRender[STAT_COUNT];

    FontRender m_pointFontRender;
};