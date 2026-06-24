#define MINIAUDIO_IMPLEMENTATION
#include "Player.h"
#include <cstdlib>
#include <ctime>

Player::Player() : soundInitialized(false), activePlaylist(nullptr), 
                   currentSong(nullptr), currentSongIndex(-1), 
                   mode(PlaybackMode::NO_REPEAT), state(PlayerState::STOPPED) {
    ma_engine_init(NULL, &engine);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Player::~Player() {
    if (soundInitialized) {
        ma_sound_uninit(&sound);
    }
    ma_engine_uninit(&engine);
}

void Player::setPlaylist(Playlist* p) {
    stop();
    activePlaylist = p;
    currentSong = nullptr;
    currentSongIndex = -1;
}

void Player::loadSong(Song* song) {
    if (!song) return;
    if (soundInitialized) {
        ma_sound_uninit(&sound);
        soundInitialized = false;
    }

    // Append standard fallback data folder structural pathing if needed
    std::string fullPath = "..\\Data" + song->GetFilePath();
    ma_result result = ma_sound_init_from_file(&engine, fullPath.c_str(), 0, NULL, NULL, &sound);
    
    if (result == MA_SUCCESS) {
        soundInitialized = true;
        currentSong = song;
        if (activePlaylist) {
            for (size_t i = 0; i < activePlaylist->getSongs().size(); ++i) {
                if (activePlaylist->getSongs()[i] == song) {
                    currentSongIndex = static_cast<int>(i);
                    break;
                }
            }
        }
    } else {
        // If file missing skip next
        next();
    }
}

void Player::play() {
    if (!soundInitialized && currentSong) {
        loadSong(currentSong);
    }
    if (soundInitialized) {
        ma_sound_start(&sound);
        state = PlayerState::PLAYING;
    }
}

void Player::pause() {
    if (soundInitialized && state == PlayerState::PLAYING) {
        ma_sound_stop(&sound);
        state = PlayerState::PAUSED;
    }
}

void Player::resume() {
    if (state == PlayerState::PAUSED) {
        play();
    }
}

void Player::stop() {
    if (soundInitialized) {
        ma_sound_stop(&sound);
        ma_sound_seek_to_pcm_frame(&sound, 0);
        state = PlayerState::STOPPED;
    }
}

void Player::next() {
    if (!activePlaylist || activePlaylist->empty()) return;

    if (mode == PlaybackMode::REPEAT_ONE && currentSong) {
        loadSong(currentSong);
        play();
        return;
    }
    if (mode == PlaybackMode::SHUFFLE) {
        currentSongIndex = std::rand() % activePlaylist->getSongs().size();
    } else {
        currentSongIndex++;
        if (currentSongIndex >= static_cast<int>(activePlaylist->getSongs().size())) {
            if (mode == PlaybackMode::REPEAT_ALL) {
                currentSongIndex = 0;
            } else {
                stop();
                return;
            }
        }
    }
    loadSong(activePlaylist->getSongs()[currentSongIndex]);
    play();
}

void Player::prev() {
    if (!activePlaylist || activePlaylist->empty()) return;

    currentSongIndex--;
    if (currentSongIndex < 0) {
        currentSongIndex = mode == PlaybackMode::REPEAT_ALL ? static_cast<int>(activePlaylist->getSongs().size()) - 1 : 0;
    }
    loadSong(activePlaylist->getSongs()[currentSongIndex]);
    play();
}

void Player::tick() {
    if (state == PlayerState::PLAYING && soundInitialized) {
        if (ma_sound_at_end(&sound)) {
            next();
        }
    }
}

float Player::getCursorSec() const {
    if (!soundInitialized) return 0.0f;
    ma_uint64 frames = 0;
    ma_sound_get_cursor_in_pcm_frames(const_cast<ma_sound*>(&sound), &frames);
    return (float)frames / (float)ma_engine_get_sample_rate(const_cast<ma_engine*>(&engine));
}
