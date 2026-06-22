#pragma once
#include <iostream>
#include <cstdlib>

// State enumeration for the application routing matrix
enum class ScreenType { MAIN_MENU, NOW_PLAYING, BROWSE_PLAYLISTS, EXIT };

class Screen {
public:
    virtual ~Screen() = default;

    // Pure virtual functions: Must be implemented by all future concrete screens
    virtual void render() = 0;
    virtual ScreenType handleInput() = 0;

    // Cross-platform helper utility to clear the console window cleanly
    void clearScreen() {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }
};