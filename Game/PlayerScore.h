#pragma once

class PlayerScore
{
public:
    void AddScore(int amount);

    int GetScore() const
    {
        return m_score;
    }

private:
    int m_score = 0;
};