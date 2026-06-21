#include "song.h" // Ensure this matches the exact casing of your file name (e.g., Song.h)
#include <iomanip>
#include <sstream>

using namespace std;

// Fixed: Added a member initializer list so variables actually save!
Song::Song(const string& title,
           const string& artist,
           const string& album,
           const string& genre,
           int year,
           int duration,
           const string& filePath) 
    : title(title), artist(artist), album(album), genre(genre), year(year), duration(duration), filePath(filePath) {}

// Fixed: Changed 'song::' to 'Song::' and added missing '()'
string Song::getTitle() const { return title; }
string Song::getArtist() const { return artist; }
string Song::getAlbum() const { return album; }
string Song::getGenre() const { return genre; }
int    Song::getDuration() const { return duration; }
int    Song::getYear() const { return year; }
string Song::getFilePath() const { return filePath; }

// Fixed: Changed return type from 'int' to 'string' to allow "MM:SS" format
string Song::providDuration() const {
    int min = duration / 60; // Fixed typo: 'duraton' -> 'duration'
    int sec = duration % 60;
    
    ostringstream oss;
    oss << setfill('0') << setw(2) << min
        << ":"
        << setfill('0') << setw(2) << sec;
    return oss.str();
}