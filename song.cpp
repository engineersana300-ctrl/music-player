#include "song.h"
#include <iomanip>
#include <sstream>
using namespace std;

Song:: Song(const string& title,
        const string& artist,
        const string& album,
        const string& genre,
        int year,
        int duration,
        const string& filePath) {};

        string song::getTitle const {return title;}
        string song::getArtist const {return artist;}
        string song::getAlbum const {return album;}
        string song::getGenre const {return genre;}
        int    song::getDuration const {return duration;}
        int    song::getYear const {return year;}
        string song::getFilePath const {return filePath;}

        int song::providDuration() const{
            int min = duraton / 60;
            int sec = duration % 60;
            
            ostringstream oss;
            oss << setfill('0') << setw(2) << min
            << ":"
            << setfill('0') << setw(2) << sec;
            return oss.str();
        };