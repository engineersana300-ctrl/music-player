#include "song.h"
#include <iomanip>
#include <sstream>

using namespace std;

Song::Song(const string& title,
           const string& artist,
           const string& album,
           const string& genre,
           int year,
           int duration,
           const string& filePath) 
    : title(title), artist(artist), album(album), genre(genre), year(year), duration(duration), filePath(filePath) {}

string Song::getTitle() const { return title; }
string Song::getArtist() const { return artist; }
string Song::getAlbum() const { return album; } // Fixed: albume -> album
string Song::getGenre() const { return genre; } // Fixed: genere -> genre
int    Song::getDuration() const { return duration; } // Fixed: getDurationSec -> getDuration
int    Song::getYear() const { return year; }
string Song::getFilePath() const { return filePath; } // Fixed: GetFilePath -> getFilePath

string Song::providDuration() const {
    int min = duration / 60;
    int sec = duration % 60;
    
    ostringstream oss;
    oss << setfill('0') << setw(2) << min
        << ":"
        << setfill('0') << setw(2) << sec;
    return oss.str();
}
