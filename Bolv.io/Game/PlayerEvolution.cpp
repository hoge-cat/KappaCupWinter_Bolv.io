#include "stdafx.h"
#include "PlayerEvolution.h"


namespace
{
	// ========================================================
	// 現在実装済みの進化候補一覧
	//
	// この順番で進化UIへ表示される。
	// ========================================================

	const PlayerEvolution::TankClass
		EVOLUTION_CANDIDATES[] =
	{
		// Lv15
		PlayerEvolution::Twin,
		PlayerEvolution::Sniper,
		PlayerEvolution::MachineGun,
		PlayerEvolution::FlankGuard,

		// Lv30
		PlayerEvolution::TripleShot,
		PlayerEvolution::QuadTank,
		PlayerEvolution::TwinFlank,

		PlayerEvolution::Assassin,
		PlayerEvolution::Hunter,
		PlayerEvolution::Trapper,

		PlayerEvolution::Smasher,

		// Lv45
		PlayerEvolution::Sprayer,

		// Machine Gun系 Lv30
		PlayerEvolution::Destroyer
	};


	const int EVOLUTION_CANDIDATE_COUNT =
		sizeof(
			EVOLUTION_CANDIDATES
			) /
		sizeof(
			EVOLUTION_CANDIDATES[0]
			);
}


// ============================================================
// Update
// ============================================================

void PlayerEvolution::Update(
	int currentLevel
)
{
	if (currentLevel <
		1)
	{
		currentLevel =
			1;
	}


	// ============================================
	// Lvが変わったら、
	// 閉じた進化UIを再表示可能にする。
	// ============================================

	if (currentLevel !=
		m_previousLevel)
	{
		m_evolutionSkipped =
			false;


		m_previousLevel =
			currentLevel;
	}


	m_currentLevel =
		currentLevel;
}


// ============================================================
// HasAvailableEvolution
// ============================================================

bool PlayerEvolution::HasAvailableEvolution() const
{
	if (m_evolutionSkipped)
	{
		return false;
	}


	return
		GetAvailableEvolutionCount() >
		0;
}


// ============================================================
// GetAvailableEvolutionCount
// ============================================================

int PlayerEvolution::GetAvailableEvolutionCount() const
{
	if (m_evolutionSkipped)
	{
		return 0;
	}


	int count =
		0;


	for (int i = 0;
		i <
		EVOLUTION_CANDIDATE_COUNT;
		++i)
	{
		if (CanEvolveTo(
			EVOLUTION_CANDIDATES[i]
		))
		{
			++count;
		}
	}


	return count;
}


// ============================================================
// GetAvailableEvolution
// ============================================================

PlayerEvolution::TankClass
PlayerEvolution::GetAvailableEvolution(
	int index
) const
{
	if (index <
		0)
	{
		return Basic;
	}


	int availableIndex =
		0;


	for (int i = 0;
		i <
		EVOLUTION_CANDIDATE_COUNT;
		++i)
	{
		const TankClass tankClass =
			EVOLUTION_CANDIDATES[i];


		if (!CanEvolveTo(
			tankClass
		))
		{
			continue;
		}


		if (availableIndex ==
			index)
		{
			return tankClass;
		}


		++availableIndex;
	}


	return Basic;
}


// ============================================================
// CanEvolveTo
// ============================================================

bool PlayerEvolution::CanEvolveTo(
	TankClass tankClass
) const
{
	// ========================================================
	// Basic
	// ========================================================

	if (m_currentClass ==
		Basic)
	{
		// ========================================
		// Lv15
		// ========================================

		if (m_currentLevel >=
			15)
		{
			if (
				tankClass ==
				Twin ||

				tankClass ==
				Sniper ||

				tankClass ==
				MachineGun ||

				tankClass ==
				FlankGuard)
			{
				return true;
			}
		}


		// ========================================
		// BasicのままLv30
		//
		// Lv15候補を残しながら
		// Smasherを追加する。
		// ========================================

		if (m_currentLevel >=
			30 &&
			tankClass ==
			Smasher)
		{
			return true;
		}


		return false;
	}


	// ========================================================
	// Twin
	// ========================================================

	if (m_currentClass ==
		Twin)
	{
		if (m_currentLevel >=
			30)
		{
			if (
				tankClass ==
				TripleShot ||

				tankClass ==
				QuadTank ||

				tankClass ==
				TwinFlank)
			{
				return true;
			}
		}


		return false;
	}


	// ========================================================
	// Sniper
	// ========================================================

	if (m_currentClass ==
		Sniper)
	{
		if (m_currentLevel >=
			30)
		{
			if (
				tankClass ==
				Assassin ||

				tankClass ==
				Hunter ||

				tankClass ==
				Trapper)
			{
				return true;
			}
		}


		return false;
	}



	// ========================================================
	// Machine Gun
	// ========================================================

	if (m_currentClass ==
		MachineGun)
	{
		// Lv30 : Destroyer
		if (m_currentLevel >=
			30 &&
			tankClass ==
			Destroyer)
		{
			return true;
		}

		// Lv45 : Sprayer
		if (m_currentLevel >=
			45 &&
			tankClass ==
			Sprayer)
		{
			return true;
		}

		return false;
	}



	// ========================================================
	// Flank Guard
	// ========================================================

	if (m_currentClass ==
		FlankGuard)
	{
		if (m_currentLevel >=
			30)
		{
			if (
				tankClass ==
				QuadTank ||

				tankClass ==
				TwinFlank)
			{
				return true;
			}
		}


		return false;
	}


	// ========================================================
	// Triple Shot
	// ========================================================

	if (m_currentClass ==
		TripleShot)
	{
		return false;
	}


	// ========================================================
	// Quad Tank
	// ========================================================

	if (m_currentClass ==
		QuadTank)
	{
		return false;
	}


	// ========================================================
	// Twin Flank
	// ========================================================

	if (m_currentClass ==
		TwinFlank)
	{
		return false;
	}


	// ========================================================
	// Assassin
	// ========================================================

	if (m_currentClass ==
		Assassin)
	{
		return false;
	}


	// ========================================================
	// Hunter
	// ========================================================

	if (m_currentClass ==
		Hunter)
	{
		return false;
	}


	// ========================================================
	// Trapper
	// ========================================================

	if (m_currentClass ==
		Trapper)
	{
		return false;
	}


	// ========================================================
	// Smasher
	// ========================================================

	if (m_currentClass ==
		Smasher)
	{
		return false;
	}


	// ========================================================
	// Sprayer
	// ========================================================

	if (m_currentClass ==
		Sprayer)
	{
		return false;
	}

	// ========================================================
	// Destroyer
	// ========================================================

	if (m_currentClass ==
		Destroyer)
	{
		return false;
	}


	return false;
}


// ============================================================
// EvolveTo
// ============================================================

bool PlayerEvolution::EvolveTo(
	TankClass tankClass
)
{
	if (m_evolutionSkipped)
	{
		return false;
	}


	if (!CanEvolveTo(
		tankClass
	))
	{
		return false;
	}


	m_currentClass =
		tankClass;


	m_evolutionSkipped =
		false;


	return true;
}


// ============================================================
// GetAvailableStage
//
// 旧コード互換。
// ============================================================

PlayerEvolution::EvolutionStage
PlayerEvolution::GetAvailableStage() const
{
	if (!HasAvailableEvolution())
	{
		return None;
	}


	// ============================================
	// Lv45
	// ============================================

	if (m_currentLevel >=
		45)
	{
		if (m_currentClass ==
			MachineGun)
		{
			return Level45;
		}
	}


	// ============================================
	// Lv30
	// ============================================

	if (m_currentLevel >=
		30)
	{
		if (
			m_currentClass ==
			Basic ||

			m_currentClass ==
			Twin ||

			m_currentClass ==
			Sniper ||

			m_currentClass ==
			FlankGuard)
		{
			return Level30;
		}
	}


	// ============================================
	// Lv15
	// ============================================

	if (m_currentClass ==
		Basic &&
		m_currentLevel >=
		15)
	{
		return Level15;
	}


	return None;
}