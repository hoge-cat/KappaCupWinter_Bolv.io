
#pragma once

class LoadingScreen : public IGameObject
{
public:
    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender m_background;
    SpriteRender m_logo;
    SpriteRender m_loadingTrack;
    SpriteRender m_loadingIndicator;
    FontRender m_hintFont;
    float m_elapsed = 0.0f;
    FontRender m_loadingFont;
};
