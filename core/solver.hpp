#pragma once
#include "grid.hpp"
#include "InterpolationWeights.hpp"

class Solver {
public:
    void step(Grid& grid, float dt);

private:
    void swapPointers(Grid& grid);
    void applyAdvection(Grid& grid, float dt);
    InterpolationWeights computeWeights(int x, int y, Grid& grid, float dt);
    void interpolateFields(Grid& grid, InterpolationWeights weigths);
    void interpolateField(std::vector<float>& field, 
                                std::vector<float>& field_next,     
                                InterpolationWeights weights, 
                                Grid& grid);
    void updatePressure( Grid& grid, float dt);
};

