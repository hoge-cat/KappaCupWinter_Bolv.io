#pragma once

class GraphicsEngine;

// ウィンドウハンドル。
extern HWND g_hWnd;

// カーソル表示状態。
// true  = 表示
// false = 非表示
extern bool g_isCursorVisible;

// ゲームの初期化。
void InitGame(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow,
    const TCHAR* appName
);

// ウィンドウメッセージをディスパッチ。
// falseが返ってきたらゲーム終了。
bool DispatchWindowMessage();