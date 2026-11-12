#pragma once

#include <vector>

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
    void PlayEvolveSE(bool afterSelection = false);
    void PlayGameOverSE();
    void PlayLevelUpSE();
    static void PlaySelectSE();
    static void PlayTitleFlybySE();
    static void PlayTitleBricksSE();
    void PlayUpgradeSE();

private:
    static void PlaySE(int bankId);

    // ループBGMだけを保持する。ワンショットSEはエンジンが自動削除する。
    SoundSource* m_bgm = nullptr;

    // 射撃SEの連続生成を抑える。
    std::vector<float> m_evolveDelays;
    float m_shootCooldown = 0.0f;
    static constexpr float SHOOT_SE_INTERVAL = 0.12f;
};
