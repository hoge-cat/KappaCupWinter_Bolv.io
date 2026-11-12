#include "stdafx.h"
#include "PlayerEvolutionUI.h"

#include "Player.h"
#include "AudioManager.h"
#include "system/system.h"
#include "InGameUI.h"


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


    for (int i = 0; i < MAX_OPTION_COUNT; ++i)
    {
        const Vector3 center = OptionCenter(i);
        BolvUI::Rect(m_cardBackground[i], 164, 92, center.x, center.y, Vector4(0.18f,0.18f,0.18f,0.85f));
        BolvUI::Rect(m_cardFill[i], 156, 84, center.x, center.y, BolvUI::StatColor(i));
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
        L"\u9032\u5316\u5148\u3092\u9078\u629e\uff08\u53f3\u30af\u30ea\u30c3\u30af\u9577\u62bc\u3057\uff0b\u5de6\u30af\u30ea\u30c3\u30af\uff09"
    );
    BolvUI::Center(m_titleFontRender, m_titleFontRender.GetText(), -740, 495, 0.42f);


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
        const Vector3 center = OptionCenter(i);
        BolvUI::Center(m_optionFontRender[i], m_optionFontRender[i].GetText(), center.x, center.y, 0.42f);
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
        m_cardFill[i].SetMulColor(BolvUI::StatColor(i));
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
        m_cardFill[i].SetMulColor(Vector4(0.20f,0.72f,0.90f,1.0f));


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
            if (auto* audio = FindGO<AudioManager>("audioManager"))
            {
                audio->PlaySelectSE();
                audio->PlayEvolveSE(true);
            }

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
        return L"\u57fa\u672c\u6226\u8eca";
    }


    // ============================================
    // Twin
    // ============================================

    if (tankClass ==
        PlayerEvolution::Twin)
    {
        return L"\u30c4\u30a4\u30f3";
    }


    // ============================================
    // Triple Shot
    // ============================================

    if (tankClass ==
        PlayerEvolution::TripleShot)
    {
        return L"\u30c8\u30ea\u30d7\u30eb\u30b7\u30e7\u30c3\u30c8";
    }


    // ============================================
    // Quad Tank
    // ============================================

    if (tankClass ==
        PlayerEvolution::QuadTank)
    {
        return L"\u30af\u30a2\u30c3\u30c9\u30bf\u30f3\u30af";
    }


    // ============================================
    // Twin Flank
    // ============================================

    if (tankClass ==
        PlayerEvolution::TwinFlank)
    {
        return L"\u30c4\u30a4\u30f3\u30d5\u30e9\u30f3\u30af";
    }


    // ============================================
    // Sniper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sniper)
    {
        return L"\u30b9\u30ca\u30a4\u30d1\u30fc";
    }


    // ============================================
    // Assassin
    // ============================================

    if (tankClass ==
        PlayerEvolution::Assassin)
    {
        return L"\u30a2\u30b5\u30b7\u30f3";
    }


    // ============================================
    // Hunter
    // ============================================

    if (tankClass ==
        PlayerEvolution::Hunter)
    {
        return L"\u30cf\u30f3\u30bf\u30fc";
    }


    // ============================================
    // Trapper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Trapper)
    {
        return L"\u30c8\u30e9\u30c3\u30d1\u30fc";
    }


    // ============================================
    // Machine Gun
    // ============================================

    if (tankClass ==
        PlayerEvolution::MachineGun)
    {
        return L"\u30de\u30b7\u30f3\u30ac\u30f3";
    }


    // ============================================
    // Flank Guard
    // ============================================

    if (tankClass ==
        PlayerEvolution::FlankGuard)
    {
        return L"\u30d5\u30e9\u30f3\u30af\u30ac\u30fc\u30c9";
    }


    // ============================================
    // Smasher
    // ============================================

    if (tankClass ==
        PlayerEvolution::Smasher)
    {
        return L"\u30b9\u30de\u30c3\u30b7\u30e3\u30fc";
    }


    // ============================================
    // Sprayer
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sprayer)
    {
        return L"\u30b9\u30d7\u30ec\u30a4\u30e4\u30fc";
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

    if (m_player->GetHealth().IsDead()) { return; }
    for (int i = 0; i < m_optionCount; ++i)
    {
        m_cardBackground[i].Draw(rc);
        m_cardFill[i].Draw(rc);
    }

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
        static_cast<float>(mousePosition.x) * UI_SPACE_WIDTH / width - UI_SPACE_WIDTH * 0.5f;


    const float mouseY =
        UI_SPACE_HEIGHT * 0.5f - static_cast<float>(mousePosition.y) * UI_SPACE_HEIGHT / height;


    // ============================================
    // 横範囲
    // ============================================

    const float left =
        OptionCenter(optionIndex).x - 78.0f;


    const float right =
        OptionCenter(optionIndex).x + 78.0f;


    // ============================================
    // 縦位置
    // ============================================

    const float optionY =
        OptionCenter(optionIndex).y;


    const float top =
        optionY + 42.0f;


    const float bottom =
        optionY - 42.0f;


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