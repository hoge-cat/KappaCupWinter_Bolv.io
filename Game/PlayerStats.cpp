#include "stdafx.h"
#include "PlayerStats.h"


// ============================================================
// Upgrade
// ============================================================

bool PlayerStats::Upgrade(
	StatType type)
{
	return Upgrade(
		type,
		7
	);
}


// ============================================================
// Upgrade
// 最大レベル指定
// ============================================================

bool PlayerStats::Upgrade(
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


// ============================================================
// GetMaxLevel
// ============================================================

int PlayerStats::GetMaxLevel(
	StatType type,
	bool isSmasher) const
{
	if (!isSmasher)
	{
		return 7;
	}


	if (IsSmasherStat(
		type))
	{
		return 10;
	}


	return 0;
}


// ============================================================
// IsSmasherStat
// ============================================================

bool PlayerStats::IsSmasherStat(
	StatType type) const
{
	return
		type ==
		HealthRegen ||

		type ==
		MaxHealth ||

		type ==
		BodyDamage ||

		type ==
		MovementSpeed;
}


// ============================================================
// IsBulletStat
// ============================================================

bool PlayerStats::IsBulletStat(
	StatType type) const
{
	return
		type ==
		BulletSpeed ||

		type ==
		BulletPenetration ||

		type ==
		BulletDamage ||

		type ==
		Reload;
}


// ============================================================
// ResetBulletStats
// ============================================================

int PlayerStats::ResetBulletStats()
{
	const int refundPoints =
		m_levels[
			BulletSpeed
		] +
		m_levels[
			BulletPenetration
		] +
				m_levels[
					BulletDamage
				] +
				m_levels[
					Reload
				];


					m_levels[
						BulletSpeed
					] =
						0;


						m_levels[
							BulletPenetration
						] =
							0;


							m_levels[
								BulletDamage
							] =
								0;


								m_levels[
									Reload
								] =
									0;


									return
										refundPoints;
}