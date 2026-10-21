#pragma once


class EnemyRenderer : public IGameObject
{
public:

    bool Start();

    void Render(
        RenderContext& rc
    );


private:

    static const int MAX_INSTANCE =
        500;


    // ============================================
    // 本体
    // ============================================

    ModelRender m_bodyRender;


    // ============================================
    // 通常砲身
    //
    // Basic / Twin / Sniper / Flank Guard /
    // Sprayerの小砲身で使用。
    // ============================================

    ModelRender m_barrelRender1;

    ModelRender m_barrelRender2;


    // ============================================
    // Machine Gun / Sprayer主砲
    // ============================================

    ModelRender m_machineGunBarrelRender;


    // ============================================
    // Smasher六角外装
    // ============================================

    ModelRender m_smasherShellRender;


    int m_previousInstanceCount =
        0;


    // ============================================
    // Smasher回転
    // ============================================

    float m_smasherRotationDeg =
        0.0f;
};