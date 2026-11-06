#pragma once

class AudioManager : public IGameObject
{
public:
    AudioManager() = default;
    ~AudioManager();

    bool Start();
    void Update();

    void PlayGameBGM();
    void PlayTitleBGM();
    void StopBGM();

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

    // ループBGMだけを保持する。ワンショットSEはエンジンが自動削除する。
    SoundSource* m_bgm = nullptr;

    // 射撃SEの連続生成を抑える。
    float m_shootCooldown = 0.0f;
    static constexpr float SHOOT_SE_INTERVAL = 0.12f;
};
