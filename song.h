#pragma once
#include <string>

class Song {
private:
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int year;
    int duration; // Total duration in seconds
    std::string filePath;

public:
    // Constructor
    Song(const std::string& title,
         const std::string& artist,
         const std::string& album,
         const std::string& genre,
         int year,
         int duration,
         const std::string& filePath);

    // Constant Getter Methods
    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;
    std::string getGenre() const;
    int getDuration() const;
    int getYear() const;
    std::string getFilePath() const;

    // Utility to return duration formatted as a "MM:SS" string
    std::string providDuration() const;
};