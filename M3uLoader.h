#pragma once
#include "MusicLibrary.h"
#include <string>

class M3uLoader {
public:
    static void loadAll(const std::string& directoryPath, MusicLibrary& library);
};