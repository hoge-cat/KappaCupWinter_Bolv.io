#pragma once

class Player;

class GameOverUI : public IGameObject
{
public:
    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:
    Player* m_player = nullptr;

    FontRender m_fontRender;
    FontRender m_resultFontRender;

    bool m_resultFixed = false;
};