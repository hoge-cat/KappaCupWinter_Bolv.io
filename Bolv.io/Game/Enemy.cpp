#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

#include <cmath>


// ============================================================
// Start
// ============================================================

bool Enemy::Start()
{
	m_player =
		FindGO<Player>(
			"player"
		);


	return true;
}


// ============================================================
// Configure
// ============================================================

void Enemy::Configure(
	int level,
	EnemyEvolution::BuildType buildType)
{
	// ============================================
	// Level
	// ============================================

	if (level <
		1)
	{
		level =
			1;
	}


	if (level >
		45)
	{
		level =
			45;
	}


	m_level =
		level;


	// ============================================
	// Evolution
	// ============================================

	m_evolution.Configure(
		m_level,
		buildType
	);


	// ============================================
	// Stats
	// ============================================

	m_stats.Reset();


	BuildStats();


	// ============================================
	// HP
	// ============================================

	m_maxHP =
		m_stats
		.GetMaxHealthValue(
			m_level
		);


	m_hp =
		m_maxHP;


	// ============================================
	// AI
	// ============================================

	switch (
		m_evolution
		.GetBuildType())
	{
	case EnemyEvolution::Rammer:

		m_moveType =
			EnemyMoveType::Chase;

		break;


	case EnemyEvolution::SniperShooter:

		m_moveType =
			EnemyMoveType::KeepDistance;

		break;


	case EnemyEvolution::RapidShooter:

		m_moveType =
			EnemyMoveType::KeepDistance;

		break;


	case EnemyEvolution::TwinShooter:

		m_moveType =
			EnemyMoveType::Orbit;

		break;


	case EnemyEvolution::FlankShooter:

		m_moveType =
			EnemyMoveType::Orbit;

		break;


	case EnemyEvolution::Balanced:

	default:

		m_moveType =
			EnemyMoveType::KeepDistance;

		break;
	}
}


// ============================================================
// Upgrade Point Count
// ============================================================

int Enemy::CalculateUpgradePointCount() const
{
	int points =
		0;


	// Lv2～28
	for (int level = 2;
		level <= m_level &&
		level <= 28;
		++level)
	{
		++points;
	}


	// Lv30 / 33 / 36 / 39 / 42 / 45
	for (int level = 30;
		level <= m_level &&
		level <= 45;
		level += 3)
	{
		++points;
	}


	return points;
}


// ============================================================
// UpgradePriority
// ============================================================

void Enemy::UpgradePriority(
	EnemyStats::StatType type,
	int maxLevel,
	int& remainingPoints)
{
	while (
		remainingPoints >
		0)
	{
		if (!m_stats
			.Upgrade(
				type,
				maxLevel
			))
		{
			break;
		}


		--remainingPoints;
	}
}


// ============================================================
// BuildStats
// ============================================================

void Enemy::BuildStats()
{
	int points =
		CalculateUpgradePointCount();


	const EnemyEvolution::BuildType buildType =
		m_evolution
		.GetBuildType();


	// ========================================================
	// Rammer / Smasher
	// ========================================================

	if (buildType ==
		EnemyEvolution::Rammer)
	{
		const bool isSmasher =
			m_evolution
			.GetTankClass() ==
			EnemyEvolution::Smasher;


		const int maxLevel =
			isSmasher ?
			10 :
			7;


		UpgradePriority(
			EnemyStats::MaxHealth,
			maxLevel,
			points
		);


		UpgradePriority(
			EnemyStats::BodyDamage,
			maxLevel,
			points
		);


		UpgradePriority(
			EnemyStats::MovementSpeed,
			maxLevel,
			points
		);


		UpgradePriority(
			EnemyStats::HealthRegen,
			maxLevel,
			points
		);


		return;
	}


	// ========================================================
	// Sniper
	// ========================================================

	if (buildType ==
		EnemyEvolution::SniperShooter)
	{
		UpgradePriority(
			EnemyStats::BulletDamage,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletPenetration,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::Reload,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MovementSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MaxHealth,
			7,
			points
		);


		return;
	}


	// ========================================================
	// Rapid / Machine Gun / Sprayer
	// ========================================================

	if (buildType ==
		EnemyEvolution::RapidShooter)
	{
		UpgradePriority(
			EnemyStats::Reload,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletDamage,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletPenetration,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MovementSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MaxHealth,
			7,
			points
		);


		return;
	}


	// ========================================================
	// Twin
	// ========================================================

	if (buildType ==
		EnemyEvolution::TwinShooter)
	{
		UpgradePriority(
			EnemyStats::Reload,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletDamage,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletPenetration,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MovementSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MaxHealth,
			7,
			points
		);


		return;
	}


	// ========================================================
	// Flank Guard
	// ========================================================

	if (buildType ==
		EnemyEvolution::FlankShooter)
	{
		UpgradePriority(
			EnemyStats::MovementSpeed,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::Reload,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletDamage,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::BulletPenetration,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::MaxHealth,
			7,
			points
		);


		UpgradePriority(
			EnemyStats::HealthRegen,
			7,
			points
		);


		return;
	}


	// ========================================================
	// Balanced
	// ========================================================

	const EnemyStats::StatType balancedOrder[] =
	{
		EnemyStats::BulletDamage,
		EnemyStats::MaxHealth,
		EnemyStats::Reload,
		EnemyStats::MovementSpeed,
		EnemyStats::BulletPenetration,
		EnemyStats::HealthRegen,
		EnemyStats::BulletSpeed,
		EnemyStats::BodyDamage
	};


	const int orderCount =
		sizeof(
			balancedOrder
			) /
		sizeof(
			balancedOrder[0]
			);


	int index =
		0;


	while (points >
		0)
	{
		const EnemyStats::StatType type =
			balancedOrder[
				index %
					orderCount
			];


		if (m_stats
			.Upgrade(
				type,
				7
			))
		{
			--points;
		}


		++index;
	}
}


// ============================================================
// GetLevelScale
// ============================================================

float Enemy::GetLevelScale() const
{
	return
		std::pow(
			1.01f,
			static_cast<float>(
				m_level -
				1
				)
		);
}


// ============================================================
// Update
// ============================================================

void Enemy::Update()
{
	if (IsDead())
	{
		return;
	}


	const float deltaTime =
		g_gameTime
		->GetFrameDeltaTime();


	// ============================================
	// ターゲット選択
	// ============================================

	m_targeting.Update(
		*this,
		m_player,
		deltaTime
	);


	// ============================================
	// 移動
	// ============================================

	Move();


	// ============================================
	// Contact
	//
	// Player
	// Enemy
	// Shape
	//
	// すべてEnemyContact側で処理。
	// ============================================

	m_contact.Update(
		*this,
		m_position,
		m_player
	);


	if (IsDead())
	{
		return;
	}


	// ============================================
	// Smasherは射撃しない
	// ============================================

	if (m_evolution
		.GetTankClass() ==
		EnemyEvolution::Smasher)
	{
		return;
	}


	// ============================================
	// Chase型は射撃しない
	// ============================================

	if (m_moveType ==
		EnemyMoveType::Chase)
	{
		return;
	}


	// ============================================
	// 射撃
	// ============================================

	m_weapon.Update(
		*this,
		m_position,
		m_direction
	);
}


// ============================================================
// Move
// ============================================================

void Enemy::Move()
{
	// ============================================
	// Movement Speed
	// ============================================

	const float movementSpeedMultiplier =
		m_stats
		.GetMovementSpeedMultiplier(
			m_level
		);


	// ============================================
	// 基本照準精度
	//
	// Lvが高いほど正確。
	// ============================================

	float maxAimErrorDeg =
		10.0f -
		static_cast<float>(
			m_level -
			1
			) *
		(
			8.0f /
			44.0f
			);


	if (maxAimErrorDeg <
		2.0f)
	{
		maxAimErrorDeg =
			2.0f;
	}


	// ============================================
	// 照準ズレ変更周期
	// ============================================

	float aimChangeInterval =
		0.65f;


	const EnemyEvolution::TankClass tankClass =
		m_evolution
		.GetTankClass();


	// ============================================
	// Sniper
	// ============================================

	if (tankClass ==
		EnemyEvolution::Sniper)
	{
		maxAimErrorDeg *=
			0.55f;


		aimChangeInterval =
			0.85f;
	}



	// ============================================
	// Assassin
	// Sniperより高精度な遠距離射撃
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::Assassin)
	{
		maxAimErrorDeg *=
			0.35f;

		aimChangeInterval =
			1.00f;
	}

	// ============================================
	// Hunter
	// 2連射を活かす高精度な照準
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::Hunter)
	{
		maxAimErrorDeg *=
			0.50f;

		aimChangeInterval =
			0.85f;
	}


	// ============================================
	// Machine Gun
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::MachineGun)
	{
		maxAimErrorDeg *=
			1.20f;


		aimChangeInterval =
			0.55f;
	}


	// ============================================
	// Flank Guard
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::FlankGuard)
	{
		maxAimErrorDeg *=
			1.15f;


		aimChangeInterval =
			0.60f;
	}


	// ============================================
	// Sprayer
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::Sprayer)
	{
		maxAimErrorDeg *=
			1.25f;


		aimChangeInterval =
			0.50f;
	}


	// ============================================
	// Twin
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::Twin)
	{
		maxAimErrorDeg *=
			1.05f;


		aimChangeInterval =
			0.60f;
	}


	// ============================================
	// Smasher
	// ============================================

	else if (
		tankClass ==
		EnemyEvolution::Smasher)
	{
		maxAimErrorDeg =
			0.0f;


		aimChangeInterval =
			1.0f;
	}


	// ============================================
	// Movement更新
	//
	// 重要：
	// Player固定ではなく、
	// EnemyTargetingが選んだ相手へ向かう。
	// ============================================

	m_movement.Update(
		m_position,
		m_direction,
		m_targeting
		.GetTargetPosition(),
		m_moveType,
		g_gameTime
		->GetFrameDeltaTime(),
		movementSpeedMultiplier,
		maxAimErrorDeg,
		aimChangeInterval
	);
}


// ============================================================
// Render
// ============================================================

void Enemy::Render(
	RenderContext& rc)
{
	(void)rc;
}


// ============================================================
// TakeDamage
// ============================================================

void Enemy::TakeDamage(
	int damage,
	bool rewardPlayer)
{
	if (damage <=
		0 ||
		IsDead())
	{
		return;
	}


	m_hp -=
		damage;


	if (m_hp >
		0)
	{
		return;
	}


	m_hp =
		0;


	if (m_player !=
		nullptr &&
		rewardPlayer)
	{
		m_player
			->AddExperience(
				30
			);


		m_player
			->AddScore(
				300
			);
	}
}


// ============================================================
// IsHit
// ============================================================

bool Enemy::IsHit(
	const Vector3& start,
	const Vector3& end,
	float bulletRadius
) const
{
	if (IsDead())
	{
		return false;
	}


	Vector3 center =
		m_position;


	center.y +=
		30.0f *
		GetLevelScale();


	const Vector3 movement =
		end -
		start;


	const Vector3 toCenter =
		center -
		start;


	const float lengthSquared =
		movement.x *
		movement.x +
		movement.y *
		movement.y +
		movement.z *
		movement.z;


	float t =
		0.0f;


	if (lengthSquared >
		0.0001f)
	{
		t =
			(
				toCenter.x *
				movement.x +
				toCenter.y *
				movement.y +
				toCenter.z *
				movement.z
				) /
			lengthSquared;


		if (t <
			0.0f)
		{
			t =
				0.0f;
		}


		if (t >
			1.0f)
		{
			t =
				1.0f;
		}
	}


	const Vector3 closest =
		start +
		movement *
		t;


	const Vector3 difference =
		center -
		closest;


	const float distanceSquared =
		difference.x *
		difference.x +
		difference.y *
		difference.y +
		difference.z *
		difference.z;


	const float hitRadius =
		GetCollisionRadius() +
		bulletRadius;


	return
		distanceSquared <=
		hitRadius *
		hitRadius;
}