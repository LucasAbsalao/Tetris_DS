#include "colors.hpp"

std::vector<Color> Colors::getColors(){
    std::vector<Color> c = {
        MY_BLACK, DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };
    return c;
}