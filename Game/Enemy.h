#pragma once

#include "EnemyWeapon.h"
#include "EnemyMovement.h"
#include "EnemyContact.h"
#include "EnemyStats.h"
#include "EnemyEvolution.h"
#include "EnemyTargeting.h"


class Player;


class Enemy : public IGameObject
{
public:

	bool Start();

	void Update();

	void Render(
		RenderContext& rc
	);


	// ============================================
	// 初期設定
	// ============================================

	void Configure(
		int level,
		EnemyEvolution::BuildType buildType
	);


	// ============================================
	// ダメージ
	// ============================================

	void TakeDamage(
		int damage,
		bool rewardPlayer = true
	);


	// ============================================
	// HP
	// ============================================

	int GetHP() const
	{
		return m_hp;
	}


	int GetMaxHP() const
	{
		return m_maxHP;
	}


	bool IsDead() const
	{
		return
			m_hp <= 0;
	}


	// ============================================
	// Level
	// ============================================

	int GetLevel() const
	{
		return m_level;
	}


	// ============================================
	// Evolution
	// ============================================

	EnemyEvolution::TankClass GetTankClass() const
	{
		return
			m_evolution
			.GetTankClass();
	}


	EnemyEvolution::BuildType GetBuildType() const
	{
		return
			m_evolution
			.GetBuildType();
	}


	// ============================================
	// Stats
	// ============================================

	const EnemyStats& GetStats() const
	{
		return m_stats;
	}


	// ============================================
	// Body Damage
	// ============================================

	int GetBodyDamageVsTank() const
	{
		return
			m_stats
			.GetBodyDamageVsTank();
	}


	int GetBodyDamageVsShape() const
	{
		return
			m_stats
			.GetBodyDamageVsShape();
	}


	int GetBodyDamageVsProjectile() const
	{
		return
			m_stats
			.GetBodyDamageVsProjectile();
	}


	// ============================================
	// Bullet Stats
	// ============================================

	int GetBulletDamage() const
	{
		return
			m_stats
			.GetBulletDamageValue();
	}


	float GetBulletSpeed() const
	{
		return
			m_stats
			.GetBulletSpeedValue();
	}


	int GetBulletPenetration() const
	{
		return
			m_stats
			.GetBulletPenetrationValue();
	}


	int GetBulletCollisionDamage() const
	{
		return
			m_stats
			.GetBulletCollisionDamageValue();
	}


	float GetReloadInterval() const
	{
		return
			m_stats
			.GetReloadInterval();
	}


	// ============================================
	// Level Scale
	// ============================================

	float GetLevelScale() const;


	float GetCollisionRadius() const
	{
		return
			30.0f *
			GetLevelScale();
	}


	// ============================================
	// Knockback
	// ============================================

	void AddKnockback(
		const Vector3& direction,
		float power
	)
	{
		m_movement
			.AddKnockback(
				direction,
				power
			);
	}


	// ============================================
	// Hit
	// ============================================

	bool IsHit(
		const Vector3& start,
		const Vector3& end,
		float bulletRadius
	) const;


	// ============================================
	// Position
	// ============================================

	void SetPosition(
		const Vector3& position
	)
	{
		m_position =
			position;
	}


	const Vector3& GetPosition() const
	{
		return m_position;
	}


	// ============================================
	// Movement
	// ============================================

	void SetMoveType(
		EnemyMoveType type
	)
	{
		m_moveType =
			type;
	}


	EnemyMoveType GetMoveType() const
	{
		return m_moveType;
	}


	void SetOrbitDirection(
		bool clockwise
	)
	{
		m_movement
			.SetOrbitDirection(
				clockwise
			);
	}


	const Vector3& GetDirection() const
	{
		return m_direction;
	}


private:

	void Move();


	// ============================================
	// ステータスポイント配分
	// ============================================

	void BuildStats();


	int CalculateUpgradePointCount() const;


	void UpgradePriority(
		EnemyStats::StatType type,
		int maxLevel,
		int& remainingPoints
	);


private:

	Player* m_player =
		nullptr;


	Vector3 m_position =
		Vector3(
			300.0f,
			0.0f,
			500.0f
		);


	Vector3 m_direction =
		Vector3(
			0.0f,
			0.0f,
			-1.0f
		);


	int m_level =
		1;


	EnemyStats m_stats;


	EnemyEvolution m_evolution;


	EnemyMovement m_movement;

	EnemyMoveType m_moveType =
		EnemyMoveType::Chase;


	// ============================================
	// ターゲット選択
	// ============================================

	EnemyTargeting m_targeting;


	EnemyWeapon m_weapon;

	EnemyContact m_contact;


	int m_maxHP =
		50;


	int m_hp =
		50;
};