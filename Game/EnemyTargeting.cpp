#include "stdafx.h"
#include "EnemyTargeting.h"

#include "Enemy.h"
#include "Player.h"

#include <vector>
#include <cmath>
#include <cstdlib>


namespace
{
    // ============================================
    // ターゲットを探す最大距離
    // ============================================

    const float TARGET_SEARCH_RADIUS =
        900.0f;


    struct TargetCandidate
    {
        bool isPlayer =
            false;


        Enemy* enemy =
            nullptr;


        Vector3 position =
            Vector3::Zero;


        float weight =
            0.0f;
    };
}


// ============================================================
// RandomRange
// ============================================================

float EnemyTargeting::RandomRange(
    float minValue,
    float maxValue)
{
    const float random01 =
        static_cast<float>(
            std::rand()
            ) /
        static_cast<float>(
            RAND_MAX
            );


    return
        minValue +
        (
            maxValue -
            minValue
            ) *
        random01;
}


// ============================================================
// Update
// ============================================================

void EnemyTargeting::Update(
    Enemy& owner,
    Player* player,
    float deltaTime)
{
    m_reselectTimer -=
        deltaTime;


    // ============================================
    // 現在のターゲットがまだ有効か確認
    // ============================================

    const bool targetValid =
        RefreshTarget(
            owner,
            player
        );


    // ============================================
    // 死亡・消滅した場合、
    // または一定時間が経過した場合に再選択
    // ============================================

    if (!targetValid ||
        m_reselectTimer <=
        0.0f)
    {
        SelectTarget(
            owner,
            player
        );


        // ========================================
        // 毎回同じ周期にすると機械的なので
        // 少しランダム化
        // ========================================

        m_reselectTimer =
            RandomRange(
                1.5f,
                3.0f
            );
    }
}


// ============================================================
// RefreshTarget
// ============================================================

bool EnemyTargeting::RefreshTarget(
    Enemy& owner,
    Player* player)
{
    // ========================================================
    // Playerを狙っている
    // ========================================================

    if (m_targetType ==
        TargetPlayer)
    {
        if (player ==
            nullptr)
        {
            return false;
        }


        if (player
            ->GetHealth()
            .IsDead())
        {
            return false;
        }


        m_targetPosition =
            player
            ->GetPosition();


        return true;
    }


    // ========================================================
    // Enemyを狙っている
    //
    // m_targetEnemyをいきなりdereferenceせず、
    // 現在存在するEnemy一覧から同じポインタを探す。
    // ========================================================

    bool found =
        false;


    QueryGOs<Enemy>(
        "enemy",
        [&](Enemy* enemy)
        {
            if (enemy ==
                nullptr)
            {
                return true;
            }


            if (enemy ==
                &owner)
            {
                return true;
            }


            if (enemy !=
                m_targetEnemy)
            {
                return true;
            }


            if (enemy
                ->IsDead())
            {
                return false;
            }


            m_targetPosition =
                enemy
                ->GetPosition();


            found =
                true;


            return false;
        }
    );


    if (!found)
    {
        m_targetEnemy =
            nullptr;
    }


    return found;
}


// ============================================================
// SelectTarget
// ============================================================

void EnemyTargeting::SelectTarget(
    Enemy& owner,
    Player* player)
{
    std::vector<TargetCandidate>
        candidates;


    const Vector3 ownerPosition =
        owner
        .GetPosition();


    // ========================================================
    // Player候補
    // ========================================================

    if (player !=
        nullptr &&
        !player
        ->GetHealth()
        .IsDead())
    {
        const Vector3 playerPosition =
            player
            ->GetPosition();


        const float dx =
            playerPosition.x -
            ownerPosition.x;


        const float dz =
            playerPosition.z -
            ownerPosition.z;


        const float distance =
            std::sqrt(
                dx * dx +
                dz * dz
            );


        if (distance <=
            TARGET_SEARCH_RADIUS)
        {
            const float distanceRate =
                1.0f -
                distance /
                TARGET_SEARCH_RADIUS;


            TargetCandidate candidate;


            candidate.isPlayer =
                true;


            candidate.enemy =
                nullptr;


            candidate.position =
                playerPosition;


            // ====================================
            // Playerだけが集中砲火されすぎないよう
            // Enemy候補より少し低い基本値。
            // ====================================

            candidate.weight =
                0.85f +
                distanceRate *
                1.5f;


            candidates.push_back(
                candidate
            );
        }
    }


    // ========================================================
    // 他Enemy候補
    // ========================================================

    QueryGOs<Enemy>(
        "enemy",
        [&](Enemy* enemy)
        {
            if (enemy ==
                nullptr)
            {
                return true;
            }


            // 自分自身は狙わない。
            if (enemy ==
                &owner)
            {
                return true;
            }


            if (enemy
                ->IsDead())
            {
                return true;
            }


            const Vector3 enemyPosition =
                enemy
                ->GetPosition();


            const float dx =
                enemyPosition.x -
                ownerPosition.x;


            const float dz =
                enemyPosition.z -
                ownerPosition.z;


            const float distance =
                std::sqrt(
                    dx * dx +
                    dz * dz
                );


            if (distance >
                TARGET_SEARCH_RADIUS)
            {
                return true;
            }


            const float distanceRate =
                1.0f -
                distance /
                TARGET_SEARCH_RADIUS;


            TargetCandidate candidate;


            candidate.isPlayer =
                false;


            candidate.enemy =
                enemy;


            candidate.position =
                enemyPosition;


            candidate.weight =
                1.0f +
                distanceRate *
                1.5f;


            candidates.push_back(
                candidate
            );


            return true;
        }
    );


    // ========================================================
    // 誰も近くにいない
    // ========================================================

    if (candidates.empty())
    {
        // ========================================
        // Playerが生きていれば、
        // 遠くてもPlayerを目標にする。
        // ========================================

        if (player !=
            nullptr &&
            !player
            ->GetHealth()
            .IsDead())
        {
            m_targetType =
                TargetPlayer;


            m_targetEnemy =
                nullptr;


            m_targetPosition =
                player
                ->GetPosition();
        }
        else
        {
            // ====================================
            // 誰もいない場合は自分の位置。
            // 原点へ勝手に向かわないようにする。
            // ====================================

            m_targetType =
                TargetPlayer;


            m_targetEnemy =
                nullptr;


            m_targetPosition =
                ownerPosition;
        }


        return;
    }


    // ========================================================
    // Weight合計
    // ========================================================

    float totalWeight =
        0.0f;


    for (const TargetCandidate& candidate :
        candidates)
    {
        totalWeight +=
            candidate.weight;
    }


    // ========================================================
    // 重み付きランダム選択
    // ========================================================

    float randomValue =
        RandomRange(
            0.0f,
            totalWeight
        );


    const TargetCandidate* selected =
        &candidates[0];


    for (const TargetCandidate& candidate :
        candidates)
    {
        randomValue -=
            candidate.weight;


        if (randomValue <=
            0.0f)
        {
            selected =
                &candidate;


            break;
        }
    }


    // ========================================================
    // 選択結果
    // ========================================================

    if (selected->isPlayer)
    {
        m_targetType =
            TargetPlayer;


        m_targetEnemy =
            nullptr;
    }
    else
    {
        m_targetType =
            TargetEnemy;


        m_targetEnemy =
            selected->enemy;
    }


    m_targetPosition =
        selected->position;
}