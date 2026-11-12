#pragma once

class RetryController
{
public:
    void Start();
    void Update();

private:
    enum class State
    {
        Title,
        Loading,
        WaitingForGameReady,
        Playing,
        ClearResult,
        WaitingForDeletion
    };

    void CreateTitleScene();
    void ClearCurrentScene();
    bool AreOldObjectsDeleted() const;

    State m_state = State::Title;
    State m_nextState = State::Playing;

    bool m_wasEnterPressed = false;
    bool m_wasRPressed = false;
    bool m_wasTPressed = false;
    bool m_wasF3Pressed = false;
    int m_clearScore = 0;
    int m_clearLevel = 1;

    // ロード画面を最低1フレーム表示するための管理
    int m_loadingFrames = 0;
};
