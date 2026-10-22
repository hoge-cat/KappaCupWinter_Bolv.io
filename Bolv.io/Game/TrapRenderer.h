#pragma once


class TrapRenderer :
    public IGameObject
{
public:

    bool Start();

    void Render(
        RenderContext& rc
    );


private:

    // ========================================================
    // 最大描画数
    //
    // Player Trapだけならかなり余裕あり。
    // 後でEnemyTrapは別Rendererにする。
    // ========================================================

    static const int MAX_INSTANCE =
        1000;


    // ========================================================
    // Trapモデル
    // ========================================================

    ModelRender m_modelRender;


    // ========================================================
    // 前フレームの使用数
    // ========================================================

    int m_previousInstanceCount =
        0;
};