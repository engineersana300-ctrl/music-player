#pragma once
#include <string>
#include <vector>
#include "song.h"

class MusicLibrary {
private:
    std::vector<Song*> songs;
    bool containsIgnoreCase(const std::string& str, const std::string& query) const;
public:
    MusicLibrary();
    ~MusicLibrary();

    void addSong(const Song& song);
    Song* getSong(int index);
    std::vector<Song*> getAllSongs() const;
    std::vector<Song*> filterByArtist(const std::string& artist) const;
    std::vector<Song*> filterByAlbum(const std::string& album) const;
    std::vector<Song*> filterByTitle(const std::string& title) const;
    std::vector<Song*> search(const std::string& query) const;

};