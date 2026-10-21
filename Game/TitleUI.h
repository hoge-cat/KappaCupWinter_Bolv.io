#pragma once

class TitleUI : public IGameObject
{
public:
    bool Start();
    void Render(RenderContext& rc);

private:
    FontRender m_titleFont;
    FontRender m_startFont;
};