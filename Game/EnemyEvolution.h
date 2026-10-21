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
        Sprayer
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
};