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
        case PlayerEvolution::Twin: return L"ツイン";
        case PlayerEvolution::Sniper: return L"スナイパー";
        case PlayerEvolution::MachineGun: return L"マシンガン";
        case PlayerEvolution::FlankGuard: return L"フランクガード";
        case PlayerEvolution::TripleShot: return L"トリプルショット";
        case PlayerEvolution::QuadTank: return L"クアッドタンク";
        case PlayerEvolution::TwinFlank: return L"ツインフランク";
        case PlayerEvolution::Assassin: return L"アサシン";
        case PlayerEvolution::Hunter: return L"ハンター";
        case PlayerEvolution::Trapper: return L"トラッパー";
        case PlayerEvolution::Smasher: return L"スマッシャー";
        case PlayerEvolution::Sprayer: return L"スプレイヤー";
        default: return L"基本戦車";
        }
    }
}