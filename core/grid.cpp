#include "grid.hpp"
#include <iostream>

Grid::Grid(int width, int height)
    : width_(width), height_(height),
    cellScale_(cellScale),
    u(width * height, 0.0f),
    v(width * height, 0.0f),
    pressure(width * height, 0.0f),
    smoke(width * height, 0.0f),
    temperature(width * height, 0.0f),
    mass(width * height, 1.0f),
    u_next(width * height, 0.0f),
    v_next(width * height, 0.0f),
    pressure_next(width * height, 0.0f),
    smoke_next(width * height, 0.0f),
    temperature_next(width * height, 0.0f),
    mass_next(width * height, 1.0f)
{
  //Set some inittial conditions that help to visualize.
  for (int y = height/2 - 10; y < height/2+10; y++) {
      for (int x = width/2 - 10; x < width/2+10; x++) {
        smoke[idx(x, y)] = 1.0f;
        u[idx(x,y)] = 1.0f;
    }
  }
}
