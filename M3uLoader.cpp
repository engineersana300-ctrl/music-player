#include "M3uLoader.h"
#include "Playlist.h"
#include <iostream>
#include <fstream>
#include <dirent.h>   // Universal directory parsing
#include <sys/stat.h> // Check if directory exists

void M3uLoader::loadAll(const std::string& directoryPath, MusicLibrary& library) {
    // 1. Check if the directory exists using POSIX stat
    struct stat info;
    if (stat(directoryPath.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        std::cerr << "[Warning] Playlist directory does not exist: " << directoryPath << "\n";
        return;
    }

    // 2. Open the directory stream
    DIR* dir = opendir(directoryPath.c_str());
    if (!dir) return;

    struct dirent* entry;
    // 3. Loop through all files inside the directory
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;

        // Filter out parent links "." / ".." and ensure file ends with ".m3u"
        if (fileName != "." && fileName != ".." && 
            fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".m3u") {
            
            std::string fullM3uPath = directoryPath + "/" + fileName;
            
            // Extract the playlist name (filename without the ".m3u" extension)
            std::string playlistName = fileName.substr(0, fileName.size() - 4);
            
            // Create a new playlist allocation instance
            Playlist* newPlaylist = new Playlist(playlistName);
            
            // Read lines inside the M3U file
            std::ifstream file(fullM3uPath);
            std::string trackPath;
            while (std::getline(file, trackPath)) {
                // Clear out carriage return markers if written on Windows notepad layouts
                if (!trackPath.empty() && trackPath.back() == '\r') {
                    trackPath.pop_back();
                }
                
                if (trackPath.empty()) continue;

                // Query the central library database to find matching path pointers
                Song* targetSong = library.findSongByPath(trackPath);
                if (targetSong) {
                    newPlaylist->addSong(targetSong);
                }
            }
            file.close();

            // Register the parsed playlist securely into the library
            library.addPlaylist(newPlaylist);
        }
    }
    closedir(dir);
}