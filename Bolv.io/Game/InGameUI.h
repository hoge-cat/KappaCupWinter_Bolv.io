#pragma once
#include "PlayerEvolution.h"

namespace BolvUI
{
    inline float Clamp01(float value) { return value < 0 ? 0 : (value > 1 ? 1 : value); }
    inline void Rect(SpriteRender& sprite, float width, float height, float x, float y, const Vector4& color)
    {
        sprite.Init("Assets/sprite/White.dds", width, height);
        sprite.SetPosition(Vector3(x, y, 0));
        sprite.SetMulColor(color);
        sprite.Update();
    }
    inline void Center(FontRender& font, const wchar_t* text, float x, float y, float scale)
    {
        const Vector2 size = g_graphicsEngine->GetFontEngine().MeasureText(text);
        if (text != font.GetText()) { font.SetText(text); }
        font.SetPosition(Vector3(x, y, 0));
        font.SetPivot(size.x * 0.5f, 1.0f - size.y * 0.5f);
        font.SetScale(scale);
        font.SetColor(Vector4(1, 1, 1, 1));
        font.SetShadowParam(true, 1.0f, Vector4(0.15f, 0.15f, 0.15f, 0.9f));
    }
    inline Vector4 StatColor(int index)
    {
        const Vector4 colors[] = {
            {0.76f,0.48f,0.83f,1}, {0.91f,0.43f,0.63f,1},
            {0.89f,0.38f,0.38f,1}, {0.37f,0.66f,0.89f,1},
            {0.90f,0.79f,0.35f,1}, {0.91f,0.59f,0.31f,1},
            {0.40f,0.78f,0.79f,1}, {0.51f,0.80f,0.39f,1}
        };
        return colors[index % 8];
    }
    inline const wchar_t* TankName(PlayerEvolution::TankClass tankClass)
    {
        switch(tankClass)
        {
        case PlayerEvolution::Twin: return L"\u30c4\u30a4\u30f3";
        case PlayerEvolution::Sniper: return L"\u30b9\u30ca\u30a4\u30d1\u30fc";
        case PlayerEvolution::MachineGun: return L"\u30de\u30b7\u30f3\u30ac\u30f3";
        case PlayerEvolution::FlankGuard: return L"\u30d5\u30e9\u30f3\u30af\u30ac\u30fc\u30c9";
        case PlayerEvolution::TripleShot: return L"\u30c8\u30ea\u30d7\u30eb\u30b7\u30e7\u30c3\u30c8";
        case PlayerEvolution::QuadTank: return L"\u30af\u30a2\u30c3\u30c9\u30bf\u30f3\u30af";
        case PlayerEvolution::TwinFlank: return L"\u30c4\u30a4\u30f3\u30d5\u30e9\u30f3\u30af";
        case PlayerEvolution::Assassin: return L"\u30a2\u30b5\u30b7\u30f3";
        case PlayerEvolution::Hunter: return L"\u30cf\u30f3\u30bf\u30fc";
        case PlayerEvolution::Trapper: return L"\u30c8\u30e9\u30c3\u30d1\u30fc";
        case PlayerEvolution::Smasher: return L"\u30b9\u30de\u30c3\u30b7\u30e3\u30fc";
        case PlayerEvolution::Sprayer: return L"\u30b9\u30d7\u30ec\u30a4\u30e4\u30fc";
        default: return L"\u57fa\u672c\u6226\u8eca";
        }
    }
}