
#pragma once

class EnemyEvolution
{
public:

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
        Hunter
    };

    enum BuildType
    {
        Balanced,
        TwinShooter,
        SniperShooter,
        RapidShooter,
        FlankShooter,
        Rammer
    };

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
    // Assassin / Hunter 進化抽選
    //
    // 敵ごとに一度だけ抽選し、
    // レベルアップ後も進化先を維持する。
    // ============================================

    bool m_hasSniperEvolutionRoll =
        false;

    TankClass m_sniperEvolutionResult =
        Assassin;
};
