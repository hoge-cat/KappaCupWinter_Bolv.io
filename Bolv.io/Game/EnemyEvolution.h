
#pragma once

class EnemyEvolution
{
public:

    // ============================================
    // 戦車クラス
    // ============================================

    enum TankClass
    {
        Basic,
        Twin,
        Sniper,
        MachineGun,
        FlankGuard,
        Smasher,
        Sprayer,
        Assassin,
        Hunter,
        TripleShot,
        QuadTank,
        TwinFlank
    };

    // ============================================
    // 敵の育成方針
    // ============================================

    enum BuildType
    {
        Balanced,
        TwinShooter,
        SniperShooter,
        RapidShooter,
        FlankShooter,
        Rammer
    };

    // ============================================
    // 進化設定
    // ============================================

    void Configure(
        int level,
        BuildType buildType
    );

    TankClass GetTankClass() const
    {
        return m_tankClass;
    }

    BuildType GetBuildType() const
    {
        return m_buildType;
    }

private:

    TankClass m_tankClass =
        Basic;

    BuildType m_buildType =
        Balanced;

    // ============================================
    // Sniper系のLv30進化抽選
    // ============================================

    bool m_hasSniperEvolutionRoll =
        false;

    TankClass m_sniperEvolutionResult =
        Assassin;

    // ============================================
    // Twin系のLv30進化抽選
    // ============================================

    bool m_hasTwinEvolutionRoll =
        false;

    TankClass m_twinEvolutionResult =
        TripleShot;


    // ============================================
    // Flank Guard系のLv30進化抽選
    //
    // Quad Tank / Twin Flank
    // ============================================

    bool m_hasFlankEvolutionRoll =
        false;

    TankClass m_flankEvolutionResult =
        QuadTank;
};
