#include "InterpolationWeights.hpp"
#include "solver.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

void Solver::step(Grid& grid, float dt) {
    applyAdvection(grid, dt);

    updatePressure(grid, dt);

    updateVelocity(grid, dt);

    updateMass(grid, dt);

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
    float factor = 10.0;
    float u = grid.u[grid.idx(x,y)];
    float v = grid.v[grid.idx(x,y)];
    if (u>10 || u < -10 || v> 10 || v<-10){
        std::cout <<"Velocity is: u=" << u << ", v = " << v << "\n";
    }
    float x_new = x - u*dt/grid.getCellScale() * factor;
    float y_new = y - v*dt/grid.getCellScale() * factor;
    if ((x_new - x) >1.0f || (y_new -y)>1.0f){
        std::cout << "WARNING: interpolation length to large in Solver::computeWeights.("<< x <<" , " << y <<")\n";
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

    if (x > grid.getWidth()|| weights.x0  > grid.getWidth()|| weights.x1 > grid.getWidth()|| weights.y0 > grid.getHeight()|| weights.y1 > grid.getHeight() ){
        std::cout << "WARNING: interpolationWeights contains an out of bounds value: x = " << x << ", y = " << y  << ", x0 = " << weights.x0 << ", x1 = " << weights.x1 << ", y0 = " << weights.y0 << ", y1 = " << weights.y1 <<"\n";
        std::cout <<"Velocity is: u=" << u << ", v = " << v << "\n";
    }
    if (x < 0 || y < 0 || weights.x0 < 0 || weights.x1 < 0 || weights.y0 < 0 || weights.y1 < 0){
        std::cout << "WARNING: interpolationWeights contains an out of bounds value: x = " << x << ", y = " << y  << ", x0 = " << weights.x0 << ", x1 = " << weights.x1 << ", y0 = " << weights.y0 << ", y1 = " << weights.y1 <<"\n";
        std::cout <<"Velocity is: u=" << u << ", v = " << v << "\n";
    }

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
    float R = 0.01f;
    float volume = std::pow(grid.getCellScale(), 3.0f);
    for (int y = 0; y < grid.getHeight(); y++){
        for (int x = 0; x < grid.getWidth(); x++){
            int index = grid.idx(x,y);
            grid.pressure[index] = grid.mass[index] * R / volume * grid.temperature[index];
        }
    }
}

void Solver::updateVelocity(Grid& grid, float dt){
    for (int y = 0; y < grid.getHeight(); y++){
        for (int x = 0; x < grid.getWidth(); x++){
            int current = grid.idx(x , y);
            int right = grid.idx((x+1) % grid.getWidth(), y);
            int up = grid.idx(x, (y+1) % grid.getHeight());

            if(current<0 || right <0 || up <0|| current> grid.getWidth()*grid.getHeight()|| right> grid.getWidth()*grid.getHeight()|| up> grid.getWidth()*grid.getHeight()){
                std::cout << "Index out of bounds in updateVelocity! x=" << x << " y=" << y 
                    << " current=" << current << " right=" << right 
                    << " up=" << up << "\n";
            }
            grid.u_next[current] = grid.u[current] + (grid.pressure[current] - grid.pressure[right]) / std::max(grid.mass[current], 0.01f) * grid.getCellScale() * grid.getCellScale() * dt;
            grid.v_next[current] = grid.v[current] + (grid.pressure[current] - grid.pressure[up]) / std::max(grid.mass[current], 0.01f) * grid.getCellScale() * grid.getCellScale() * dt;
        }
    }
}

void Solver::updateMass(Grid& grid, float dt){
    float total_mass = 0.0f;
    float max_mass = 0.0f;

    for (int y = 0; y < grid.getHeight(); y++){
        for (int x = 0; x < grid.getWidth(); x++){
            int current = grid.idx(x , y);
            int right = grid.idx((x+1) % grid.getWidth(), y);
            int left = grid.idx((x-1 + grid.getWidth()) % grid.getWidth(), y);
            int up = grid.idx(x, (y+1) % grid.getHeight());
            int down = grid.idx(x, (y-1 + grid.getHeight()) % grid.getHeight());
            
            if(current<0 || left <0 || down <0|| current> grid.getWidth()*grid.getHeight()|| left > grid.getWidth()*grid.getHeight()|| down> grid.getWidth()*grid.getHeight()){
                std::cout << "Out of bounds! x=" << x << " y=" << y 
                    << " current=" << current << " left=" << left 
                    << " down=" << down << "\n";
            }

            float max_right = std::min(grid.mass[right] , grid.mass[current]) / 8.0f;
            float max_left = std::min(grid.mass[current] , grid.mass[left]) / 8.0f;
            float max_up = std::min(grid.mass[up] , grid.mass[current]) / 8.0f;
            float max_down = std::min(grid.mass[current] , grid.mass[down]) / 8.0f;
            
            grid.mass_next[current] = grid.mass[current] + (
                                        + std::clamp(std::max(grid.mass[current], grid.mass[left]) * grid.u[left] * dt, -max_left, max_left)
                                        - std::clamp(std::max(grid.mass[current], grid.mass[right]) * grid.u[current] * dt, -max_right, max_right)
                                        + std::clamp(std::max(grid.mass[current], grid.mass[down]) * grid.v[down] * dt, -max_down, max_down)
                                        - std::clamp(std::max(grid.mass[current], grid.mass[up]) * grid.v[current] * dt, -max_up, max_up)) / grid.getCellScale();
            
            grid.mass_next[current] = std::clamp(grid.mass_next[current], 0.0f, 10.0f);
            max_mass = std::max(max_mass, grid.mass_next[current]);
            total_mass = total_mass + grid.mass_next[current];
        }
    }
    std::cout << "Total mass: " << total_mass << "     Max: " << max_mass << "\r";
}