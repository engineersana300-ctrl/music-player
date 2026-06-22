#include "M3uLoader.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void M3uLoader::loadAll(const std::string& directoryPath, MusicLibrary& library) {
    if (!fs::exists(directoryPath)) return;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".m3u") {
            std::string playlistName = entry.path().stem().string();
            std::ifstream file(entry.path());
            if (!file.is_open()) continue;

            Playlist* playlist = new Playlist(playlistName);
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;
                
                Song* targetSong = library.findSongByPath(line);
                if (targetSong) {
                    playlist->addSong(targetSong);
                }
            }
            library.addPlaylist(playlist);
        }
    }
}