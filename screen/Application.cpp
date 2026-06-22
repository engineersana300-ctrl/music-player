#include "Application.h"
#include "CSVLoader.h"
#include "M3uLoader.h"
#include "Screen.h"
#include "ConcreteScreens.h"
#include <fstream>
#include <sstream>
#include <iostream>

Application::Application() : running(true) {}

void Application::loadSettings() {
    std::ifstream file("../Data/settings.cfg");
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
    std::ofstream file("../Data/settings.cfg");
    if (!file.is_open()) return;

    file << "playback_mode=" << static_cast<int>(player.getPlaybackMode()) << "\n";
    if (player.getCurrentSong()) {
        file << "last_song=" << player.getCurrentSong()->GetFilePath() << "\n";
    }
}

void Application::init() {
    // Populate the database repositories from previous branch work
    CSVLoader csvLoader;
    csvLoader.load("../Data/library.csv", library);
    M3uLoader::loadAll("../Data/Playlists", library);
    loadSettings();
}

void Application::run() {
    MainMenuScreen mainMenu(player);
    BrowsePlaylistScreen browse(player, library);
    NowPlayingScreen nowPlaying(player);
    
    Screen* current = &mainMenu;
    ScreenType next = ScreenType::MAIN_MENU;
    
    while (running) {
        player.tick();
        
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
        
        current->render();
        next = current->handleInput();
        
        switch (next) {
            case ScreenType::MAIN_MENU:        current = &mainMenu; break;
            case ScreenType::NOW_PLAYING:      current = &nowPlaying; break;
            case ScreenType::BROWSE_PLAYLISTS: current = &browse; break;
            case ScreenType::EXIT:             running = false; break;
        }
    }
    shutdown();
}

void Application::shutdown() {
    saveSettings();
}
