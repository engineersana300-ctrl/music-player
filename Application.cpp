#include "Application.h"
#include "CsvLoader.h"
#include "M3uLoader.h"
#include "Screen.h"
#include <fstream>
#include <sstream>
#include <iostream>

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
    // Populate the database repositories from previous branch work
    CsvLoader::load("Data/library.csv", library);
    M3uLoader::loadAll("Data/Playlists", library);
    loadSettings();
}

void Application::run() {
    ScreenType currentType = ScreenType::MAIN_MENU;

    // Core TUI Orchestration Loop Testing Framework
    while (running) {
        player.tick(); // Heartbeat check for audio execution status

#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif

        std::cout << "=======================================\n";
        std::cout << "     TUI BASE ROUTING FRAMEWORK TEST   \n";
        std::cout << "=======================================\n";
        std::cout << " Current Application State: ";
        
        switch (currentType) {
            case ScreenType::MAIN_MENU:        std::cout << "[MAIN_MENU]\n"; break;
            case ScreenType::NOW_PLAYING:      std::cout << "[NOW_PLAYING]\n"; break;
            case ScreenType::BROWSE_PLAYLISTS: std::cout << "[BROWSE_PLAYLISTS]\n"; break;
            case ScreenType::EXIT:             std::cout << "[EXIT]\n"; break;
        }

        std::cout << "---------------------------------------\n";
        std::cout << " Simulate Navigation:\n";
        std::cout << " 1. Go to Now Playing\n";
        std::cout << " 2. Go to Browse Playlists\n";
        std::cout << " 3. Back to Main Menu\n";
        std::cout << " 4. Exit Application\n";
        std::cout << "---------------------------------------\n";
        std::cout << "Enter testing option (1-4): ";

        std::string input;
        std::cin >> input;

        // Input extraction processing matching the future routing table matrix
        if (input == "1") {
            currentType = ScreenType::NOW_PLAYING;
        } else if (input == "2") {
            currentType = ScreenType::BROWSE_PLAYLISTS;
        } else if (input == "3") {
            currentType = ScreenType::MAIN_MENU;
        } else if (input == "4") {
            currentType = ScreenType::EXIT;
            running = false;
        } else {
            std::cout << "\nInvalid choice! Input stream recovery active. Press Enter...";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
    shutdown();
}

void Application::shutdown() {
    saveSettings();
}