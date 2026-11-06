
#pragma once

class AudioManager : public IGameObject
{
public:
    AudioManager() = default;
    ~AudioManager();

    bool Start();

    // BGM
    void PlayGameBGM();
    void PlayTitleBGM();
    void StopBGM();

    // SE
    void PlayShootSE();
    void PlayDamageSE();
    void PlayDestroySE();
    void PlayEvolveSE();
    void PlayGameOverSE();
    void PlayLevelUpSE();
    void PlaySelectSE();
    void PlayUpgradeSE();

private:
    void PlaySE(int bankId);

    SoundSource* m_bgm = nullptr;
};
