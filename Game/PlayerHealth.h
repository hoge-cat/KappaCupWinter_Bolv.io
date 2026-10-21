#pragma once


// 自機のHPを管理する。
class PlayerHealth
{
public:

    void TakeDamage(
        int damage
    );


    int GetHP() const
    {
        return m_hp;
    }


    int GetMaxHP() const
    {
        return m_maxHP;
    }


    bool IsDead() const
    {
        return
            m_hp <=
            0;
    }


    // ============================================
    // 最大HP変更
    //
    // 最大HPが増えた分だけ
    // 現在HPも増加させる。
    // ============================================

    void SetMaxHP(
        int maxHP
    )
    {
        if (maxHP <=
            0)
        {
            return;
        }


        const int difference =
            maxHP -
            m_maxHP;


        m_maxHP =
            maxHP;


        if (difference >
            0)
        {
            m_hp +=
                difference;
        }


        if (m_hp >
            m_maxHP)
        {
            m_hp =
                m_maxHP;
        }
    }


    // ============================================
    // 回復
    // ============================================

    void Heal(
        int amount
    )
    {
        if (amount <=
            0 ||
            IsDead())
        {
            return;
        }


        m_hp +=
            amount;


        if (m_hp >
            m_maxHP)
        {
            m_hp =
                m_maxHP;
        }
    }


private:

    // Diep.io Lv1基礎HP。
    int m_maxHP =
        50;


    int m_hp =
        50;
};