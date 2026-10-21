#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "system/system.h"


namespace
{
    // ============================================
    // Sniper
    // ============================================

    const float SNIPER_CAMERA_DISTANCE_MULTIPLIER =
        1.18f;


    // ============================================
    // Assassin
    //
    // Sniperよりさらに遠くを見る。
    // ============================================

    const float ASSASSIN_CAMERA_DISTANCE_MULTIPLIER =
        1.30f;
}


// ============================================================
// Start
// ============================================================

bool GameCamera::Start()
{
    m_player =
        FindGO<Player>(
            "player"
        );


    g_camera3D->SetNear(
        1.0f
    );


    g_camera3D->SetFar(
        10000.0f
    );


    // Lv1時のカメラ距離を保存。
    m_baseCameraDistance =
        m_offset.Length();


    // ゲーム開始時はカーソル非表示。
    g_isCursorVisible =
        false;


    return true;
}


// ============================================================
// Update
// ============================================================

void GameCamera::Update()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    UpdateMouseRotation();


    // レベルと進化先に応じて
    // カメラ距離を変更。
    UpdateLevelDistance();


    Vector3 target =
        m_player
        ->GetPosition();


    target.y +=
        50.0f;


    g_camera3D->SetTarget(
        target
    );


    g_camera3D->SetPosition(
        target +
        m_offset
    );


    g_camera3D->Update();
}


// ============================================================
// レベル・クラスによるカメラ距離
// ============================================================

void GameCamera::UpdateLevelDistance()
{
    if (m_player ==
        nullptr)
    {
        return;
    }


    const int currentLevel =
        m_player
        ->GetProgress()
        .GetLevel();


    // ============================================
    // レベルによる距離
    //
    // Lv1  = 1.0倍
    // Lv45 = 約1.22倍
    // ============================================

    const float levelDistanceScale =
        1.0f +
        static_cast<float>(
            currentLevel -
            1
            ) *
        0.005f;


    // ============================================
    // クラスによる距離
    // ============================================

    float classDistanceScale =
        1.0f;


    const PlayerEvolution::TankClass tankClass =
        m_player
        ->GetEvolution()
        .GetCurrentClass();


    // ============================================
    // Sniper
    // ============================================

    if (tankClass ==
        PlayerEvolution::Sniper)
    {
        classDistanceScale =
            SNIPER_CAMERA_DISTANCE_MULTIPLIER;
    }


    // ============================================
    // Assassin
    // ============================================

    else if (
        tankClass ==
        PlayerEvolution::Assassin)
    {
        classDistanceScale =
            ASSASSIN_CAMERA_DISTANCE_MULTIPLIER;
    }


    const float targetDistance =
        m_baseCameraDistance *
        levelDistanceScale *
        classDistanceScale;


    // ============================================
    // 現在のカメラ方向を維持
    // ============================================

    Vector3 offsetDirection =
        m_offset;


    const float currentDistance =
        offsetDirection.Length();


    if (currentDistance <=
        0.0001f)
    {
        return;
    }


    offsetDirection.Normalize();


    m_offset =
        offsetDirection *
        targetDistance;
}


// ============================================================
// マウスによるカメラ回転
// ============================================================

void GameCamera::UpdateMouseRotation()
{
    // ============================================
    // ゲームが最前面ではない場合
    // ============================================

    if (GetForegroundWindow() !=
        g_hWnd)
    {
        m_mouseReady =
            false;


        return;
    }


    // ============================================
    // 右クリック状態
    // ============================================

    const bool cursorMode =
        (
            GetAsyncKeyState(
                VK_RBUTTON
            ) &
            0x8000
            ) != 0;


    // ============================================
    // 右クリック中
    // カーソル操作モード
    // ============================================

    if (cursorMode)
    {
        if (!m_cursorVisible)
        {
            // カーソル移動制限解除。
            ClipCursor(
                nullptr
            );


            // Windows側にも表示状態を伝える。
            g_isCursorVisible =
                true;


            // 確実に表示。
            while (
                ShowCursor(
                    TRUE
                ) <
                0)
            {
            }


            SetCursor(
                LoadCursor(
                    NULL,
                    IDC_ARROW
                )
            );


            m_cursorVisible =
                true;
        }


        // カーソル操作中は
        // カメラを回さない。
        m_mouseReady =
            false;


        return;
    }


    // ============================================
    // 通常モード
    // ============================================

    RECT rect;


    if (!GetClientRect(
        g_hWnd,
        &rect))
    {
        return;
    }


    POINT center;


    center.x =
        (
            rect.left +
            rect.right
            ) /
        2;


    center.y =
        (
            rect.top +
            rect.bottom
            ) /
        2;


    ClientToScreen(
        g_hWnd,
        &center
    );


    // ============================================
    // カーソルモードから通常モードへ戻る瞬間
    // ============================================

    if (m_cursorVisible)
    {
        g_isCursorVisible =
            false;


        while (
            ShowCursor(
                FALSE
            ) >=
            0)
        {
        }


        m_cursorVisible =
            false;


        SetCursorPos(
            center.x,
            center.y
        );


        m_mouseReady =
            false;


        return;
    }


    // ============================================
    // カメラ回転開始時
    // ============================================

    if (!m_mouseReady)
    {
        SetCursorPos(
            center.x,
            center.y
        );


        m_mouseReady =
            true;


        return;
    }


    // ============================================
    // マウス位置取得
    // ============================================

    POINT mouse;


    if (!GetCursorPos(
        &mouse))
    {
        return;
    }


    const float dx =
        static_cast<float>(
            mouse.x -
            center.x
            );


    const float dy =
        static_cast<float>(
            mouse.y -
            center.y
            );


    const float sensitivity =
        0.15f;


    Quaternion rotation;


    // ============================================
    // 左右回転
    // ============================================

    rotation.SetRotationDeg(
        Vector3::AxisY,
        dx *
        sensitivity
    );


    rotation.Apply(
        m_offset
    );


    // ============================================
    // 上下回転
    // ============================================

    const Vector3 beforePitch =
        m_offset;


    Vector3 right;


    right.Cross(
        Vector3::AxisY,
        m_offset
    );


    right.Normalize();


    rotation.SetRotationDeg(
        right,
        -dy *
        sensitivity
    );


    rotation.Apply(
        m_offset
    );


    // ============================================
    // 上下回転制限
    // ============================================

    Vector3 direction =
        m_offset;


    direction.Normalize();


    if (direction.y <
        -0.5f ||
        direction.y >
        0.9f)
    {
        m_offset =
            beforePitch;
    }


    // ============================================
    // 次フレーム用にカーソルを中央へ戻す
    // ============================================

    SetCursorPos(
        center.x,
        center.y
    );
}