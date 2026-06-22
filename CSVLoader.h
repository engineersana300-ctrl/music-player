#pragma once

#include <string>
#include "MusicLibrary.h"

class CSVLoader {
public:
    void load(const std::string& path, MusicLibrary& library);
};