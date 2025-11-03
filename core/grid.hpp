#pragma once
#include <vector>

class Grid {
public:
    Grid(int width, int height);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

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
    int width_;
    int height_;
};
