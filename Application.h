#pragma once
#include "MusicLibrary.h"
#include "Player.h"
#include <string>

class Application {
private:
    MusicLibrary library;
    Player player;
    bool running;
    std::string lastPlayedPath;

    // Internal helper utilities for state persistence (Homework requirements)
    void loadSettings();
    void saveSettings();

public:
    Application();
    void init();
    void run();
    void shutdown();
};