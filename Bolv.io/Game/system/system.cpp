#include "stdafx.h"
#include "system.h"
#include "../PlayerUpgradeUI.h"
#include "../GameSettings.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/RenderingEngine.h"
#include "sound/SoundEngine.h"

HWND g_hWnd = NULL;

namespace
{
    bool isFullscreen = true;
    RECT windowedRect = {};
}
bool IsGameFullscreen() { return isFullscreen; }
bool SetGameFullscreen(bool fullscreen)
{
    if (!g_hWnd) { return false; }
    MONITORINFO monitor = { sizeof(MONITORINFO) };
    if (!GetMonitorInfoW(MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTONEAREST), &monitor)) { return false; }
    RECT oldRect;
    if (!GetWindowRect(g_hWnd, &oldRect)) { return false; }
    const LONG_PTR oldStyle = GetWindowLongPtrW(g_hWnd, GWL_STYLE);
    constexpr LONG_PTR WINDOW_STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT target = monitor.rcMonitor;
    const LONG_PTR style = (fullscreen ? WS_POPUP : WINDOW_STYLE) | (oldStyle & WS_VISIBLE);
    if (!fullscreen)
    {
        if (!isFullscreen) { return true; }
        if (windowedRect.right > windowedRect.left &&
            windowedRect.right - windowedRect.left <= monitor.rcWork.right - monitor.rcWork.left &&
            windowedRect.bottom - windowedRect.top <= monitor.rcWork.bottom - monitor.rcWork.top)
        {
            target = windowedRect;
            // The saved position may belong to a monitor that has been disconnected.
            const int width = target.right - target.left, height = target.bottom - target.top;
            int x = target.left, y = target.top;
            if (x < monitor.rcWork.left) { x = monitor.rcWork.left; }
            if (y < monitor.rcWork.top) { y = monitor.rcWork.top; }
            if (x + width > monitor.rcWork.right) { x = monitor.rcWork.right - width; }
            if (y + height > monitor.rcWork.bottom) { y = monitor.rcWork.bottom - height; }
            target = { x, y, x + width, y + height };
        }
        else
        {
            const int availableWidth = monitor.rcWork.right - monitor.rcWork.left;
            const int availableHeight = monitor.rcWork.bottom - monitor.rcWork.top;
            int width = 1280, height = 720;
            if (width > availableWidth - 40) { width = availableWidth - 40; }
            if (height > availableHeight - 80) { height = availableHeight - 80; }
            if (width < 320) { width = 320; }
            if (height < 180) { height = 180; }
            if (width * 9 > height * 16) { width = height * 16 / 9; }
            else { height = width * 9 / 16; }
            target = { 0, 0, width, height };
            if (!AdjustWindowRectEx(&target, static_cast<DWORD>(style), FALSE, 0)) { return false; }
            width = target.right - target.left; height = target.bottom - target.top;
            const int x = monitor.rcWork.left + (availableWidth - width) / 2;
            const int y = monitor.rcWork.top + (availableHeight - height) / 2;
            target = { x, y, x + width, y + height };
        }
    }
    SetLastError(0);
    if (!SetWindowLongPtrW(g_hWnd, GWL_STYLE, style) && GetLastError() != 0) { return false; }
    if (!SetWindowPos(g_hWnd, HWND_NOTOPMOST, target.left, target.top,
        target.right - target.left, target.bottom - target.top, SWP_FRAMECHANGED | SWP_NOACTIVATE))
    {
        SetWindowLongPtrW(g_hWnd, GWL_STYLE, oldStyle);
        SetWindowPos(g_hWnd, nullptr, oldRect.left, oldRect.top, oldRect.right - oldRect.left,
            oldRect.bottom - oldRect.top, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE);
        return false;
    }
    if (fullscreen && !isFullscreen) { windowedRect = oldRect; }
    isFullscreen = fullscreen;
    ClipCursor(nullptr);
    return true;
}


// カーソルを表示するかどうか。
// GameCamera.cpp から切り替える。
bool g_isCursorVisible = false;


///////////////////////////////////////////////////////////////////
// メッセージプロシージャ。
///////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    PlayerUpgradeUI::HandleWindowMessage(msg, wParam, lParam);
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SETCURSOR:
        // ゲーム画面内のカーソル。
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (g_isCursorVisible)
            {
                // カーソル表示中なら矢印。
                SetCursor(
                    LoadCursor(
                        NULL,
                        IDC_ARROW
                    )
                );
            }
            else
            {
                // 通常時はカーソル非表示。
                SetCursor(NULL);
            }

            return TRUE;
        }

        return DefWindowProc(
            hWnd,
            msg,
            wParam,
            lParam
        );

    default:
        return DefWindowProc(
            hWnd,
            msg,
            wParam,
            lParam
        );
    }

    return 0;
}


///////////////////////////////////////////////////////////////////
// ウィンドウの初期化。
///////////////////////////////////////////////////////////////////
void InitWindow(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow,
    const TCHAR* appName)
{
    WNDCLASSEX wc = {};

    wc.cbSize =
        sizeof(WNDCLASSEX);

    wc.style =
        CS_CLASSDC;

    wc.lpfnWndProc =
        MsgProc;

    wc.hInstance =
        hInstance;

    // カーソルそのものは矢印を登録しておく。
    wc.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    wc.lpszClassName =
        appName;

    RegisterClassEx(
        &wc
    );

    // メイン画面のサイズを取得。
    const int screenWidth =
        GetSystemMetrics(
            SM_CXSCREEN
        );

    const int screenHeight =
        GetSystemMetrics(
            SM_CYSCREEN
        );

    // 枠とタイトルバーのないウィンドウ。
    g_hWnd =
        CreateWindow(
            appName,
            appName,
            WS_POPUP,
            0,
            0,
            screenWidth,
            screenHeight,
            NULL,
            NULL,
            hInstance,
            NULL
        );

    SetGameFullscreen(GameSettings::GetFullscreen());

    ShowWindow(
        g_hWnd,
        SW_SHOW
    );

    UpdateWindow(
        g_hWnd
    );
}


///////////////////////////////////////////////////////////////////
// ゲームの初期化。
///////////////////////////////////////////////////////////////////
void InitGame(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow,
    const TCHAR* appName)
{
    GameSettings::Load();
    InitWindow(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nCmdShow,
        appName
    );

    K2Engine::InitData initData;

    initData.isSoftShadow =
        true;

    initData.frameBufferWidth =
        GetSystemMetrics(
            SM_CXSCREEN
        );

    initData.frameBufferHeight =
        GetSystemMetrics(
            SM_CYSCREEN
        );

    initData.hwnd =
        g_hWnd;

    K2Engine::CreateInstance(
        initData
    );
    GameSettings::ApplyAudio();
}


///////////////////////////////////////////////////////////////////
// ウィンドウメッセージをディスパッチ。
///////////////////////////////////////////////////////////////////
bool DispatchWindowMessage()
{
    MSG msg = { 0 };

    while (
        WM_QUIT !=
        msg.message)
    {
        if (PeekMessage(
            &msg,
            NULL,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(
                &msg
            );

            DispatchMessage(
                &msg
            );
        }
        else
        {
            break;
        }
    }

    return
        msg.message !=
        WM_QUIT;
}