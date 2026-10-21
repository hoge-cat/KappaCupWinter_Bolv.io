#include "stdafx.h"
#include "PlayerEvolutionUI.h"

#include "Player.h"
#include "system/system.h"


// ============================================================
// Start
// ============================================================

bool PlayerEvolutionUI::Start()
{
    // ============================================
    // Player取得
    // ============================================

    m_player =
        FindGO<Player>(
            "player"
        );


    // ============================================
    // Title
    // ============================================

    m_titleFontRender.SetPosition(
        Vector3(
            OPTION_X,
            -180.0f,
            0.0f
        )
    );


    m_titleFontRender.SetScale(
        1.0f
    );


    m_titleFontRender.SetColor(
        {
            1.0f,
            1.0f,
            1.0f,
            1.0f
        }
    );


    // ============================================
    // Options
    // ============================================

    for (int i = 0;
        i <
        MAX_OPTION_COUNT;
        ++i)
    {
        const float optionY =
            OPTION_START_Y -
            OPTION_INTERVAL_Y *
            static_cast<float>(
                i
                );


        m_optionFontRender[i]
            .SetPosition(
                Vector3(
                    OPTION_X,
                    optionY,
                    0.0f
                )
            );


        m_optionFontRender[i]
            .SetScale(
                0.9f
            );


        m_optionFontRender[i]
            .SetColor(
                {
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f
                }
            );


        m_optionFontRender[i]
            .SetText(
                L""
            );
    }


    return true;
}


// ============================================================
// Update
// ============================================================

void PlayerEvolutionUI::Update()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    // ============================================
    // 左クリック
    // ============================================

    const bool leftMouse =
        (
            GetAsyncKeyState(
                VK_LBUTTON
            ) &
            0x8000
            ) !=
        0;


    // ============================================
    // Evolution
    // ============================================

    PlayerEvolution&
        evolution =
        m_player
        ->GetEvolutionMutable();


    // ============================================
    // 現在進化できない
    // ============================================

    if (!evolution
        .HasAvailableEvolution())
    {
        // ========================================
        // UIを消す
        // ========================================

        m_titleFontRender.SetText(
            L""
        );


        m_optionCount =
            0;


        for (int i = 0;
            i <
            MAX_OPTION_COUNT;
            ++i)
        {
            m_optionFontRender[i]
                .SetText(
                    L""
                );
        }


        m_leftMousePressed =
            leftMouse;


        return;
    }


    // ============================================
    // 候補数更新
    // ============================================

    m_optionCount =
        evolution
        .GetAvailableEvolutionCount();


    if (m_optionCount >
        MAX_OPTION_COUNT)
    {
        m_optionCount =
            MAX_OPTION_COUNT;
    }


    // ============================================
    // Title
    // ============================================

    m_titleFontRender.SetText(
        L"Evolution Available!"
    );


    // ============================================
    // 選択肢文字更新
    // ============================================

    UpdateOptionTexts();


    // ============================================
    // Hover色
    // ============================================

    UpdateHoverColors();


    // ============================================
    // 左クリックした瞬間だけ選択
    // ============================================

    if (leftMouse &&
        !m_leftMousePressed)
    {
        HandleEvolutionSelection();
    }


    m_leftMousePressed =
        leftMouse;
}


// ============================================================
// UpdateOptionTexts
// ============================================================

void PlayerEvolutionUI::UpdateOptionTexts()
{
    // ============================================
    // 全Optionを消す
    // ============================================

    for (int i = 0;
        i <
        MAX_OPTION_COUNT;
        ++i)
    {
        m_optionFontRender[i]
            .SetText(
                L""
            );
    }


    // ============================================
    // 現在選択可能な候補だけ表示
    // ============================================

    for (int i = 0;
        i <
        m_optionCount;
        ++i)
    {
        m_optionFontRender[i]
            .SetText(
                GetTankClassName(
                    i
                )
            );
    }
}


// ============================================================
// UpdateHoverColors
// ============================================================

void PlayerEvolutionUI::UpdateHoverColors()
{
    const Vector4 normalColor =
    {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };


    const Vector4 hoverColor =
    {
        1.0f,
        1.0f,
        0.0f,
        1.0f
    };


    // ============================================
    // 全部通常色へ戻す
    // ============================================

    for (int i = 0;
        i <
        MAX_OPTION_COUNT;
        ++i)
    {
        m_optionFontRender[i]
            .SetColor(
                normalColor
            );
    }


    // ============================================
    // Hover中だけ色変更
    // ============================================

    for (int i = 0;
        i <
        m_optionCount;
        ++i)
    {
        if (!IsMouseOverOption(
            i
        ))
        {
            continue;
        }


        m_optionFontRender[i]
            .SetColor(
                hoverColor
            );


        break;
    }
}


// ============================================================
// HandleEvolutionSelection
// ============================================================

void PlayerEvolutionUI::HandleEvolutionSelection()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    PlayerEvolution&
        evolution =
        m_player
        ->GetEvolutionMutable();


    // ============================================
    // Hover中候補を探す
    // ============================================

    for (int i = 0;
        i <
        m_optionCount;
        ++i)
    {
        if (!IsMouseOverOption(
            i
        ))
        {
            continue;
        }


        // ========================================
        // TankClass取得
        // ========================================

        const PlayerEvolution::TankClass
            tankClass =
            evolution
            .GetAvailableEvolution(
                i
            );


        // ========================================
        // 進化
        // ========================================

        if (evolution
            .EvolveTo(
                tankClass
            ))
        {
            OutputDebugStringA(
                "Player evolved.\n"
            );
        }


        return;
    }
}


// ============================================================
// GetTankClassName
// ============================================================

const wchar_t*
PlayerEvolutionUI::GetTankClassName(
    int optionIndex
) const
{
    if (m_player ==
        nullptr)
    {
        return L"";
    }


    const PlayerEvolution&
        evolution =
        m_player
        ->GetEvolution();


    const PlayerEvolution::TankClass
        tankClass =
        evolution
        .GetAvailableEvolution(
            optionIndex
        );


    // ============================================
    // Basic
    // ============================================

    if (tankClass ==
        PlayerEvolution::Basic)
    {
        return L"Basic";
    }


    // ============================================
    // Twin
    // ============================================

    if (tankClass ==
        PlayerEvolution::Twin)
    {
        return L"Twin";
    }


    // ============================================
    // Triple Shot
    // ============================================

    if (tankClass ==
        PlayerEvolution::TripleShot)
    {
        return L"Triple Shot";
    }


    // ============================================
    // Quad Tank
    // ============================================

    if (tankClass ==
        PlayerEvolution::QuadTank)
    {
        return L"Quad Tank";
    }


    // ============================================
    // Twin Flank
    // ============================================

    if (tankClass ==
        PlayerEvolution::TwinFlank)
    {
        return L"Twin Flank";
    }


    // ============================================
    // Sniper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sniper)
    {
        return L"Sniper";
    }


    // ============================================
    // Assassin
    // ============================================

    if (tankClass ==
        PlayerEvolution::Assassin)
    {
        return L"Assassin";
    }


    // ============================================
    // Hunter
    // ============================================

    if (tankClass ==
        PlayerEvolution::Hunter)
    {
        return L"Hunter";
    }


    // ============================================
    // Trapper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Trapper)
    {
        return L"Trapper";
    }


    // ============================================
    // Machine Gun
    // ============================================

    if (tankClass ==
        PlayerEvolution::MachineGun)
    {
        return L"Machine Gun";
    }


    // ============================================
    // Flank Guard
    // ============================================

    if (tankClass ==
        PlayerEvolution::FlankGuard)
    {
        return L"Flank Guard";
    }


    // ============================================
    // Smasher
    // ============================================

    if (tankClass ==
        PlayerEvolution::Smasher)
    {
        return L"Smasher";
    }


    // ============================================
    // Sprayer
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sprayer)
    {
        return L"Sprayer";
    }


    return L"";
}


// ============================================================
// Render
// ============================================================

void PlayerEvolutionUI::Render(
    RenderContext& rc)
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    const PlayerEvolution&
        evolution =
        m_player
        ->GetEvolution();


    // ============================================
    // 候補なし
    // ============================================

    if (!evolution
        .HasAvailableEvolution())
    {
        return;
    }


    // ============================================
    // Title
    // ============================================

    m_titleFontRender.Draw(
        rc
    );


    // ============================================
    // Options
    // ============================================

    for (int i = 0;
        i <
        m_optionCount;
        ++i)
    {
        m_optionFontRender[i]
            .Draw(
                rc
            );
    }
}


// ============================================================
// IsMouseOverOption
// ============================================================

bool PlayerEvolutionUI::IsMouseOverOption(
    int optionIndex
) const
{
    // ============================================
    // 範囲確認
    // ============================================

    if (optionIndex <
        0 ||
        optionIndex >=
        m_optionCount)
    {
        return false;
    }


    POINT mousePosition;


    GetCursorPos(
        &mousePosition
    );


    ScreenToClient(
        g_hWnd,
        &mousePosition
    );


    RECT clientRect;


    GetClientRect(
        g_hWnd,
        &clientRect
    );


    const float width =
        static_cast<float>(
            clientRect.right -
            clientRect.left
            );


    const float height =
        static_cast<float>(
            clientRect.bottom -
            clientRect.top
            );


    if (width <=
        0.0f ||
        height <=
        0.0f)
    {
        return false;
    }


    // ============================================
    // 画面中央を(0,0)
    // ============================================

    const float mouseX =
        static_cast<float>(
            mousePosition.x
            ) -
        width *
        0.5f;


    const float mouseY =
        height *
        0.5f -
        static_cast<float>(
            mousePosition.y
            );


    // ============================================
    // 横範囲
    // ============================================

    const float left =
        320.0f;


    const float right =
        630.0f;


    // ============================================
    // 縦位置
    // ============================================

    const float optionY =
        OPTION_START_Y -
        OPTION_INTERVAL_Y *
        static_cast<float>(
            optionIndex
            );


    const float top =
        optionY +
        5.0f;


    const float bottom =
        optionY -
        30.0f;


    return
        mouseX >=
        left &&

        mouseX <=
        right &&

        mouseY <=
        top &&

        mouseY >=
        bottom;
}