#include "stdafx.h"
#include "system.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/RenderingEngine.h"
#include "sound/SoundEngine.h"

HWND g_hWnd = NULL;

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