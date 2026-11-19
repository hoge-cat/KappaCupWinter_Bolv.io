#include "stdafx.h"
#include "GameSettings.h"
#include "system/system.h"
#include <string>

namespace
{
    int volumes[] = { 100, 100, 100 };
    bool fullscreen = true;
    std::wstring settingsPath;
    int ClampPercent(int value) { return value < 0 ? 0 : (value > 100 ? 100 : value); }
    const wchar_t* VOLUME_KEYS[] = { L"Master", L"SE", L"BGM" };
}
void GameSettings::Load()
{
    wchar_t localAppData[32768] = {};
    const DWORD length = GetEnvironmentVariableW(L"LOCALAPPDATA", localAppData, 32768);
    if (length == 0 || length >= 32768) { return; }
    const std::wstring directory = std::wstring(localAppData) + L"\\Bolv.io";
    if (!CreateDirectoryW(directory.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) { return; }
    settingsPath = directory + L"\\settings.ini";
    for (int i = 0; i < 3; ++i)
    {
        volumes[i] = ClampPercent(static_cast<int>(GetPrivateProfileIntW(
            L"Audio", VOLUME_KEYS[i], 100, settingsPath.c_str())));
    }
    fullscreen = GetPrivateProfileIntW(L"Display", L"Fullscreen", 1, settingsPath.c_str()) != 0;
}
bool GameSettings::Save()
{
    if (settingsPath.empty()) { return false; }
    bool success = true;
    for (int i = 0; i < 3; ++i)
    {
        const std::wstring value = std::to_wstring(volumes[i]);
        if (!WritePrivateProfileStringW(L"Audio", VOLUME_KEYS[i], value.c_str(), settingsPath.c_str())) { success = false; }
    }
    if (!WritePrivateProfileStringW(L"Display", L"Fullscreen", fullscreen ? L"1" : L"0", settingsPath.c_str())) { success = false; }
    return success;
}
int GameSettings::GetVolume(int channel) { return channel >= 0 && channel < 3 ? volumes[channel] : 0; }
void GameSettings::SetVolume(int channel, int percent)
{
    if (channel < 0 || channel >= 3) { return; }
    const int value = ClampPercent(percent);
    if (volumes[channel] == value) { return; }
    volumes[channel] = value;
    ApplyAudio();
}
void GameSettings::ApplyAudio()
{
    SoundSource::SetMixVolumes(volumes[0] / 100.0f, volumes[1] / 100.0f, volumes[2] / 100.0f);
}
bool GameSettings::GetFullscreen() { return fullscreen; }
bool GameSettings::SetFullscreen(bool value)
{
    if (!SetGameFullscreen(value)) { return false; }
    fullscreen = value;
    return true;
}
