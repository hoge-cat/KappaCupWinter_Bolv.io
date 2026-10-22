#pragma once

class Player;

class GameCamera : public IGameObject
{
public:
    bool Start();
    void Update();

private:
    void UpdateMouseRotation();

    // レベルに応じてカメラ距離を変更する。
    void UpdateLevelDistance();

    Player* m_player = nullptr;

    Vector3 m_offset =
        Vector3(
            0.0f,
            300.0f,
            -600.0f
        );

    // Lv1時のカメラ距離。
    float m_baseCameraDistance = 0.0f;

    // カメラ回転用のマウス初期化フラグ。
    bool m_mouseReady = false;

    // 現在カーソルを表示しているか。
    bool m_cursorVisible = false;
};