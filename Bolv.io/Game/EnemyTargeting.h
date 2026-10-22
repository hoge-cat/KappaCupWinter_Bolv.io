#pragma once


class Enemy;
class Player;


// ============================================================
// Enemyが現在誰を狙うか管理する。
// ============================================================

class EnemyTargeting
{
public:

    enum TargetType
    {
        TargetPlayer,
        TargetEnemy
    };


    void Update(
        Enemy& owner,
        Player* player,
        float deltaTime
    );


    const Vector3& GetTargetPosition() const
    {
        return m_targetPosition;
    }


    bool IsTargetPlayer() const
    {
        return
            m_targetType ==
            TargetPlayer;
    }


    Enemy* GetTargetEnemy() const
    {
        return m_targetEnemy;
    }


private:

    // ============================================
    // ターゲット選択
    // ============================================

    void SelectTarget(
        Enemy& owner,
        Player* player
    );


    // ============================================
    // 現在のターゲットがまだ存在するか確認
    // ============================================

    bool RefreshTarget(
        Enemy& owner,
        Player* player
    );


    float RandomRange(
        float minValue,
        float maxValue
    );


private:

    TargetType m_targetType =
        TargetPlayer;


    // ============================================
    // Enemyターゲット
    // ============================================

    Enemy* m_targetEnemy =
        nullptr;


    // ============================================
    // 現在狙っている位置
    // ============================================

    Vector3 m_targetPosition =
        Vector3::Zero;


    // ============================================
    // ターゲット再選択までの時間
    // ============================================

    float m_reselectTimer =
        0.0f;
};