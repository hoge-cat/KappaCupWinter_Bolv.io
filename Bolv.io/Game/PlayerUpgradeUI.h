#pragma once
#include <array>
#include "PlayerStats.h"

class Player;

class PlayerUpgradeUI : public IGameObject
{
public:
    ~PlayerUpgradeUI();
    bool Start();
    static void HandleWindowMessage(UINT message, WPARAM key, LPARAM parameter);
    static bool PointerBlocksShooting();
    void Update();
    void Render(RenderContext& rc);

private:
    // 表示を更新する。
    void UpdateText();
    bool CanAcceptInput() const;
    int GetRowCount() const;
    PlayerStats::StatType GetStatType(int row) const;
    bool CanUpgradeRow(int row) const;
    int HitTest(float x, float y, bool plusOnly) const;
    void QueueUpgrade(int row);
    static PlayerUpgradeUI* s_inputTarget;
    std::array<PlayerStats::StatType, 32> m_requests;
    int m_requestCount = 0;
    bool m_clickCaptured = false;

private:
    Player* m_player = nullptr;

    static const int STAT_COUNT = 8;

    FontRender m_statFontRender[STAT_COUNT];

    FontRender m_pointFontRender;
    SpriteRender m_statBackground[STAT_COUNT];
    SpriteRender m_statFill[STAT_COUNT];
    SpriteRender m_plusBackground[STAT_COUNT];
    FontRender m_plusFont[STAT_COUNT];
    FontRender m_hintFontRender;
};