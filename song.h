#pragma once

#include <iostream>
#include <string>

class Song
{
private:
    std::string title;
   std::string artist;
    std::string albume;
    std::string genere;
    int year;
    int duration;
    std::string filePath;
public:
    Song(const std::string& title,
        const std::string& artist,
        const std::string& album,
        const std::string& genre,
        int year,
        int duration,
        const std::string& filePath);
    
    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;
    std::string getGenre() const;
    int         getYear() const;
    int         getDuration() const;
    std::string GetFilePath() const;

    std::string providDuration() const;
};
