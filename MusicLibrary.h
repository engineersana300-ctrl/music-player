#pragma once
#include <string>
#include <vector>
#include <map>
#include "song.h"

class Playlist;

class MusicLibrary {
private:
    std::vector<Song*> songs;
    std::map<std::string, Playlist*> playlists;
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

    Song* findSongByPath(const std::string& path) const;
    void addPlaylist(Playlist* playlist);
    const std::map<std::string, Playlist*>& getPlaylists() const;
    Playlist* getPlaylist(const std::string& name) const;
};
