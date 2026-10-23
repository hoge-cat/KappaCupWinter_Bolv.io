
#include "stdafx.h"
#include "EnemyEvolution.h"

void EnemyEvolution::Configure(
    int level,
    BuildType buildType)
{
    m_buildType =
        buildType;

    m_tankClass =
        Basic;

    // ============================================
    // Lv1～14
    // ============================================

    if (level < 15)
    {
        return;
    }

    // ============================================
    // Rammer
    // Lv30からSmasher
    // ============================================

    if (buildType == Rammer)
    {
        if (level >= 30)
        {
            m_tankClass =
                Smasher;
        }

        return;
    }

    // ============================================
    // Lv15進化
    // ============================================

    switch (buildType)
    {
    case TwinShooter:
        m_tankClass = Twin;
        break;

    case SniperShooter:
        m_tankClass = Sniper;
        break;

    case RapidShooter:
        m_tankClass = MachineGun;
        break;

    case FlankShooter:
        m_tankClass = FlankGuard;
        break;

    case Balanced:
    default:
        m_tankClass = Basic;
        break;
    }

    // ============================================
    // Lv30進化
    // Sniper → Assassin / Hunter
    // ============================================

    if (level >= 30 &&
        buildType == SniperShooter)
    {
        // レベルによって進化先を分ける
        if (level % 2 == 0)
        {
            m_tankClass = Assassin;
        }
        else
        {
            m_tankClass = Hunter;
        }
    }

    // ============================================
    // Lv45進化
    // Machine Gun → Sprayer
    // ============================================

    if (level >= 45 &&
        buildType == RapidShooter)
    {
        m_tankClass = Sprayer;
    }
}
