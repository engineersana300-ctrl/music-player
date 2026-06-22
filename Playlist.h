#pragma once
#include <string>
#include <vector>
#include "song.h"

class Playlist {
private:
    std::string name;
    std::vector<Song*> songs;
public:
    Playlist(const std::string& playlistName) : name(playlistName) {}
    
    void addSong(Song* song) {
        if (song) songs.push_back(song);
    }
    
    std::string getName() const { return name; }
    std::vector<Song*> getSongs() const { return songs; }
};