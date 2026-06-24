#pragma once
#include "Screen.h"
#include "Player.h"
#include "MusicLibrary.h"
#include <iomanip>
#include <vector>
#include <string>

// ============================================================================
// 1. MAIN MENU PANEL
// ============================================================================
class MainMenuScreen : public Screen {
private:
    Player& player;

public:
    MainMenuScreen(Player& p) : player(p) {}

    void render() override {
        clearScreen();
        std::cout << "=======================================\n";
        std::cout << "      TERMINAL MUSIC PLAYER v1.0       \n";
        std::cout << "=======================================\n";
        
        // Dynamic status bar based on current playback engine state
        if (player.getCurrentSong()) {
            std::cout << " Track Active: " << player.getCurrentSong()->getTitle() << "\n";
            std::cout << " Engine Status: " << (player.getPlayerState() == PlayerState::PLAYING ? "[PLAYING]" : "[PAUSED]") << "\n";
        } else {
            std::cout << " Engine Status: [STANDBY - NO TRACK LOADED]\n";
        }
        std::cout << "---------------------------------------\n";
        std::cout << " 1. Open 'Now Playing' Console\n";
        std::cout << " 2. Browse Loaded Playlists\n";
        std::cout << " 3. Exit Application\n";
        std::cout << "---------------------------------------\n";
        std::cout << "Selection: ";
    }

    ScreenType handleInput() override {
        std::string choice;
        std::cin >> choice;
        if (choice == "1") return ScreenType::NOW_PLAYING;
        if (choice == "2") return ScreenType::BROWSE_PLAYLISTS;
        if (choice == "3") return ScreenType::EXIT;
        
        // Bad input fallback: refresh the menu safely
        return ScreenType::MAIN_MENU;
    }
};

// ============================================================================
// 2. BROWSE PLAYLISTS PANEL
// ============================================================================
class BrowsePlaylistScreen : public Screen {
private:
    Player& player;
    MusicLibrary& library;
    std::vector<std::string> playlistCache; // Keeps track of numeric selection mapping

public:
    BrowsePlaylistScreen(Player& p, MusicLibrary& lib) : player(p), library(lib) {}

    void render() override {
        clearScreen();
        playlistCache.clear();
        
        std::cout << "=======================================\n";
        std::cout << "          EXPLORE LOCAL PLAYLISTS      \n";
        std::cout << "=======================================\n";

        int index = 1;
        for (const auto& pair : library.getPlaylists()) {
            std::cout << " " << index << ". " << pair.first 
                      << " [" << pair.second->getSongs().size() << " Tracks]\n";
            playlistCache.push_back(pair.first);
            index++;
        }

        if (playlistCache.empty()) {
            std::cout << " [System Alert: No .m3u files parsed in Data/Playlists/]\n";
        }

        std::cout << "---------------------------------------\n";
        std::cout << " Enter playlist number to load, or '0' to go back: ";
    }

    ScreenType handleInput() override {
        std::string choice;
        std::cin >> choice;
        try {
            int numericSelection = std::stoi(choice);
            if (numericSelection == 0) return ScreenType::MAIN_MENU;
            
            if (numericSelection > 0 && numericSelection <= static_cast<int>(playlistCache.size())) {
                std::string targetName = playlistCache[numericSelection - 1];
                Playlist* selectedPlaylist = library.getPlaylist(targetName);
                
                if (selectedPlaylist && !selectedPlaylist->getSongs().empty()) {
                    // Update player contextual references and kick off streaming audio
                    player.setPlaylist(selectedPlaylist);
                    player.loadSong(selectedPlaylist->getSongs()[0]);
                    player.play();
                    return ScreenType::NOW_PLAYING;
                }
            }
        } catch (...) {
            // Drop parsing errors silently and refresh safe UI
        }
        return ScreenType::BROWSE_PLAYLISTS;
    }
};

// ============================================================================
// 3. NOW PLAYING CONSOLE (WITH AUDIO TIMINGS)
// ============================================================================
class NowPlayingScreen : public Screen {
private:
    Player& player;

    void drawTimelineBar(int currentSeconds, int totalSeconds) {
        int barCharacterWidth = 24; 
        float completionRatio = totalSeconds > 0 ? (float)currentSeconds / (float)totalSeconds : 0.0f;
        int filledSegments = static_cast<int>(completionRatio * barCharacterWidth);

        std::cout << " Progress: [";
        for (int i = 0; i < barCharacterWidth; ++i) {
            if (i < filledSegments) std::cout << "=";
            else if (i == filledSegments) std::cout << ">";
            else std::cout << "-";
        }
        std::cout << "] ";
        
        // Output clean human-readable timestamps (MM:SS)
        std::cout << (currentSeconds / 60) << ":" << std::setw(2) << std::setfill('0') << (currentSeconds % 60)
                  << " / " 
                  << (totalSeconds / 60) << ":" << std::setw(2) << std::setfill('0') << (totalSeconds % 60) << "\n";
    }

public:
    NowPlayingScreen(Player& p) : player(p) {}

    void render() override {
        clearScreen();
        std::cout << "=======================================\n";
        std::cout << "         LIVE SOUND CONSOLE            \n";
        std::cout << "=======================================\n";

        Song* currentTrack = player.getCurrentSong();
        if (currentTrack) {
            std::cout << " Title:   " << currentTrack->getTitle() << "\n";
            std::cout << " Artist:  " << currentTrack->getArtist() << "\n";
            std::cout << " Album:   " << currentTrack->getAlbum() << "\n";
            std::cout << " Genre:   " << currentTrack->getGenre() << " (" << currentTrack->getYear() << ")\n";
            std::cout << "---------------------------------------\n";
            
            // Extract current audio driver PCM cursor timings
            int currentPlaybackSec = static_cast<int>(player.getCursorSec());
            drawTimelineBar(currentPlaybackSec, currentTrack->getDuration());
        } else {
            std::cout << " \n    [System Notice: Audio engine idle]\n\n";
        }

        std::cout << "---------------------------------------\n";
        std::cout << " [P] Pause   [R] Resume   [N] Next Track   [B] Back\n";
        std::cout << " Action: ";
    }

    ScreenType handleInput() override {
        std::string command;
        std::cin >> command;
        
        if (command == "p" || command == "P") player.pause();
        else if (command == "r" || command == "R") player.resume();
        else if (command == "n" || command == "N") player.next();
        else if (command == "b" || command == "B") return ScreenType::MAIN_MENU;
        
        return ScreenType::NOW_PLAYING;
    }
};