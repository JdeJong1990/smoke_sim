#include "solver.hpp"
#include <iostream>

void Solver::step(Grid& grid, float dt) {
    // Temporarily just write the old values to the new grid directly.
    // for (int x = 0; x<grid.getWidth(); x++){
    //     for (int y = 0; y<grid.getHeight(); y++){
    //         grid.smoke_next[grid.idx(x,y)] = grid.smoke[grid.idx(x,y)];
    //     }
    // }
    flowTransform(grid, dt);

    swapPointers(grid);
}

void Solver::swapPointers(Grid& grid){
    std::swap(grid.u, grid.u_next);
    std::swap(grid.v, grid.v_next);
    std::swap(grid.pressure, grid.pressure_next);
    std::swap(grid.smoke, grid.smoke_next);
    std::swap(grid.temperature, grid.temperature_next);
    std::swap(grid.mass, grid.mass_next);
}

//semi-Lagrangian advection
void applyAdvection(Grid& grid, float dt){
    weights = computeWeights(grid, dt);
    interpolateField(Grid, weights);
}

InterpolationWeights computeWeights(Grid& grid, float dt){
    
    InterpolationWeights weights;

    return weights;
}

void interpolateField(Grid& grid, InterpolationWeights weigths){
    //Interpolate between cells that influence the cell of interest.
}