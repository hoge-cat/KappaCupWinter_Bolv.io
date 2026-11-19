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
    bool windowedMaximized = false;
}

namespace
{
    bool GetWindowFrame(HWND window, int& width, int& height)
    {
        RECT frame = {};
        if (!AdjustWindowRectEx(&frame,
            static_cast<DWORD>(GetWindowLongPtrW(window, GWL_STYLE)), FALSE, 0)) { return false; }
        width = frame.right - frame.left;
        height = frame.bottom - frame.top;
        return true;
    }
    void ConstrainWindowAspect(HWND window, RECT& rect, UINT edge)
    {
        int frameWidth, frameHeight;
        if (!GetWindowFrame(window, frameWidth, frameHeight)) { return; }
        int width = rect.right - rect.left - frameWidth;
        int height = rect.bottom - rect.top - frameHeight;
        bool useHeight = edge == WMSZ_TOP || edge == WMSZ_BOTTOM;
        if (edge != WMSZ_LEFT && edge != WMSZ_RIGHT && !useHeight)
        {
            RECT old;
            if (GetWindowRect(window, &old))
            {
                const int deltaWidth = width - (old.right - old.left - frameWidth);
                const int deltaHeight = height - (old.bottom - old.top - frameHeight);
                const long long dx = deltaWidth < 0 ? -static_cast<long long>(deltaWidth) : deltaWidth;
                const long long dy = deltaHeight < 0 ? -static_cast<long long>(deltaHeight) : deltaHeight;
                useHeight = dy * 16 > dx * 9;
            }
        }
        if (useHeight)
        {
            if (height < 360) { height = 360; }
            width = static_cast<int>((static_cast<long long>(height) * 16 + 4) / 9);
        }
        else
        {
            if (width < 640) { width = 640; }
            height = static_cast<int>((static_cast<long long>(width) * 9 + 8) / 16);
        }
        const bool left = edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT;
        const bool top = edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT;
        if (left) { rect.left = rect.right - width - frameWidth; }
        else { rect.right = rect.left + width + frameWidth; }
        if (top) { rect.top = rect.bottom - height - frameHeight; }
        else { rect.bottom = rect.top + height + frameHeight; }
    }
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
    constexpr LONG_PTR WINDOW_STYLE = WS_OVERLAPPEDWINDOW;
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
    const bool wasMaximized = !isFullscreen && IsZoomed(g_hWnd);
    RECT normalRect = oldRect;
    if (fullscreen && wasMaximized)
    {
        // Capture the normal window rectangle before removing its frame.
        ShowWindow(g_hWnd, SW_RESTORE);
        if (!GetWindowRect(g_hWnd, &normalRect))
        {
            ShowWindow(g_hWnd, SW_MAXIMIZE);
            return false;
        }
    }
    SetLastError(0);
    if (!SetWindowLongPtrW(g_hWnd, GWL_STYLE, style) && GetLastError() != 0)
    {
        if (wasMaximized) { ShowWindow(g_hWnd, SW_MAXIMIZE); }
        return false;
    }
    if (!SetWindowPos(g_hWnd, HWND_NOTOPMOST, target.left, target.top,
        target.right - target.left, target.bottom - target.top, SWP_FRAMECHANGED | SWP_NOACTIVATE))
    {
        SetWindowLongPtrW(g_hWnd, GWL_STYLE, oldStyle);
        SetWindowPos(g_hWnd, nullptr, oldRect.left, oldRect.top, oldRect.right - oldRect.left,
            oldRect.bottom - oldRect.top, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE);
        if (wasMaximized) { ShowWindow(g_hWnd, SW_MAXIMIZE); }
        return false;
    }
    if (fullscreen && !isFullscreen)
    {
        windowedRect = normalRect;
        windowedMaximized = wasMaximized;
    }
    if (!fullscreen && windowedMaximized) { ShowWindow(g_hWnd, SW_MAXIMIZE); }
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
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wParam == VK_F11)
        {
            // Bit 30 is set for repeated key-down messages while the key is held.
            if ((lParam & (static_cast<LPARAM>(1) << 30)) == 0)
            {
                if (GameSettings::SetFullscreen(!IsGameFullscreen())) { GameSettings::Save(); }
            }
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);

    case WM_SIZING:
        if (!IsGameFullscreen())
        {
            ConstrainWindowAspect(hWnd, *reinterpret_cast<RECT*>(lParam), static_cast<UINT>(wParam));
            return TRUE;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);

    case WM_GETMINMAXINFO:
        if (!IsGameFullscreen())
        {
            RECT minimum = { 0, 0, 640, 360 };
            if (AdjustWindowRectEx(&minimum,
                static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE)), FALSE, 0))
            {
                auto* info = reinterpret_cast<MINMAXINFO*>(lParam);
                info->ptMinTrackSize.x = minimum.right - minimum.left;
                info->ptMinTrackSize.y = minimum.bottom - minimum.top;
                MONITORINFO monitor = { sizeof(MONITORINFO) };
                if (GetMonitorInfoW(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &monitor))
                {
                    const int frameWidth = minimum.right - minimum.left - 640;
                    const int frameHeight = minimum.bottom - minimum.top - 360;
                    int width = monitor.rcWork.right - monitor.rcWork.left - frameWidth;
                    int height = monitor.rcWork.bottom - monitor.rcWork.top - frameHeight;
                    if (static_cast<long long>(width) * 9 > static_cast<long long>(height) * 16)
                    {
                        width = height * 16 / 9;
                    }
                    else { height = width * 9 / 16; }
                    info->ptMaxSize = { width + frameWidth, height + frameHeight };
                    info->ptMaxPosition = {
                        monitor.rcWork.left - monitor.rcMonitor.left +
                            (monitor.rcWork.right - monitor.rcWork.left - info->ptMaxSize.x) / 2,
                        monitor.rcWork.top - monitor.rcMonitor.top +
                            (monitor.rcWork.bottom - monitor.rcWork.top - info->ptMaxSize.y) / 2
                    };
                }
            }
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);

    case WM_ENTERSIZEMOVE:
        ClipCursor(nullptr);
        return 0;

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