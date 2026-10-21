#include "stdafx.h"
#include "TitleUI.h"

bool TitleUI::Start()
{
    m_titleFont.SetText(L"Bolv.io");
    m_titleFont.SetPosition(
        Vector3(-250.0f, 100.0f, 0.0f)
    );
    m_titleFont.SetScale(4.0f);
    m_titleFont.SetColor(
        { 0.0f, 0.65f, 1.0f, 1.0f }
    );

    m_startFont.SetText(L"[ENTER] START");
    m_startFont.SetPosition(
        Vector3(-250.0f, -80.0f, 0.0f)
    );
    m_startFont.SetScale(1.5f);
    m_startFont.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    return true;
}

void TitleUI::Render(RenderContext& rc)
{
    m_titleFont.Draw(rc);
    m_startFont.Draw(rc);
}