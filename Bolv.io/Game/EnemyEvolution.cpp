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

    if (level <
        15)
    {
        return;
    }


    // ============================================
    // Rammer
    //
    // Lv30までBasic維持。
    // ============================================

    if (buildType ==
        Rammer)
    {
        if (level >=
            30)
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

        m_tankClass =
            Twin;

        break;


    case SniperShooter:

        m_tankClass =
            Sniper;

        break;


    case RapidShooter:

        m_tankClass =
            MachineGun;

        break;


    case FlankShooter:

        m_tankClass =
            FlankGuard;

        break;


    case Balanced:

    default:

        m_tankClass =
            Basic;

        break;
    }


    // ============================================
    // Lv45
    //
    // RapidShooter
    // MachineGun → Sprayer
    // ============================================

    if (level >=
        45 &&
        buildType ==
        RapidShooter)
    {
        m_tankClass =
            Sprayer;
    }
}