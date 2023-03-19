#pragma once

#include <ILogger.h>

class Game final
{
public:
    Game(ILogger& logger);

    void start();
    void stop();
    void pause();
    void showHelp();
    void closeHelp();
    void moveCamera();
    void switchCamera();

private:
    enum class State
    {
        STARTED,
        STOPPED,
        PAUSED,
        HELP,
        PAUSED_AND_HELP
    };

    [[maybe_unused]] State _state;
    [[maybe_unused]] ILogger& _logger;
};
