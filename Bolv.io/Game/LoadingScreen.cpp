
#include "stdafx.h"
#include "LoadingScreen.h"
#include <cmath>

namespace
{
    void CenterLoadingText(FontRender& font, const wchar_t* text, float scale, float y)
    {
        const Vector2 size = g_graphicsEngine->GetFontEngine().MeasureText(text);
        font.SetText(text);
        font.SetScale(scale);
        font.SetPosition(Vector3(0.0f, y, 0.0f));
        font.SetPivot(size.x * 0.5f, 1.0f - size.y * 0.5f);
        font.SetShadowParam(false, 0.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    }
}

bool LoadingScreen::Start()
{
    // タイトル
    m_background.Init("Assets/sprite/White.dds", UI_SPACE_WIDTH, UI_SPACE_HEIGHT);
    m_background.SetMulColor(Vector4(0.025f, 0.045f, 0.075f, 1.0f));
    m_background.Update();
    m_logo.Init("Assets/sprite/BolvLogo.DDS", 740.0f, 740.0f * 793.0f / 1983.0f);
    m_logo.SetPosition(Vector3(0.0f, 165.0f, 0.0f));
    m_logo.Update();
    m_loadingTrack.Init("Assets/sprite/White.dds", 420.0f, 5.0f);
    m_loadingTrack.SetPosition(Vector3(0.0f, -165.0f, 0.0f));
    m_loadingTrack.SetMulColor(Vector4(0.10f, 0.18f, 0.25f, 1.0f));
    m_loadingTrack.Update();
    m_loadingIndicator.Init("Assets/sprite/White.dds", 84.0f, 5.0f);
    m_loadingIndicator.SetMulColor(Vector4(0.08f, 0.73f, 0.93f, 1.0f));
    m_loadingIndicator.SetPosition(Vector3(-168.0f, -165.0f, 0.0f));
    m_loadingIndicator.Update();
    // ロード中の文字
    CenterLoadingText(m_loadingFont, L"\u8aad\u307f\u8fbc\u307f\u4e2d", 1.0f, -65.0f);
    m_loadingFont.SetColor(Vector4(0.90f, 0.96f, 1.0f, 1.0f));
    CenterLoadingText(m_hintFont,
        L"WASD\uff1a\u79fb\u52d5\u3000\u30de\u30a6\u30b9\uff1a\u7167\u6e96\u3000\u5de6\u30af\u30ea\u30c3\u30af\uff1a\u5c04\u6483\u3000\u53f3\u30af\u30ea\u30c3\u30af\u9577\u62bc\u3057\uff1a\u30ab\u30fc\u30bd\u30eb\u64cd\u4f5c", 0.6f, -405.0f);
    m_hintFont.SetColor(Vector4(0.48f, 0.62f, 0.73f, 1.0f));

    return true;
}

void LoadingScreen::Update()
{
    m_elapsed += g_gameTime->GetFrameDeltaTime();
    // An indeterminate indicator: elapsed time is not loading progress.
    const float phase = std::fmod(m_elapsed, 1.4f) / 1.4f;
    const float travel = 1.0f - std::fabs(phase * 2.0f - 1.0f);
    m_loadingIndicator.SetPosition(Vector3(-168.0f + travel * 336.0f, -165.0f, 0.0f));
    m_loadingIndicator.Update();
}

void LoadingScreen::Render(RenderContext& rc)
{
    m_background.Draw(rc);
    m_logo.Draw(rc);
    m_loadingTrack.Draw(rc);
    m_loadingIndicator.Draw(rc);
    m_loadingFont.Draw(rc);
    m_hintFont.Draw(rc);
}
