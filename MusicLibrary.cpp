#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "MusicLibrary.h"
#include "song.h"
#include "Playlist.h"

using namespace std;

MusicLibrary::MusicLibrary() {}

void MusicLibrary::addSong(const Song& song) {
    songs.push_back(new Song(song));
}

bool MusicLibrary::containsIgnoreCase(const std::string& str, const std::string& query) const {
    std::string lowerStr = str;
    std::string lowerQuery = query;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerStr.find(lowerQuery) != std::string::npos;
}

vector<Song*> MusicLibrary::filterByArtist(const string& artist) const {
    vector<Song*> result;
    for (Song* song : songs) {
        if (song->getArtist() == artist)
            result.push_back(song);
    }
    return result;
}

vector<Song*> MusicLibrary::filterByAlbum(const string& album) const {
    vector<Song*> result;
    for (Song* song : songs) {
        if (song->getAlbum() == album)
            result.push_back(song);
    }
    return result;
}

vector<Song*> MusicLibrary::filterByTitle(const string& title) const {
    vector<Song*> result;
    for (Song* song : songs) {
        if (containsIgnoreCase(song->getTitle(), title))
            result.push_back(song);
    }
    return result;
}

vector<Song*> MusicLibrary::search(const string& query) const {
    vector<Song*> result;
    for (Song* song : songs) {
        if (containsIgnoreCase(song->getTitle(), query)
            || containsIgnoreCase(song->getAlbum(), query)
            || containsIgnoreCase(song->getArtist(), query)) {
            result.push_back(song);
        }
    }
    return result;
}

Song* MusicLibrary::getSong(int index) {
    if (index < 0 || index >= (int)songs.size())
        return nullptr;
    return songs[index];
}

vector<Song*> MusicLibrary::getAllSongs() const {
    return songs;
}

Song* MusicLibrary::findSongByPath(const std::string& path) const {
    for (Song* song : songs) {
        if (song->getFilePath() == path) {
            return song;
        }
    }
    return nullptr;
}

void MusicLibrary::addPlaylist(Playlist* playlist) {
    if (playlist) {
        playlists[playlist->getName()] = playlist;
    }
}

const std::map<std::string, Playlist*>& MusicLibrary::getPlaylists() const {
    return playlists;
}

Playlist* MusicLibrary::getPlaylist(const std::string& name) const {
    auto it = playlists.find(name);
    if (it != playlists.end()) {
        return it->second;
    }
    return nullptr;
}

MusicLibrary::~MusicLibrary() {
    for (auto* song : songs) delete song;
    for (auto& pair : playlists) delete pair.second;
}
