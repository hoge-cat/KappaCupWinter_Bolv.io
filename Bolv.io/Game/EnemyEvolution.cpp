
#include "stdafx.h"
#include "EnemyEvolution.h"

#include <cstdlib>

void EnemyEvolution::Configure(
    int level,
    BuildType buildType)
{
    // ============================================
    // BuildTypeが変わった場合は抽選状態をリセット
    // ============================================

    if (m_buildType != buildType)
    {
        m_hasSniperEvolutionRoll = false;
        m_sniperEvolutionResult = Assassin;
    }

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
        // この敵でまだ抽選していない場合のみ実行
        if (!m_hasSniperEvolutionRoll)
        {
            if (rand() % 2 == 0)
            {
                m_sniperEvolutionResult =
                    Assassin;
            }
            else
            {
                m_sniperEvolutionResult =
                    Hunter;
            }

            m_hasSniperEvolutionRoll =
                true;
        }

        // 抽選済みの進化先を維持
        m_tankClass =
            m_sniperEvolutionResult;
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
