#pragma once

// ユーザー設定はシーンをまたいで保持し、LocalAppDataに保存する。
class GameSettings
{
public:
    static void Load();
    static bool Save();
    static int GetVolume(int channel); // 0:マスター 1:SE 2:BGM
    static void SetVolume(int channel, int percent);
    static void ApplyAudio();
    static bool GetFullscreen();
    static bool SetFullscreen(bool fullscreen);
};
