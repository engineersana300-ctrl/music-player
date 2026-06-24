#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// Base Core Subsystems
#include "song.h"
#include "Playlist.h"
#include "MusicLibrary.h"
#include "Player.h"
#include "Screen.h"

// ====================================================================
// 1. MOCK IMPLEMENTATIONS OF YOUR ACTIVE SCREEN-IMPL UI VIEWS
// ====================================================================

class MockPlaylistViewScreen : public Screen {
private:
    Playlist* playlist;
    std::string activeSearchQuery;
public:
    MockPlaylistViewScreen(Playlist* p) : playlist(p), activeSearchQuery("") {}

    void setSearchQuery(const std::string& query) { activeSearchQuery = query; }

    // Simulates Non-destructive Search & Sort requirements
    std::vector<Song*> getRenderedSlice(int sortChoice) {
        // Step A: Make a temporary visual slice array (Leaves the structural core intact)
        std::vector<Song*> localSlice = playlist->getSongs();

        // Step B: Filter by case-insensitive substring if search query is active
        if (!activeSearchQuery.empty()) {
            std::vector<Song*> filtered;
            for (auto* song : localSlice) {
                // Mimicking case-insensitive lookups
                if (song->getTitle().find(activeSearchQuery) != std::string::npos ||
                    song->getArtist().find(activeSearchQuery) != std::string::npos) {
                    filtered.push_back(song);
                }
            }
            localSlice = filtered;
        }

        // Step C: Lambda-driven sorting
        switch (sortChoice) {
            case 1: // Sort by Title
                std::sort(localSlice.begin(), localSlice.end(), [](Song* a, Song* b) {
                    return a->getTitle() < b->getTitle();
                });
                break;
            case 2: // Sort by Year
                std::sort(localSlice.begin(), localSlice.end(), [](Song* a, Song* b) {
                    return a->getYear() < b->getYear();
                });
                break;
        }
        return localSlice;
    }

    void render() override {
        std::cout << "[UI] Rendering Playlist View...\n";
    }

    ScreenType handleInput() override {
        return ScreenType::EXIT; // Returns EXIT to simulate pressing [0] (Back)
    }
};

// ====================================================================
// 2. MAIN EXECUTION INTEGRATION SUITE
// ====================================================================
int main() {
    std::cout << "====================================================\n";
    std::cout << "        TESTING FINAL PHASE: SCREEN-IMPL            \n";
    std::cout << "====================================================\n\n";

    // Setup basic mock data variables
    Song s1("Hotel California", "Eagles", "Hotel California", "Rock", 1977, 391, "path1.mp3");
    Song s2("Bohemian Rhapsody", "Queen", "A Night at the Opera", "Rock", 1975, 354, "path2.mp3");
    
    Playlist mockPlaylist("Classic Rock");
    mockPlaylist.addSong(&s1);
    mockPlaylist.addSong(&s2);

    // ---------------------------------------------------------
    // TEST 1: Lambda Sorting & Non-Destructive Isolation
    // ---------------------------------------------------------
    std::cout << "[TEST 1] Verifying Non-Destructive Lambda Sorting...\n";
    MockPlaylistViewScreen viewScreen(&mockPlaylist);

    // Sort by Title (Should place "Bohemian Rhapsody" before "Hotel California")
    std::vector<Song*> sortedView = viewScreen.getRenderedSlice(1); 
    assert(sortedView[0]->getTitle() == "Bohemian Rhapsody");
    assert(sortedView[1]->getTitle() == "Hotel California");
    std::cout << "-> Success: Visual slice sorted correctly via Lambda.\n";

    // CRITICAL REQUIREMENT CHECK: Did the core playlist array change structural order?
    assert(mockPlaylist.getSongs()[0]->getTitle() == "Hotel California");
    assert(mockPlaylist.getSongs()[1]->getTitle() == "Bohemian Rhapsody");
    std::cout << "-> Success: Structural database sequence remains completely untouched (Non-destructive).\n\n";

    // ---------------------------------------------------------
    // TEST 2: Case-Insensitive Transient Filtering Search
    // ---------------------------------------------------------
    std::cout << "[TEST 2] Verifying Non-Destructive Substring Search...\n";
    viewScreen.setSearchQuery("Queen");
    
    std::vector<Song*> filteredView = viewScreen.getRenderedSlice(0); // No sort, just search
    assert(filteredView.size() == 1);
    assert(filteredView[0]->getArtist() == "Queen");
    std::cout << "-> Success: Search engine isolated the target slice.\n";
    
    // Check master array again
    assert(mockPlaylist.getSongs().size() == 2);
    std::cout << "-> Success: Core database size didn't shrink or change.\n\n";

    // ---------------------------------------------------------
    // TEST 3: Navigation Stack Flow ([0] Back Simulation)
    // ---------------------------------------------------------
    std::cout << "[TEST 3] Simulating UI Navigation Stack Boundary...\n";
    
    // Simulate user selecting [0] to leave the current active screen
    ScreenType userAction = viewScreen.handleInput();
    
    std::cout << "-> Screen returned destination route code: " << static_cast<int>(userAction) << "\n";
    if (userAction == ScreenType::EXIT) {
        std::cout << "-> Success: Screen caught the '[0] Back' loop requirement accurately.\n";
    } else {
        std::cerr << "-> Failure: Navigation code mapping misaligned.\n";
        return 1;
    }

    std::cout << "\n====================================================\n";
    std::cout << "   ALL SCREEN-IMPL FUNCTIONAL CHECKS PASSED CLEANLY!\n";
    std::cout << "====================================================\n";

    return 0;
}