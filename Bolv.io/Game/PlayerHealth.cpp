#include "stdafx.h"
#include "PlayerHealth.h"

void PlayerHealth::TakeDamage(int damage)
{
    if (damage <= 0 || IsDead())
    {
        return;
    }

    m_hp -= damage;

    // HPがマイナスにならないようにする。
    if (m_hp < 0)
    {
        m_hp = 0;
    }
}