#pragma once

class Player;

class PlayerContact
{
public:
    void Update(Player& player);

private:
    // 接触ダメージの連続発生を防ぐ。
    float m_damageTimer =
        0.0f;

    // ノックバックが毎フレーム大量に
    // 加算されるのを防ぐ。
    float m_knockbackTimer =
        0.0f;
};