#include "stdafx.h"
#include "Background.h"


Background::Background()
{}


Background::~Background()
{}


bool Background::Start()
{
    // ============================================
    // 地面
    // ============================================

    m_modelRender.Init(
        "Assets/modelData/ground.tkm"
    );


    m_modelRender.Update();


    // ============================================
    // 地面の物理オブジェクト
    // ============================================

    m_phyStaticObject.CreateFromModel(
        m_modelRender.GetModel(),
        m_modelRender
        .GetModel()
        .GetWorldMatrix()
    );


    // ============================================
    // 外周の壁
    // ============================================

    CreateWalls();


    return true;
}


void Background::CreateWalls()
{
    // 内側の範囲は
    // X・Zともに -1500 ～ 1500。
    //
    // 元モデルは
    // 100 × 100 × 100。
    //
    // 底面中央が原点。


    // ============================================
    // 奥
    // ============================================

    m_walls[0].Init(
        Vector3(
            0.0f,
            0.0f,
            1525.0f
        ),
        Vector3(
            31.0f,
            1.0f,
            0.5f
        )
    );


    // ============================================
    // 手前
    // ============================================

    m_walls[1].Init(
        Vector3(
            0.0f,
            0.0f,
            -1525.0f
        ),
        Vector3(
            31.0f,
            1.0f,
            0.5f
        )
    );


    // ============================================
    // 右
    // ============================================

    m_walls[2].Init(
        Vector3(
            1525.0f,
            0.0f,
            0.0f
        ),
        Vector3(
            0.5f,
            1.0f,
            30.0f
        )
    );


    // ============================================
    // 左
    // ============================================

    m_walls[3].Init(
        Vector3(
            -1525.0f,
            0.0f,
            0.0f
        ),
        Vector3(
            0.5f,
            1.0f,
            30.0f
        )
    );
}


void Background::Render(
    RenderContext& rc)
{
    // 地面。
    m_modelRender.Draw(
        rc
    );


    // 外周の壁。
    for (StageWall& wall :
        m_walls)
    {
        wall.Render(
            rc
        );
    }
}