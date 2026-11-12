#include "stdafx.h"
#include "AudioManager.h"

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

    constexpr float BGM_VOLUME = 0.30f;
    constexpr float SHOOT_SE_VOLUME = 0.40f;
}

AudioManager::~AudioManager()
{
    StopBGM();
    // ワンショットSEはSoundSource自身が再生終了後に削除する。
}

bool AudioManager::Start()
{
    g_soundEngine->ResistWaveFileBank(BGM_GAME, "Assets/sound/bgm_game.wav");
    g_soundEngine->ResistWaveFileBank(BGM_TITLE, "Assets/sound/bgm_title.wav");
    g_soundEngine->ResistWaveFileBank(SE_DAMAGE, "Assets/sound/se_damage.wav");
    g_soundEngine->ResistWaveFileBank(SE_DESTROY, "Assets/sound/se_destroy.wav");
    g_soundEngine->ResistWaveFileBank(SE_EVOLVE, "Assets/sound/se_evolve.wav");
    g_soundEngine->ResistWaveFileBank(SE_GAMEOVER, "Assets/sound/se_gameover.wav");
    g_soundEngine->ResistWaveFileBank(SE_LEVELUP, "Assets/sound/se_levelup.wav");
    g_soundEngine->ResistWaveFileBank(SE_SELECT, "Assets/sound/se_select.wav");
    g_soundEngine->ResistWaveFileBank(SE_SHOOT, "Assets/sound/se_shoot.wav");
    g_soundEngine->ResistWaveFileBank(SE_UPGRADE, "Assets/sound/se_upgrade.wav");

    m_shootCooldown = 0.0f;
    // BGMはロード完了後にGame側から再生する。
    return true;
}

void AudioManager::Update()
{
    for (auto it = m_evolveDelays.begin(); it != m_evolveDelays.end();)
    {
        *it -= g_gameTime->GetFrameDeltaTime();
        if (*it <= 0.0f)
        {
            PlaySE(SE_EVOLVE);
            it = m_evolveDelays.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (m_shootCooldown > 0.0f)
    {
        m_shootCooldown -= g_gameTime->GetFrameDeltaTime();
        if (m_shootCooldown < 0.0f)
        {
            m_shootCooldown = 0.0f;
        }
    }
}

void AudioManager::PlayGameBGM()
{
    StopBGM();
    m_bgm = NewGO<SoundSource>(0);
    if (m_bgm != nullptr)
    {
        m_bgm->Init(BGM_GAME);
        m_bgm->SetVolume(BGM_VOLUME);
        m_bgm->Play(true);
    }
}

void AudioManager::PlayTitleBGM()
{
    // Title playback can begin before the engine invokes Start().
    g_soundEngine->ResistWaveFileBank(BGM_TITLE, "Assets/sound/bgm_title.wav");
    StopBGM();
    m_bgm = NewGO<SoundSource>(0);
    if (m_bgm != nullptr)
    {
        m_bgm->Init(BGM_TITLE);
        m_bgm->SetVolume(BGM_VOLUME);
        m_bgm->Play(true);
    }
}

void AudioManager::StopBGM()
{
    if (m_bgm != nullptr)
    {
        m_bgm->Stop();
        DeleteGO(m_bgm);
        m_bgm = nullptr;
    }
}

void AudioManager::PlaySE(int bankId)
{
    // ワンショットSEのポインタは保持しない。
    // 再生終了後のDeleteGOはSoundSourceが行う。
    SoundSource* se = NewGO<SoundSource>(0);
    if (se == nullptr)
    {
        return;
    }
    se->Init(bankId);
    if (bankId == SE_SHOOT)
    {
        se->SetVolume(SHOOT_SE_VOLUME);
    }
    se->Play(false);
}

void AudioManager::PlayShootSE()
{
    if (m_shootCooldown > 0.0f)
    {
        return;
    }

    m_shootCooldown = SHOOT_SE_INTERVAL;
    PlaySE(SE_SHOOT);
}

void AudioManager::PlayDamageSE() { PlaySE(SE_DAMAGE); }
void AudioManager::PlayDestroySE() { PlaySE(SE_DESTROY); }
void AudioManager::PlayEvolveSE(bool afterSelection)
{
    // Keep evolution audio after the selection cue without delaying gameplay.
    if (afterSelection)
    {
        const auto wave = g_soundEngine->GetWaveFileBank().FindWaveFile(SE_SELECT);
        if (wave && wave->GetFormat() && wave->GetFormat()->nAvgBytesPerSec > 0)
        {
            m_evolveDelays.push_back(static_cast<float>(wave->GetSize()) /
                wave->GetFormat()->nAvgBytesPerSec);
            return;
        }
    }
    PlaySE(SE_EVOLVE);
}
void AudioManager::PlayGameOverSE() { PlaySE(SE_GAMEOVER); }
void AudioManager::PlayLevelUpSE() { PlaySE(SE_LEVELUP); }
void AudioManager::PlaySelectSE()
{
    // Selection can be confirmed before a scene AudioManager exists.
    g_soundEngine->ResistWaveFileBank(SE_SELECT, "Assets/sound/se_select.wav");
    PlaySE(SE_SELECT);
}
void AudioManager::PlayUpgradeSE() { PlaySE(SE_UPGRADE); }
