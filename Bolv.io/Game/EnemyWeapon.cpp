#include "stdafx.h"
#include "EnemyWeapon.h"

#include "Enemy.h"
#include "EnemyBullet.h"

#include <cmath>
#include <cstdlib>


namespace
{
	// ============================================
	// Player側と合わせたBolv.io調整値
	// ============================================

	const float TWIN_BARREL_OFFSET =
		11.0f;

	// Twin Flank: 前後の左右砲身を交互に使用
	const float TWIN_FLANK_BARREL_OFFSET = 11.0f;
	const float TWIN_FLANK_BULLET_DAMAGE_MULTIPLIER = 0.60f;
	const float TWIN_FLANK_BULLET_PENETRATION_MULTIPLIER = 0.90f;


	const float SNIPER_BULLET_SPEED_MULTIPLIER =
		1.35f;


	const float SNIPER_RELOAD_MULTIPLIER =
		1.35f;


	const float SNIPER_MUZZLE_MULTIPLIER =
		1.35f;


	const float MACHINE_GUN_RELOAD_MULTIPLIER =
		0.45f;


	const float MACHINE_GUN_SPREAD_DEG =
		6.0f;



	// ============================================
	// Sprayer
	//
	// 外側と内側の砲身を独立して発射する。
	// 1tick = 0.04秒
	// ============================================

	const float SPRAYER_OUTER_SPREAD_DEG = 10.0f;
	const float SPRAYER_INNER_SPREAD_DEG = 3.0f;

	const float SPRAYER_OUTER_DAMAGE_MULTIPLIER = 0.7f;
	const float SPRAYER_INNER_DAMAGE_MULTIPLIER = 0.1f;

	const float SPRAYER_INNER_BULLET_SIZE = 0.7f;

	// Player側と同じ暫定リロード値
	const int SPRAYER_OUTER_RELOAD_TICKS[8] =
	{ 8, 7, 7, 6, 6, 5, 5, 4 };

	const int SPRAYER_INNER_RELOAD_TICKS[8] =
	{ 15, 14, 13, 12, 11, 10, 9, 8 };


	const float ASSASSIN_BULLET_SPEED_MULTIPLIER = 1.50f;
	const float ASSASSIN_RELOAD_MULTIPLIER = 1.50f;
	const float ASSASSIN_MUZZLE_MULTIPLIER = 1.32f;

	const float HUNTER_SECOND_SHOT_DELAY = 0.08f;
	const float HUNTER_BULLET_SPEED_MULTIPLIER = 1.40f;
	const float HUNTER_BULLET_DAMAGE_MULTIPLIER = 0.75f;
	const float HUNTER_SMALL_BULLET_SPREAD_DEG = 1.5f;
	const float HUNTER_SMALL_BULLET_SIZE = 0.72f;
	const float HUNTER_SMALL_MUZZLE_MULTIPLIER = 1.18f;
	const float HUNTER_BIG_MUZZLE_MULTIPLIER = 1.02f;
}


// ============================================================
// Update
// ============================================================

void EnemyWeapon::Update(
	Enemy& enemy,
	const Vector3& position,
	const Vector3& direction
)
{
	const float deltaTime =
		g_gameTime->GetFrameDeltaTime();

	// Hunterの予約済み2発目は通常リロードとは独立して処理する。
	if (m_hunterSecondShotPending)
	{
		if (enemy.GetTankClass() != EnemyEvolution::Hunter)
		{
			m_hunterSecondShotPending = false;
		}
		else
		{
			m_hunterSecondShotRemainingTime -= deltaTime;
			if (m_hunterSecondShotRemainingTime <= 0.0f)
			{
				m_hunterSecondShotPending = false;
				Fire(enemy, position, direction, 0.0f, 0.0f,
					HUNTER_BULLET_DAMAGE_MULTIPLIER, 1.0f,
					HUNTER_BIG_MUZZLE_MULTIPLIER);
			}
		}
	}


	// Sprayerは外側・内側それぞれの
	// 独立したリロードタイマーで管理する。
	if (enemy.GetTankClass() == EnemyEvolution::Sprayer)
	{
		UpdateSprayer(
			enemy,
			position,
			direction,
			deltaTime
		);

		return;
	}


	m_remainingTime -= deltaTime;


	if (m_remainingTime >
		0.0f)
	{
		return;
	}


	Vector3 aimDirection =
		direction;


	aimDirection.y =
		0.0f;


	const float lengthSquared =
		aimDirection.x *
		aimDirection.x +
		aimDirection.z *
		aimDirection.z;


	if (lengthSquared <=
		0.0001f)
	{
		return;
	}


	aimDirection.Normalize();


	const EnemyEvolution::TankClass tankClass =
		enemy
		.GetTankClass();


	// ========================================================
	// Smasher
	// ========================================================

	if (tankClass ==
		EnemyEvolution::Smasher)
	{
		m_twinFireLeft =
			true;


		return;
	}


	float reloadInterval =
		enemy
		.GetReloadInterval();


	// ============================================
	// Triple Shot
	// 中央・左35度・右35度の3方向へ同時射撃
	// ============================================

	if (tankClass ==
		EnemyEvolution::TripleShot)
	{
		// 中央
		Fire(
			enemy,
			position,
			aimDirection,
			0.0f,
			0.0f
		);

		// 左
		Fire(
			enemy,
			position,
			aimDirection,
			-6.0f,
			-30.0f
		);

		// 右
		Fire(
			enemy,
			position,
			aimDirection,
			6.0f,
			30.0f
		);

		m_twinFireLeft = true;

		if (reloadInterval < 0.04f)
		{
			reloadInterval = 0.04f;
		}

		m_remainingTime = reloadInterval;

		return;
	}


	// ========================================================
	// Quad Tank
	// 正面・右・後方・左の4方向へ同時射撃
	// ========================================================

	if (tankClass == EnemyEvolution::QuadTank)
	{
		Fire(enemy, position, aimDirection, 0.0f, 0.0f);
		Fire(enemy, position, aimDirection, 0.0f, 90.0f);
		Fire(enemy, position, aimDirection, 0.0f, 180.0f);
		Fire(enemy, position, aimDirection, 0.0f, -90.0f);

		m_twinFireLeft = true;

		if (reloadInterval < 0.04f)
		{
			reloadInterval = 0.04f;
		}

		m_remainingTime = reloadInterval;
		return;
	}


	// ========================================================
	// Twin Flank
	// 前後から同時に1発ずつ、左右砲身を交互に使用
	// ========================================================

	if (tankClass == EnemyEvolution::TwinFlank)
	{
		if (m_twinFlankFireLeft)
		{
			// 前方左
			Fire(enemy, position, aimDirection,
				-TWIN_FLANK_BARREL_OFFSET, 0.0f,
				TWIN_FLANK_BULLET_DAMAGE_MULTIPLIER);

			// 後方左（180度回転で左右基準が反転）
			Fire(enemy, position, aimDirection,
				TWIN_FLANK_BARREL_OFFSET, 180.0f,
				TWIN_FLANK_BULLET_DAMAGE_MULTIPLIER);
		}
		else
		{
			// 前方右
			Fire(enemy, position, aimDirection,
				TWIN_FLANK_BARREL_OFFSET, 0.0f,
				TWIN_FLANK_BULLET_DAMAGE_MULTIPLIER);

			// 後方右
			Fire(enemy, position, aimDirection,
				-TWIN_FLANK_BARREL_OFFSET, 180.0f,
				TWIN_FLANK_BULLET_DAMAGE_MULTIPLIER);
		}

		m_twinFlankFireLeft = !m_twinFlankFireLeft;
		m_twinFireLeft = true;

		// Twinと同じリロード倍率
		reloadInterval *= 0.5f;
		if (reloadInterval < 0.04f)
		{
			reloadInterval = 0.04f;
		}
		m_remainingTime = reloadInterval;
		return;
	}


	// ========================================================
	// Twin
	//
	// 左右を交互に発射。
	// ========================================================

	if (tankClass ==
		EnemyEvolution::Twin)
	{
		float sideOffset =
			TWIN_BARREL_OFFSET;


		if (m_twinFireLeft)
		{
			sideOffset =
				-TWIN_BARREL_OFFSET;
		}


		Fire(
			enemy,
			position,
			aimDirection,
			sideOffset
		);


		m_twinFireLeft =
			!m_twinFireLeft;


		reloadInterval *=
			0.5f;
	}


	// ========================================================
	// Sniper
	// ========================================================

	else if (
		tankClass ==
		EnemyEvolution::Sniper)
	{
		Fire(
			enemy,
			position,
			aimDirection
		);


		reloadInterval *=
			SNIPER_RELOAD_MULTIPLIER;
	}


	// ========================================================
	// Assassin
	// ========================================================

	else if (tankClass == EnemyEvolution::Assassin)
	{
		Fire(enemy, position, aimDirection);
		reloadInterval *= ASSASSIN_RELOAD_MULTIPLIER;
	}

	// ========================================================
	// Hunter: 小弾を撃ち、0.08秒後に大弾を撃つ
	// ========================================================

	else if (tankClass == EnemyEvolution::Hunter)
	{
		const float random01 =
			static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		const float spreadAngle =
			(random01 * 2.0f - 1.0f) * HUNTER_SMALL_BULLET_SPREAD_DEG;

		Fire(enemy, position, aimDirection, 0.0f, spreadAngle,
			HUNTER_BULLET_DAMAGE_MULTIPLIER,
			HUNTER_SMALL_BULLET_SIZE,
			HUNTER_SMALL_MUZZLE_MULTIPLIER);

		m_hunterSecondShotPending = true;
		m_hunterSecondShotRemainingTime = HUNTER_SECOND_SHOT_DELAY;
	}

	// ========================================================
	// Machine Gun
	// ========================================================

	else if (
		tankClass ==
		EnemyEvolution::MachineGun)
	{
		const float random01 =
			static_cast<float>(
				std::rand()
				) /
			static_cast<float>(
				RAND_MAX
				);


		const float spreadAngle =
			(
				random01 *
				2.0f -
				1.0f
				) *
			MACHINE_GUN_SPREAD_DEG;


		Fire(
			enemy,
			position,
			aimDirection,
			0.0f,
			spreadAngle
		);


		reloadInterval *=
			MACHINE_GUN_RELOAD_MULTIPLIER;


		m_twinFireLeft =
			true;
	}


	// ========================================================
	// Flank Guard
	//
	// 本家同様、前後同時発射。
	// ========================================================

	else if (
		tankClass ==
		EnemyEvolution::FlankGuard)
	{
		Fire(
			enemy,
			position,
			aimDirection
		);


		Vector3 backDirection =
			aimDirection *
			-1.0f;


		Fire(
			enemy,
			position,
			backDirection
		);
	}


	// ========================================================
	// Basic
	// ========================================================

	else
	{
		Fire(
			enemy,
			position,
			aimDirection
		);


		m_twinFireLeft =
			true;
	}


	// ============================================
	// 安全下限
	// ============================================

	if (reloadInterval <
		0.04f)
	{
		reloadInterval =
			0.04f;
	}


	m_remainingTime =
		reloadInterval;
}


// ============================================================
// Fire
// ============================================================

void EnemyWeapon::Fire(
	Enemy& enemy,
	const Vector3& position,
	const Vector3& originalDirection,
	float sideOffset,
	float spreadAngleDeg,
	float damageMultiplier,
	float bulletScaleMultiplier,
	float muzzleDistanceMultiplier
)
{
	Vector3 direction =
		originalDirection;


	direction.y =
		0.0f;


	if (
		direction.x *
		direction.x +
		direction.z *
		direction.z <=
		0.0001f)
	{
		return;
	}


	direction.Normalize();


	const EnemyEvolution::TankClass tankClass =
		enemy
		.GetTankClass();


	// ============================================
	// 拡散
	// ============================================

	if (spreadAngleDeg !=
		0.0f)
	{
		const float radian =
			spreadAngleDeg *
			3.14159265f /
			180.0f;


		const float cosValue =
			std::cos(
				radian
			);


		const float sinValue =
			std::sin(
				radian
			);


		const float originalX =
			direction.x;


		const float originalZ =
			direction.z;


		direction.x =
			originalX *
			cosValue +
			originalZ *
			sinValue;


		direction.z =
			-originalX *
			sinValue +
			originalZ *
			cosValue;


		direction.Normalize();
	}


	const float levelScale =
		enemy
		.GetLevelScale();


	// ============================================
	// 右方向
	// ============================================

	Vector3 right(
		direction.z,
		0.0f,
		-direction.x
	);


	if (
		right.x *
		right.x +
		right.z *
		right.z >
		0.0001f)
	{
		right.Normalize();
	}


	// ============================================
	// 発射位置
	// ============================================

	Vector3 firePosition =
		position;


	firePosition.y +=
		30.0f *
		levelScale;


	firePosition +=
		right *
		(
			sideOffset *
			levelScale
			);


	float muzzleDistance =
		56.0f;


	if (tankClass ==
		EnemyEvolution::Sniper)
	{
		muzzleDistance *=
			SNIPER_MUZZLE_MULTIPLIER;
	}
	else if (tankClass == EnemyEvolution::Assassin)
	{
		muzzleDistance *= ASSASSIN_MUZZLE_MULTIPLIER;
	}

	muzzleDistance *= muzzleDistanceMultiplier;


	firePosition +=
		direction *
		(
			muzzleDistance *
			levelScale
			);


	// ============================================
	// 弾性能
	// ============================================

	int damage =
		enemy
		.GetBulletDamage();


	float speed =
		enemy
		.GetBulletSpeed();


	int penetration =
		enemy
		.GetBulletPenetration();


	int collisionDamage =
		enemy
		.GetBulletCollisionDamage();

	// Twin Flankは通常弾の貫通力90%。
	// EnemyBullet::Initは整数の貫通力を受け取る。
	if (tankClass == EnemyEvolution::TwinFlank)
	{
		penetration = static_cast<int>(
			static_cast<float>(penetration) *
			TWIN_FLANK_BULLET_PENETRATION_MULTIPLIER);
		if (penetration < 1)
		{
			penetration = 1;
		}
	}


	// ============================================
	// Sniper
	// ============================================

	if (tankClass ==
		EnemyEvolution::Sniper)
	{
		speed *=
			SNIPER_BULLET_SPEED_MULTIPLIER;
	}
	else if (tankClass == EnemyEvolution::Assassin)
	{
		speed *= ASSASSIN_BULLET_SPEED_MULTIPLIER;
	}
	else if (tankClass == EnemyEvolution::Hunter)
	{
		speed *= HUNTER_BULLET_SPEED_MULTIPLIER;
	}


	// ============================================
	// 小弾などの補正
	// ============================================

	damage =
		static_cast<int>(
			static_cast<float>(
				damage
				) *
			damageMultiplier
			);


	collisionDamage =
		static_cast<int>(
			static_cast<float>(
				collisionDamage
				) *
			damageMultiplier
			);


	// 最低1ダメージ。
	if (damage <
		1)
	{
		damage =
			1;
	}


	if (collisionDamage <
		1)
	{
		collisionDamage =
			1;
	}


	// ============================================
	// 弾生成
	// ============================================

	EnemyBullet* bullet =
		NewGO<EnemyBullet>(
			0,
			"enemyBullet"
		);


	if (bullet ==
		nullptr)
	{
		return;
	}


	bullet->Init(
		firePosition,
		direction,
		damage,
		speed,
		penetration,
		collisionDamage,
		levelScale *
		bulletScaleMultiplier,
		&enemy
	);
}


// ============================================================
// UpdateSprayer
//
// 外側砲身と内側砲身を独立した周期で発射する。
// ============================================================

void EnemyWeapon::UpdateSprayer(
	Enemy& enemy,
	const Vector3& position,
	const Vector3& originalDirection,
	float deltaTime
)
{
	Vector3 direction = originalDirection;
	direction.y = 0.0f;

	const float lengthSquared =
		direction.x * direction.x +
		direction.z * direction.z;

	if (lengthSquared <= 0.0001f)
	{
		return;
	}

	direction.Normalize();

	// ============================================
	// Reloadステータス
	//
	// 現段階では敵のReloadステータスを
	// 直接取得する関数を確認していないため、
	// Lv0相当の値を使用する。
	// ============================================

	const int reloadLevel = 0;

	const float outerInterval =
		SPRAYER_OUTER_RELOAD_TICKS[reloadLevel] * 0.04f;

	const float innerInterval =
		SPRAYER_INNER_RELOAD_TICKS[reloadLevel] * 0.04f;

	// ============================================
	// 外側砲身
	// ============================================

	m_sprayerOuterRemainingTime -= deltaTime;

	if (m_sprayerOuterRemainingTime <= 0.0f)
	{
		const float random01 =
			static_cast<float>(std::rand()) /
			static_cast<float>(RAND_MAX);

		const float spreadAngle =
			(random01 * 2.0f - 1.0f) *
			SPRAYER_OUTER_SPREAD_DEG;

		Fire(
			enemy,
			position,
			direction,
			0.0f,
			spreadAngle,
			SPRAYER_OUTER_DAMAGE_MULTIPLIER
		);

		m_sprayerOuterRemainingTime = outerInterval;
	}

	// ============================================
	// 内側砲身
	// ============================================

	m_sprayerInnerRemainingTime -= deltaTime;

	if (m_sprayerInnerRemainingTime <= 0.0f)
	{
		const float random01 =
			static_cast<float>(std::rand()) /
			static_cast<float>(RAND_MAX);

		const float spreadAngle =
			(random01 * 2.0f - 1.0f) *
			SPRAYER_INNER_SPREAD_DEG;

		Fire(
			enemy,
			position,
			direction,
			0.0f,
			spreadAngle,
			SPRAYER_INNER_DAMAGE_MULTIPLIER,
			SPRAYER_INNER_BULLET_SIZE
		);

		m_sprayerInnerRemainingTime = innerInterval;
	}
}
