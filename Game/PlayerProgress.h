#pragma once


class PlayerProgress
{
public:

    // ============================================
    // 経験値
    // ============================================

    void AddExperience(
        int amount
    );


    // ============================================
    // 現在レベル
    // ============================================

    int GetLevel() const
    {
        return m_level;
    }


    // ============================================
    // 現在レベル内の経験値
    // ============================================

    int GetCurrentExperience() const
    {
        return m_currentExperience;
    }


    // ============================================
    // 累計経験値
    // ============================================

    int GetTotalExperience() const
    {
        return m_totalExperience;
    }


    // ============================================
    // 次レベルまでの必要経験値
    // ============================================

    int GetRequiredExperience() const;


    // ============================================
    // 現在使える強化ポイント
    // ============================================

    int GetUpgradePoints() const
    {
        return m_upgradePoints;
    }


    // ============================================
    // 強化ポイントを1消費
    // ============================================

    bool UseUpgradePoint()
    {
        if (m_upgradePoints <=
            0)
        {
            return false;
        }


        --m_upgradePoints;


        return true;
    }


    // ============================================
    // 強化ポイントを返却・追加
    //
    // Smasher進化時の
    // 弾系ステータス返却などに使用。
    // ============================================

    void AddUpgradePoints(
        int amount
    )
    {
        if (amount <=
            0)
        {
            return;
        }


        m_upgradePoints +=
            amount;
    }


private:

    // このレベルで
    // 強化ポイントを獲得するか。
    bool ShouldGiveUpgradePoint(
        int level
    ) const;


private:

    int m_level =
        1;


    int m_currentExperience =
        0;


    int m_totalExperience =
        0;


    int m_upgradePoints =
        0;
};