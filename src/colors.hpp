#pragma once

#include<raylib.h>
#include<vector>

inline const Color MY_BLACK = {0,0,0,255};

class Colors{
    public:
        static std::vector<Color> getColors();
};