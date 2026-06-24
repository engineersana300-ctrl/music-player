#pragma once
#include "miniaudio.h"
#include "Song.h"
#include "Playlist.h"

enum class PlaybackMode { NO_REPEAT, REPEAT_ONE, REPEAT_ALL, SHUFFLE };
enum class PlayerState { PLAYING, PAUSED, STOPPED };

class Player {
private:
    ma_engine engine;
    ma_sound sound;
    bool soundInitialized;
    
    Playlist* activePlaylist;
    Song* currentSong;
    int currentSongIndex;
    
    PlaybackMode mode;
    PlayerState state;

public:
    Player();
    ~Player();

    void setPlaylist(Playlist* p);
    Playlist* getPlaylist() const { return activePlaylist; }
    
    void loadSong(Song* song);
    void play();
    void pause();
    void resume();
    void stop();
    void next();
    void prev();
    void tick();

    void setPlaybackMode(PlaybackMode m) { mode = m; }
    PlaybackMode getPlaybackMode() const { return mode; }
    PlayerState getPlayerState() const { return state; }
    Song* getCurrentSong() const { return currentSong; }
    float getCursorSec() const;
};