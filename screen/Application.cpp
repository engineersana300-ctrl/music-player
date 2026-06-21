#include "Application.h"
#include "CsvLoader.h"
#include "M3uLoader.h"
#include "ConcreteScreens.h" // <-- Hook in the concrete views
#include <fstream>
#include <sstream>

Application::Application() : running(true) {}

void Application::loadSettings() {
    std::ifstream file("Data/settings.cfg");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, value;
        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            if (key == "playback_mode") {
                player.setPlaybackMode(static_cast<PlaybackMode>(std::stoi(value)));
            } else if (key == "last_song") {
                lastPlayedPath = value;
                Song* s = library.findSongByPath(lastPlayedPath);
                if (s) player.loadSong(s);
            }
        }
    }
}

void Application::saveSettings() {
    std::ofstream file("Data/settings.cfg");
    if (!file.is_open()) return;

    file << "playback_mode=" << static_cast<int>(player.getPlaybackMode()) << "\n";
    if (player.getCurrentSong()) {
        file << "last_song=" << player.getCurrentSong()->getFilePath() << "\n";
    }
}

void Application::init() {
    CsvLoader::load("Data/library.csv", library);
    M3uLoader::loadAll("Data/Playlists", library);
    loadSettings();
}

void Application::run() {
    // 1. Instantiate the view panels right on the stack frame
    MainMenuScreen mainMenu(player);
    BrowsePlaylistScreen browseScreen(player, library);
    NowPlayingScreen nowPlaying(player);

    // 2. Establish our polymorphic base pointer viewer
    Screen* activeView = &mainMenu;
    ScreenType routingTarget = ScreenType::MAIN_MENU;

    // 3. The Production Application Event Loop
    while (running) {
        // Continuous hardware streaming heartbeat calculation check
        player.tick(); 
        
        // Polymorphic Call 1: Render whatever layout is currently active
        activeView->render();
        
        // Polymorphic Call 2: Capture stream extractions and get next destination routing command
        routingTarget = activeView->handleInput();

        // Evaluate application layout redirection demands
        switch (routingTarget) {
            case ScreenType::MAIN_MENU:
                activeView = &mainMenu;
                break;
            case ScreenType::BROWSE_PLAYLISTS:
                activeView = &browseScreen;
                break;
            case ScreenType::NOW_PLAYING:
                activeView = &nowPlaying;
                break;
            case ScreenType::EXIT:
                running = false;
                break;
        }
    }
    shutdown();
}

void Application::shutdown() {
    saveSettings();
}