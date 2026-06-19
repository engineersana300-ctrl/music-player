#include <iostream>
#include <fstream>
#include <sstream>
#include "song.h"
#include "CSVLoader.h"
using namespace std;

void CSVLoader :: load (const path& string , MusicLibrary& library) {
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: could not open file" << endl;
        return;
    }

    string line;
    string headerLine;
    getline(file, headerLine); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string title, artist, album, genre, filePath;
        string yearStr, durationStr;
        int year, duration;

        getline(ss, title, ',');
        if (title.empty()) {
            cerr << "Warning: no title found in row: " << line << "\n";
            continue;
        }

        getline(ss, artist, ',');
        if (artist.empty()) {
            cerr << "Warning: no artist found in row: " << line << "\n";
            continue;
        }

        getline(ss, album, ',');
        getline(ss, genre, ',');

        getline(ss, yearStr, ',');
        try {
            year = stoi(yearStr);
        }
        catch (invalid_argument&) {
            cerr << "Warning: invalid year in row: " << line << "\n";
            continue;
        }

        getline(ss, durationStr, ',');
        try {
            duration = stoi(durationStr);
        }
        catch (invalid_argument&) {
            cerr << "Warning: invalid duration in row: " << line << "\n";
            continue;
        }

        getline(ss, filePath, ',');
        if (filePath.empty()) {
            cerr << "Warning: no filePath in row: " << line << "\n";
            continue;
        }

        Song song(title, artist, album, genre, year, duration, filePath);
        library.addSong(song);
    }

    file.close();
}