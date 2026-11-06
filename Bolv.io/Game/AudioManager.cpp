
#include "stdafx.h"
#include "AudioManager.h"

// ============================================
// 音声ファイルの登録番号
// ============================================
namespace
{
    constexpr int BGM_GAME = 0;
    constexpr int BGM_TITLE = 1;

    constexpr int SE_DAMAGE = 2;
    constexpr int SE_DESTROY = 3;
    constexpr int SE_EVOLVE = 4;
    constexpr int SE_GAMEOVER = 5;
    constexpr int SE_LEVELUP = 6;
    constexpr int SE_SELECT = 7;
    constexpr int SE_SHOOT = 8;
    constexpr int SE_UPGRADE = 9;
}

// ============================================
// デストラクタ
// ============================================
AudioManager::~AudioManager()
{
    StopBGM();
}

// ============================================
// 初期化
// ============================================
bool AudioManager::Start()
{
    // ========================================
    // BGM登録
    // ========================================
    g_soundEngine->ResistWaveFileBank(
        BGM_GAME,
        "Assets/sound/bgm_game.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        BGM_TITLE,
        "Assets/sound/bgm_title.wav"
    );

    // ========================================
    // SE登録
    // ========================================
    g_soundEngine->ResistWaveFileBank(
        SE_DAMAGE,
        "Assets/sound/se_damage.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_DESTROY,
        "Assets/sound/se_destroy.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_EVOLVE,
        "Assets/sound/se_evolve.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_GAMEOVER,
        "Assets/sound/se_gameover.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_LEVELUP,
        "Assets/sound/se_levelup.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_SELECT,
        "Assets/sound/se_select.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_SHOOT,
        "Assets/sound/se_shoot.wav"
    );

    g_soundEngine->ResistWaveFileBank(
        SE_UPGRADE,
        "Assets/sound/se_upgrade.wav"
    );

    // BGMはロード完了後に再生する
    return true;
}

// ============================================
// インゲームBGM
// ============================================
void AudioManager::PlayGameBGM()
{
    StopBGM();

    m_bgm = NewGO<SoundSource>(0);

    m_bgm->Init(BGM_GAME);

    // ループ再生
    m_bgm->Play(true);
}

// ============================================
// タイトルBGM
// ============================================
void AudioManager::PlayTitleBGM()
{
    StopBGM();

    m_bgm = NewGO<SoundSource>(0);

    m_bgm->Init(BGM_TITLE);

    // ループ再生
    m_bgm->Play(true);
}

// ============================================
// BGM停止
// ============================================
void AudioManager::StopBGM()
{
    if (m_bgm != nullptr)
    {
        m_bgm->Stop();

        DeleteGO(m_bgm);

        m_bgm = nullptr;
    }
}

// ============================================
// SE再生（共通）
// ============================================
void AudioManager::PlaySE(int bankId)
{
    SoundSource* se = NewGO<SoundSource>(0);

    if (se == nullptr)
    {
        return;
    }

    se->Init(bankId);

    // ループなし
    se->Play(false);
}

// ============================================
// 射撃SE
// ============================================
void AudioManager::PlayShootSE()
{
    PlaySE(SE_SHOOT);
}

// ============================================
// ダメージSE
// ============================================
void AudioManager::PlayDamageSE()
{
    PlaySE(SE_DAMAGE);
}

// ============================================
// 破壊SE
// ============================================
void AudioManager::PlayDestroySE()
{
    PlaySE(SE_DESTROY);
}

// ============================================
// 進化SE
// ============================================
void AudioManager::PlayEvolveSE()
{
    PlaySE(SE_EVOLVE);
}

// ============================================
// ゲームオーバーSE
// ============================================
void AudioManager::PlayGameOverSE()
{
    PlaySE(SE_GAMEOVER);
}

// ============================================
// レベルアップSE
// ============================================
void AudioManager::PlayLevelUpSE()
{
    PlaySE(SE_LEVELUP);
}

// ============================================
// 選択SE
// ============================================
void AudioManager::PlaySelectSE()
{
    PlaySE(SE_SELECT);
}

// ============================================
// アップグレードSE
// ============================================
void AudioManager::PlayUpgradeSE()
{
    PlaySE(SE_UPGRADE);
}
