#pragma once
#include "grid.hpp"
#include "InterpolationWeights.hpp"

/**
* @brief Calculate progress of time for a Grid object, based on physics.
* 
* Usage: 
* Solver solver;
* solver.step(grid, dt);
*/
class Solver {
public:
    /**
    * @brief Calculate the next time step in the simulation.
    * 
    * @param grid   Grid object containing physical data. 
    * @param dt     Duration of a time step.
    */
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
    void updatePressure(Grid& grid, float dt);
    void updateVelocity(Grid& grid, float dt);
    void updateMass(Grid& grid, float dt);
};

