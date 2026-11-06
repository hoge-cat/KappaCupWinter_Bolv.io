
#include "stdafx.h"
#include "LoadingScreen.h"

bool LoadingScreen::Start()
{
    // タイトル
    m_titleFont.SetText(L"Bolv.io");

    m_titleFont.SetPosition(
        Vector3(-250.0f, 100.0f, 0.0f)
    );

    m_titleFont.SetScale(4.0f);

    m_titleFont.SetColor(
        { 0.0f, 0.65f, 1.0f, 1.0f }
    );

    // ロード中の文字
    m_loadingFont.SetText(L"NOW LOADING...");

    m_loadingFont.SetPosition(
        Vector3(-220.0f, -80.0f, 0.0f)
    );

    m_loadingFont.SetScale(1.5f);

    m_loadingFont.SetColor(
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    return true;
}

void LoadingScreen::Render(RenderContext& rc)
{
    m_titleFont.Draw(rc);
    m_loadingFont.Draw(rc);
}
