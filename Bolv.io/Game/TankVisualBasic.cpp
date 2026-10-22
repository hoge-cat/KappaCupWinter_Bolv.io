#include "stdafx.h"
#include "TankVisual.h"


// ============================================================
// UpdateBasic
// ============================================================

void TankVisual::UpdateBasic()
{
    // Basicはデフォルト状態の
    // 中央1本砲身をそのまま使用する。
}


// ============================================================
// RenderBasic
// ============================================================

void TankVisual::RenderBasic(
    RenderContext& rc)
{
    m_barrelRender1.Draw(
        rc
    );
}