#pragma once
#include <vector>
#include <iostream>

/**
* @brief Store physical data of every cell of the simulation. 
*/
class Grid {
public:
    Grid(int width, int height, float cellScale);

    /// @return The width of the grid in cells.
    int getWidth() const { return width_; }

    /// @return The height of the grid in cells. 
    int getHeight() const { return height_; }

    /// @return The scale that a squrare cell represents in meters. 
    float getCellScale() const{ return cellScale_; }
    
    /// @return The index of an element in a 1D array that represents cell (x,y).
    inline int idx(int x, int y) const { return y * width_ + x; }

    std::vector<float> u;           // horizontal velocity
    std::vector<float> v;           // vertical velocity
    std::vector<float> pressure;
    std::vector<float> smoke;
    std::vector<float> temperature;
    std::vector<float> mass;

    std::vector<float> u_next;
    std::vector<float> v_next;
    std::vector<float> pressure_next;
    std::vector<float> smoke_next;
    std::vector<float> temperature_next;
    std::vector<float> mass_next;

private:
    float cellScale_;                // Width and height of a square cell in meters
    int width_;
    int height_;
};
