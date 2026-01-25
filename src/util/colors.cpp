#include "colors.hpp"

// Returns the color palette used by the game
std::vector<Color> Colors::getColors() {
    // Color list indexed by color ID
    std::vector<Color> c = {
        MY_BLACK,     // background / empty cell
        DARKGRAY,
        MAROON,
        ORANGE,
        DARKGREEN,
        DARKBLUE,
        DARKPURPLE,
        DARKBROWN,
        GRAY,
        RED,
        GOLD,
        LIME,
        BLUE,
        VIOLET,
        BROWN,
        LIGHTGRAY,
        PINK,
        YELLOW,
        GREEN,
        SKYBLUE,
        PURPLE,
        BEIGE
    };

    // Return the palette
    return c;
}
