#pragma once

struct Position {
    int x = 0;
    int y = 0;

    Position() = default;              // needed for arrays/vectors default init
    Position(int x, int y);
};
