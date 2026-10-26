#include "stdafx.h"
#include "EnemyRenderer.h"
#include "Enemy.h"

#include <cmath>


namespace
{
	const float PI =
		3.14159265f;


	// ============================================
	// Twin
	// ============================================

	const float TWIN_BARREL_OFFSET =
		11.0f;


	// ============================================
	// 方向から砲身回転を作る
	//
	// TankBarrelの正面は-Z。
	// ============================================

	Quaternion CreateBarrelRotation(
		const Vector3& originalDirection
	)
	{
		Vector3 direction =
			originalDirection;


		direction.y =
			0.0f;


		const float lengthSquared =
			direction.x *
			direction.x +
			direction.z *
			direction.z;


		if (lengthSquared <=
			0.0001f)
		{
			direction =
				Vector3(
					0.0f,
					0.0f,
					1.0f
				);
		}
		else
		{
			direction.Normalize();
		}


		const Vector3 modelForward(
			0.0f,
			0.0f,
			-1.0f
		);


		Vector3 rotationAxis;


		rotationAxis.Cross(
			modelForward,
			direction
		);


		const float axisLength =
			std::sqrt(
				rotationAxis.x *
				rotationAxis.x +
				rotationAxis.y *
				rotationAxis.y +
				rotationAxis.z *
				rotationAxis.z
			);


		Quaternion rotation =
			Quaternion::Identity;


		if (axisLength >
			0.0001f)
		{
			rotationAxis.Normalize();


			const float angle =
				std::atan2(
					axisLength,
					-direction.z
				);


			rotation.SetRotationDeg(
				rotationAxis,
				angle *
				180.0f /
				PI
			);
		}
		else if (
			direction.z >
			0.0f)
		{
			rotation.SetRotationDeg(
				Vector3::AxisY,
				180.0f
			);
		}


		return rotation;
	}
}


// ============================================================
// Start
// ============================================================

bool EnemyRenderer::Start()
{
	// ============================================
	// 赤い敵本体
	// ============================================

	m_bodyRender.Init(
		"Assets/modelData/tank/EnemyBody.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// 通常砲身1
	// ============================================

	m_barrelRender1.Init(
		"Assets/modelData/tank/TankBarrel.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// 通常砲身2
	// ============================================

	m_barrelRender2.Init(
		"Assets/modelData/tank/TankBarrel.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// 通常砲身3
	// ============================================

	m_barrelRender3.Init(
		"Assets/modelData/tank/TankBarrel.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// 通常砲身4
	// ============================================

	m_barrelRender4.Init(
		"Assets/modelData/tank/TankBarrel.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// Machine Gun専用台形砲身
	// ============================================

	m_machineGunBarrelRender.Init(
		"Assets/modelData/tank/TankMachineGunBarrel.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// Smasher外装
	// ============================================

	m_smasherShellRender.Init(
		"Assets/modelData/tank/TankSmasherShell.tkm",
		nullptr,
		0,
		EnModelUpAxis::enModelUpAxisZ,
		true,
		MAX_INSTANCE,
		false
	);


	// ============================================
	// 最初は全部非表示
	// ============================================

	for (int i = 0;
		i < MAX_INSTANCE;
		++i)
	{
		m_bodyRender.RemoveInstance(
			i
		);


		m_barrelRender1.RemoveInstance(
			i
		);


		m_barrelRender2.RemoveInstance(
			i
		);



		m_barrelRender3.RemoveInstance(i);



		m_barrelRender4.RemoveInstance(i);


		m_machineGunBarrelRender.RemoveInstance(
			i
		);


		m_smasherShellRender.RemoveInstance(
			i
		);
	}


	return true;
}


// ============================================================
// Render
// ============================================================

void EnemyRenderer::Render(
	RenderContext& rc)
{
	// ============================================
	// Smasher外装回転
	// ============================================

	m_smasherRotationDeg +=
		90.0f *
		g_gameTime
		->GetFrameDeltaTime();


	if (m_smasherRotationDeg >=
		360.0f)
	{
		m_smasherRotationDeg -=
			360.0f;
	}


	int instanceNo =
		0;


	QueryGOs<Enemy>(
		"enemy",
		[&](Enemy* enemy)
		{
			if (enemy ==
				nullptr)
			{
				return true;
			}


			if (enemy
				->IsDead())
			{
				return true;
			}


			if (instanceNo >=
				MAX_INSTANCE)
			{
				return false;
			}


			// ====================================
			// 前フレームの同スロットを一旦消す
			//
			// 進化クラスが違う敵が同じslotに
			// 入っても前の砲身が残らないようにする。
			// ====================================

			m_barrelRender1.RemoveInstance(
				instanceNo
			);


			m_barrelRender2.RemoveInstance(
				instanceNo
			);



			m_barrelRender3.RemoveInstance(instanceNo);



			m_barrelRender4.RemoveInstance(instanceNo);


			m_machineGunBarrelRender.RemoveInstance(
				instanceNo
			);


			m_smasherShellRender.RemoveInstance(
				instanceNo
			);


			// ====================================
			// Level Scale
			// ====================================

			const float levelScale =
				enemy
				->GetLevelScale();


			// ====================================
			// 中央位置
			// ====================================

			Vector3 modelPosition =
				enemy
				->GetPosition();


			modelPosition.y +=
				30.0f *
				levelScale;


			// ====================================
			// 照準方向
			// ====================================

			Vector3 direction =
				enemy
				->GetDirection();


			direction.y =
				0.0f;


			const float directionLengthSquared =
				direction.x *
				direction.x +
				direction.z *
				direction.z;


			if (directionLengthSquared <=
				0.0001f)
			{
				direction =
					Vector3(
						0.0f,
						0.0f,
						1.0f
					);
			}
			else
			{
				direction.Normalize();
			}


			// ====================================
			// 右方向
			// ====================================

			Vector3 right(
				direction.z,
				0.0f,
				-direction.x
			);


			const float rightLengthSquared =
				right.x *
				right.x +
				right.z *
				right.z;


			if (rightLengthSquared >
				0.0001f)
			{
				right.Normalize();
			}


			// ====================================
			// 本体回転
			// ====================================

			Quaternion bodyRotation =
				Quaternion::Identity;


			const float yaw =
				std::atan2(
					direction.x,
					direction.z
				);


			bodyRotation.SetRotationDeg(
				Vector3::AxisY,
				yaw *
				180.0f /
				PI +
				180.0f
			);


			// ====================================
			// 本体サイズ
			// ====================================

			const Vector3 bodyScale(
				levelScale,
				levelScale,
				levelScale
			);


			// ====================================
			// 本体
			// ====================================

			m_bodyRender.UpdateInstancingData(
				instanceNo,
				modelPosition,
				bodyRotation,
				bodyScale
			);


			// ====================================
			// クラス
			// ====================================

			const EnemyEvolution::TankClass tankClass =
				enemy
				->GetTankClass();


			// =================================================
			// Basic
			// =================================================

			if (tankClass ==
				EnemyEvolution::Basic)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(
						direction
					);


				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					bodyScale
				);
			}


			// =================================================
			// Twin
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Twin)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(
						direction
					);


				// 左砲身
				Vector3 leftPosition =
					modelPosition -
					right *
					(
						TWIN_BARREL_OFFSET *
						levelScale
						);


				// 右砲身
				Vector3 rightPosition =
					modelPosition +
					right *
					(
						TWIN_BARREL_OFFSET *
						levelScale
						);


				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					leftPosition,
					barrelRotation,
					bodyScale
				);


				m_barrelRender2.UpdateInstancingData(
					instanceNo,
					rightPosition,
					barrelRotation,
					bodyScale
				);
			}


			// =================================================
			// Triple Shot
			// 砲身は正面・左右35度（弾は左右30度）
			// =================================================

			else if (tankClass == EnemyEvolution::TripleShot)
			{
				const float rad = 35.0f * PI / 180.0f;
				const float c = std::cos(rad);
				const float sn = std::sin(rad);

				Vector3 leftDirection(
					direction.x * c - direction.z * sn,
					0.0f,
					direction.x * sn + direction.z * c
				);
				leftDirection.Normalize();

				Vector3 rightDirection(
					direction.x * c + direction.z * sn,
					0.0f,
					-direction.x * sn + direction.z * c
				);
				rightDirection.Normalize();

				m_barrelRender1.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(direction), bodyScale
				);
				m_barrelRender2.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(leftDirection), bodyScale
				);
				m_barrelRender3.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(rightDirection), bodyScale
				);
			}


			// =================================================
			// Quad Tank
			// 正面・右・後方・左の4本砲身
			// =================================================

			else if (tankClass == EnemyEvolution::QuadTank)
			{
				const Vector3 frontDirection = direction;
				const Vector3 rightDirection(
					direction.z, 0.0f, -direction.x
				);
				const Vector3 backDirection = direction * -1.0f;
				const Vector3 leftDirection = rightDirection * -1.0f;

				m_barrelRender1.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(frontDirection), bodyScale
				);
				m_barrelRender2.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(rightDirection), bodyScale
				);
				m_barrelRender3.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(backDirection), bodyScale
				);
				m_barrelRender4.UpdateInstancingData(
					instanceNo, modelPosition,
					CreateBarrelRotation(leftDirection), bodyScale
				);
			}


			// =================================================
			// Twin Flank
			// 前方左右2本 + 後方左右2本
			// プレイヤーのTankVisualTwinFlankと同じ配置
			// =================================================

			else if (tankClass == EnemyEvolution::TwinFlank)
			{
				const Vector3 backDirection = direction * -1.0f;

				const Vector3 leftPosition =
					modelPosition - right * (TWIN_BARREL_OFFSET * levelScale);

				const Vector3 rightPosition =
					modelPosition + right * (TWIN_BARREL_OFFSET * levelScale);

				const Quaternion frontRotation =
					CreateBarrelRotation(direction);

				const Quaternion backRotation =
					CreateBarrelRotation(backDirection);

				// 前方左
				m_barrelRender1.UpdateInstancingData(
					instanceNo, leftPosition, frontRotation, bodyScale
				);

				// 前方右
				m_barrelRender2.UpdateInstancingData(
					instanceNo, rightPosition, frontRotation, bodyScale
				);

				// 後方左
				m_barrelRender3.UpdateInstancingData(
					instanceNo, leftPosition, backRotation, bodyScale
				);

				// 後方右
				m_barrelRender4.UpdateInstancingData(
					instanceNo, rightPosition, backRotation, bodyScale
				);
			}


			// =================================================
			// Sniper
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Sniper)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(
						direction
					);


				// Player側Sniperと同じ考え方。
				const Vector3 sniperScale(
					levelScale *
					0.82f,

					levelScale *
					0.82f,

					levelScale *
					1.35f
				);


				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					sniperScale
				);
			}


			// =================================================
			// Assassin
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Assassin)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(direction);

				const Vector3 assassinScale(
					levelScale * 0.72f,
					levelScale * 0.72f,
					levelScale * 1.50f
				);

				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					assassinScale
				);
			}

			// =================================================
			// Hunter
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Hunter)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(direction);

				// 外側の太い砲身
				const Vector3 outerBarrelScale(
					levelScale * 1.05f,
					levelScale * 1.05f,
					levelScale * 1.02f
				);

				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					outerBarrelScale
				);

				// 内側の細長い砲身を同じ位置に重ねる
				const Vector3 innerBarrelScale(
					levelScale * 0.72f,
					levelScale * 0.72f,
					levelScale * 1.18f
				);

				m_barrelRender2.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					innerBarrelScale
				);
			}



			// =================================================
			// Machine Gun
			//
			// プレイヤー側と同じ砲身サイズ
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::MachineGun)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(
						direction
					);

				// =============================================
				// Machine Gun専用の台形砲身
				//
				// 横幅：1.35倍
				// 高さ：1.35倍
				// 長さ：1.00倍
				// =============================================

				const Vector3 machineGunScale(
					1.35f * levelScale,
					1.35f * levelScale,
					1.00f * levelScale
				);

				m_machineGunBarrelRender
					.UpdateInstancingData(
						instanceNo,
						modelPosition,
						barrelRotation,
						machineGunScale
					);
			}



			// =================================================
			// Flank Guard
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::FlankGuard)
			{
				// 正面
				const Quaternion frontRotation =
					CreateBarrelRotation(
						direction
					);


				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					frontRotation,
					bodyScale
				);


				// 後方
				const Vector3 backDirection =
					direction *
					-1.0f;


				const Quaternion backRotation =
					CreateBarrelRotation(
						backDirection
					);


				m_barrelRender2.UpdateInstancingData(
					instanceNo,
					modelPosition,
					backRotation,
					bodyScale
				);
			}


			// =================================================
			// Smasher
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Smasher)
			{
				// 砲身なし。


				Quaternion shellRotation =
					Quaternion::Identity;


				shellRotation.SetRotationDeg(
					Vector3::AxisY,
					m_smasherRotationDeg
				);


				m_smasherShellRender.UpdateInstancingData(
					instanceNo,
					modelPosition,
					shellRotation,
					bodyScale
				);
			}



			// =================================================
			// Sprayer
			//
			// プレイヤー側と同じ2本砲身
			// =================================================

			else if (
				tankClass ==
				EnemyEvolution::Sprayer)
			{
				const Quaternion barrelRotation =
					CreateBarrelRotation(
						direction
					);


				// =============================================
				// 外側：Machine Gun砲身
				//
				// 太さ：1.35倍
				// 長さ：1.00倍
				// =============================================

				const Vector3 outerBarrelScale(
					1.35f * levelScale,
					1.35f * levelScale,
					1.00f * levelScale
				);

				m_machineGunBarrelRender
					.UpdateInstancingData(
						instanceNo,
						modelPosition,
						barrelRotation,
						outerBarrelScale
					);


				// =============================================
				// 内側：通常砲身
				//
				// 太さ：0.95倍
				// 長さ：1.20倍
				// =============================================

				const Vector3 innerBarrelScale(
					0.95f * levelScale,
					0.95f * levelScale,
					1.20f * levelScale
				);

				m_barrelRender1.UpdateInstancingData(
					instanceNo,
					modelPosition,
					barrelRotation,
					innerBarrelScale
				);
			}



			++instanceNo;


			return true;
		}
	);


	// ============================================
	// 前フレームで多かった分を削除
	// ============================================

	for (int i = instanceNo;
		i < m_previousInstanceCount;
		++i)
	{
		m_bodyRender.RemoveInstance(
			i
		);


		m_barrelRender1.RemoveInstance(
			i
		);


		m_barrelRender2.RemoveInstance(
			i
		);



		m_barrelRender3.RemoveInstance(i);



		m_barrelRender4.RemoveInstance(i);


		m_machineGunBarrelRender.RemoveInstance(
			i
		);


		m_smasherShellRender.RemoveInstance(
			i
		);
	}


	m_previousInstanceCount =
		instanceNo;


	// ============================================
	// 一括描画
	// ============================================

	m_bodyRender.Draw(
		rc
	);


	m_barrelRender1.Draw(
		rc
	);


	m_barrelRender2.Draw(
		rc
	);


	m_barrelRender3.Draw(
		rc
	);


	m_barrelRender4.Draw(
		rc
	);


	m_machineGunBarrelRender.Draw(
		rc
	);


	m_smasherShellRender.Draw(
		rc
	);
}