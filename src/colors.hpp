#pragma once

#include <raylib.h>
#include <vector>

// Custom black color definition
inline const Color MY_BLACK = {0, 0, 0, 255};

using namespace std;

// Utility class that provides the color palette for the game
class Colors {
public:
    // Returns a list of colors indexed by color ID
    static vector<Color> getColors();
};
