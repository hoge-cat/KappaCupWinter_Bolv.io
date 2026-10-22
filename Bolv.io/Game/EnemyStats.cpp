#include "stdafx.h"
#include "EnemyStats.h"


// ============================================================
// Constructor
// ============================================================

EnemyStats::EnemyStats()
{
    Reset();
}


// ============================================================
// Reset
// ============================================================

void EnemyStats::Reset()
{
    for (int i = 0;
        i < StatCount;
        ++i)
    {
        m_levels[i] =
            0;
    }
}


// ============================================================
// Upgrade
// ============================================================

bool EnemyStats::Upgrade(
    StatType type,
    int maxLevel)
{
    if (type <
        0 ||
        type >=
        StatCount)
    {
        return false;
    }


    if (maxLevel <=
        0)
    {
        return false;
    }


    if (m_levels[type] >=
        maxLevel)
    {
        return false;
    }


    ++m_levels[type];


    return true;
}