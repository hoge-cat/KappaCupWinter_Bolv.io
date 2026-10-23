
#include "stdafx.h"
#include "EnemyEvolution.h"

#include <cstdlib>


// ============================================================
// Configure
//
// 敵のレベルと育成方針から進化先を決定する。
// 複数の進化先がある場合は敵ごとに一度だけ抽選する。
// ============================================================

void EnemyEvolution::Configure(
    int level,
    BuildType buildType
)
{
    // ============================================
    // 育成方針が変わったら抽選をリセット
    // ============================================

    if (m_buildType != buildType)
    {
        m_hasSniperEvolutionRoll = false;
        m_sniperEvolutionResult = Assassin;

        m_hasTwinEvolutionRoll = false;
        m_twinEvolutionResult = TripleShot;

        m_hasFlankEvolutionRoll = false;
        m_flankEvolutionResult = QuadTank;
    }

    m_buildType = buildType;
    m_tankClass = Basic;


    // ============================================
    // Lv15未満はBasic
    // ============================================

    if (level < 15)
    {
        return;
    }


    // ============================================
    // Rammer系
    // Lv30以上でSmasher
    // ============================================

    if (buildType == Rammer)
    {
        if (level >= 30)
        {
            m_tankClass = Smasher;
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
    // Twin → Triple Shot / Quad Tank / Twin Flank
    // ============================================

    if (level >= 30 &&
        buildType == TwinShooter)
    {
        if (!m_hasTwinEvolutionRoll)
        {
            const int result = rand() % 3;

            switch (result)
            {
            case 0:
                m_twinEvolutionResult = TripleShot;
                break;

            case 1:
                m_twinEvolutionResult = QuadTank;
                break;

            default:
                m_twinEvolutionResult = TwinFlank;
                break;
            }

            m_hasTwinEvolutionRoll = true;
        }

        m_tankClass = m_twinEvolutionResult;
    }


    // ============================================
    // Lv30進化
    // Flank Guard → Quad Tank / Twin Flank
    //
    // FlankShooter専用の抽選結果を保持する。
    // ============================================

    if (level >= 30 &&
        buildType == FlankShooter)
    {
        if (!m_hasFlankEvolutionRoll)
        {
            if (rand() % 2 == 0)
            {
                m_flankEvolutionResult = QuadTank;
            }
            else
            {
                m_flankEvolutionResult = TwinFlank;
            }

            m_hasFlankEvolutionRoll = true;
        }

        m_tankClass = m_flankEvolutionResult;
    }


    // ============================================
    // Lv30進化
    // Sniper → Assassin / Hunter
    // ============================================

    if (level >= 30 &&
        buildType == SniperShooter)
    {
        if (!m_hasSniperEvolutionRoll)
        {
            if (rand() % 2 == 0)
            {
                m_sniperEvolutionResult = Assassin;
            }
            else
            {
                m_sniperEvolutionResult = Hunter;
            }

            m_hasSniperEvolutionRoll = true;
        }

        m_tankClass = m_sniperEvolutionResult;
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
