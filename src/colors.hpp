#pragma once

#include<raylib.h>
#include<vector>

inline const Color MY_BLACK = {0,0,0,255};

using namespace std;

class Colors{
    public:
        static vector<Color> getColors();
};