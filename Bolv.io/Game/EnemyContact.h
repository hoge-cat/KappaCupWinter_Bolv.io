#pragma once


class Enemy;
class Player;


class EnemyContact
{
public:

    void Update(
        Enemy& enemy,
        const Vector3& enemyPosition,
        Player* player
    );


private:

    // ============================================
    // Playerとの接触
    // ============================================

    void UpdatePlayerContact(
        Enemy& enemy,
        const Vector3& enemyCenter,
        Player* player
    );


    // ============================================
    // Enemy同士の接触
    // ============================================

    void UpdateEnemyContact(
        Enemy& enemy,
        const Vector3& enemyCenter
    );


    // ============================================
    // Shapeとの接触
    // ============================================

    void UpdateShapeContact(
        Enemy& enemy,
        const Vector3& enemyCenter
    );


private:

    // ============================================
    // Body Damage判定間隔
    // ============================================

    float m_damageTimer =
        0.0f;


    // ============================================
    // ノックバック判定間隔
    // ============================================

    float m_knockbackTimer =
        0.0f;
};