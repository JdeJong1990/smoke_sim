#include "InterpolationWeights.hpp"
#include "solver.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

void Solver::step(Grid& grid, float dt) {
    // Temporarily just write the old values to the new grid directly.
    // for (int x = 0; x<grid.getWidth(); x++){
    //     for (int y = 0; y<grid.getHeight(); y++){
    //         grid.smoke_next[grid.idx(x,y)] = grid.smoke[grid.idx(x,y)];
    //     }
    // }
    applyAdvection(grid, dt);

    updatePressure(grid, dt);

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
void Solver::applyAdvection(Grid& grid, float dt){
    for (int y = 0; y < grid.getHeight(); y++){
        for (int x = 0; x < grid.getWidth(); x++){
            InterpolationWeights weights = computeWeights(x, y, grid, dt);
            interpolateFields(grid, weights);
        }
    }
}

InterpolationWeights Solver::computeWeights(int x, int y, Grid& grid, float dt){
    float factor = 100.0;
    float u = grid.u[grid.idx(x,y)];
    float v = grid.v[grid.idx(x,y)];

    float x_new = x - u*dt/grid.getCellScale() * factor;
    float y_new = y - v*dt/grid.getCellScale() * factor;

    if (x == 32 && y == 32){
        std::cout <<"x: " << x << "\n";
        std::cout <<"u: " << u << "\n";
        std::cout <<"dt: "<< dt << "\n";
        std::cout <<"cell scale: " << grid.getCellScale() << "\n";
        std::cout <<"xnew: "<<x_new<<"\n\n";
    }
    float width = grid.getWidth();      //Define as float
    float height = grid.getHeight();

    x_new = std::max(x_new, 0.0f);
    x_new = std::min(x_new, width - 1.0f);
    y_new = std::max(y_new, 0.0f);
    y_new = std::min(y_new, height - 1.0f);

    InterpolationWeights weights;

    weights.x = x;
    weights.y = y;

    weights.x0 = static_cast<int>(std::floor(x_new));
    weights.y0 = static_cast<int>(std::floor(y_new));
    weights.x1 = static_cast<int>(std::min(weights.x0 + 1, grid.getWidth() - 1));
    weights.y1 = static_cast<int>(std::min(weights.y0 + 1, grid.getHeight() - 1));

    float dx = x_new - weights.x0;
    float dy = y_new - weights.y0;

    weights.w00 = (1 - dx) * (1 - dy); 
    weights.w01 = (1 - dx) * dy;
    weights.w10 = dx * (1 - dy);
    weights.w11 = dx * dy;

    return weights;
}

void Solver::interpolateFields(Grid& grid, InterpolationWeights weights){
    //Interpolate between cells four that influence the cell of interest. 
    interpolateField(grid.u, grid.u_next, weights, grid);
    interpolateField(grid.v, grid.v_next, weights, grid);
    interpolateField(grid.smoke, grid.smoke_next, weights, grid);
    interpolateField(grid.temperature, grid.temperature_next, weights, grid);
    interpolateField(grid.mass, grid.mass_next, weights, grid);
    
    if (weights.x == 32 && weights.y == 32) {
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Coordinate: " << weights.x << "," << weights.y << "\n"
                << "("<<weights.x0<<","<<weights.x1<<","<<weights.y0<<","<<weights.y1<<")\n"
                << "Weights: " << weights.w00 << ", " << weights.w01
                << ", " << weights.w10 << ", " << weights.w11 << "\n";
    }
}

void Solver::interpolateField(  std::vector<float>& field, 
                                std::vector<float>& field_next,     
                                InterpolationWeights weights, 
                                Grid& grid){
    int x = weights.x;
    int y = weights.y;

    field_next[grid.idx(x,y)] = weights.w00*field[grid.idx(weights.x0, weights.y0)]+
                                weights.w01*field[grid.idx(weights.x0, weights.y1)]+
                                weights.w10*field[grid.idx(weights.x1, weights.y0)]+
                                weights.w11*field[grid.idx(weights.x1, weights.y1)];   
}

void Solver::updatePressure( Grid& grid, float dt){
    for (int y = 0; y < grid.getHeight(); y++){
        for (int x = 0; x < grid.getWidth(); x++){
            int index = grid.idx(x,y);
            float volume = std::pow(grid.getCellScale(), 3.0f);
            grid.pressure[index] = grid.mass[index] / volume * grid.temperature[index];
        }
    }
}