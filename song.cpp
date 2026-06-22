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
    : title(title), artist(artist), albume(album), genere(genre), 
      year(year), duration(duration), filePath(filePath) {}

string Song::getTitle() const { return title; }
string Song::getArtist() const { return artist; }
string Song::getAlbum() const { return albume; }
string Song::getGenre() const { return genere; }
int Song::getYear() const { return year; }
int Song::getDurationSec() const { return duration; }
string Song::GetFilePath() const { return filePath; }

string Song::providDuration() const {
    int min = duration / 60;
    int sec = duration % 60;
    
    ostringstream oss;
    oss << setfill('0') << setw(2) << min
        << ":"
        << setfill('0') << setw(2) << sec;
    return oss.str();
}
