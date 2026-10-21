#include "stdafx.h"
#include "PlayerProgress.h"
#include <cstdio>

int PlayerProgress::GetRequiredExperience() const
{
    // 最大レベルは45。
    if (m_level >= 45)
    {
        return 0;
    }

    // 現在はテストしやすい値。
    //
    // Lv1 → 10
    // Lv2 → 15
    // Lv3 → 20
    // Lv4 → 25
    return 10 + (m_level - 1) * 5;
}

bool PlayerProgress::ShouldGiveUpgradePoint(int level) const
{
    // Diep.io風。
    //
    // Lv2～Lv28までは毎レベル1ポイント。
    if (level >= 2 && level <= 28)
    {
        return true;
    }

    // Lv30以降は
    //
    // Lv30
    // Lv33
    // Lv36
    // Lv39
    // Lv42
    // Lv45
    //
    // で1ポイント。
    if (level >= 30 &&
        level <= 45 &&
        (level - 30) % 3 == 0)
    {
        return true;
    }

    return false;
}

void PlayerProgress::AddExperience(int amount)
{
    // 0以下の経験値は無視。
    if (amount <= 0)
    {
        return;
    }

    // 累計経験値。
    m_totalExperience += amount;

    // Lv45ならこれ以上レベルアップしない。
    if (m_level >= 45)
    {
        return;
    }

    // 現在の経験値に追加。
    m_currentExperience += amount;

    // 一気に大量の経験値を獲得した場合でも
    // 複数レベル上がれるようにする。
    while (m_level < 45)
    {
        const int requiredExperience =
            GetRequiredExperience();

        // 必要経験値に届いていなければ終了。
        if (m_currentExperience < requiredExperience)
        {
            break;
        }

        // 必要経験値を引く。
        //
        // 例：
        //
        // 17 / 10
        //
        // ↓
        //
        // レベルアップ後
        //
        // 7 EXP
        //
        // が残る。
        m_currentExperience -= requiredExperience;

        // レベルアップ。
        ++m_level;

        // 強化ポイントをもらえるレベルなら加算。
        if (ShouldGiveUpgradePoint(m_level))
        {
            ++m_upgradePoints;
        }

        // デバッグ表示。
        char message[128];

        sprintf_s(
            message,
            "Level Up! Lv.%d Points:%d\n",
            m_level,
            m_upgradePoints
        );

        OutputDebugStringA(message);
    }

    // 最大レベルに到達。
    if (m_level >= 45)
    {
        m_currentExperience = 0;

        OutputDebugStringA(
            "MAX LEVEL Lv.45\n"
        );

        return;
    }

    // 現在の状態をデバッグ表示。
    char message[128];

    sprintf_s(
        message,
        "Lv.%d EXP:%d/%d Points:%d\n",
        m_level,
        m_currentExperience,
        GetRequiredExperience(),
        m_upgradePoints
    );

    OutputDebugStringA(message);
}