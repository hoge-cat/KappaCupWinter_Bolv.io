
#pragma once

class LoadingScreen : public IGameObject
{
public:
    bool Start();
    void Render(RenderContext& rc);

private:
    FontRender m_titleFont;
    FontRender m_loadingFont;
};
