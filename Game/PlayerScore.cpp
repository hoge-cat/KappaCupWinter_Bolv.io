#include "stdafx.h"
#include "PlayerScore.h"

void PlayerScore::AddScore(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    m_score += amount;
}