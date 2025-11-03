#pragma once
#include "grid.hpp"

class Solver {
public:
    void step(Grid& grid, float dt);

private:
    void swapPointers(Grid& grid);
    void flowTransform(Grid& grid, float dt)
};

