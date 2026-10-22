#pragma once


class WorldHealthBarRenderer :
    public IGameObject
{
public:

    bool Start();

    void Render(
        RenderContext& rc
    );


private:

    // ============================================
    // HPバー1本分
    // ============================================

    struct HealthBar
    {
        SpriteRender background;

        SpriteRender health;
    };


    // ============================================
    // 最大表示数
    //
    // 現状：
    // Shape 約30
    // Enemy 約3
    //
    // 余裕を持って128。
    // ============================================

    static const int MAX_HEALTH_BARS =
        128;


    // ============================================
    // 1本描画
    // ============================================

    bool DrawHealthBar(
        RenderContext& rc,
        int index,
        const Vector3& worldPosition,
        int currentHP,
        int maxHP,
        float width
    );


private:

    HealthBar m_healthBars[
        MAX_HEALTH_BARS
    ];
};