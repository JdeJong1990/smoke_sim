#include "grid.hpp"
#include <cmath>
#include <iostream>

Grid::Grid(int width, int height, float cellScale)
    : width_(width), height_(height),
    cellScale_(cellScale),
    u(width * height, 0.0f),
    v(width * height, 0.0f),
    pressure(width * height, 0.0f),
    smoke(width * height, 0.0f),
    temperature(width * height, 293.0f),
    mass(width * height, 0.5f),
    u_next(width * height, 0.0f),
    v_next(width * height, 0.0f),
    smoke_next(width * height, 0.0f),
    temperature_next(width * height, 0.0f),
    mass_next(width * height, 1.0f)
{
  //Set some inittial conditions that help to visualize.
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int index = idx(x,y);
        float value = std::exp(-0.02f*((x-width/2)*(x-width/2)+(y-height/2)*(y-height/2)));

        if (std::sin(x*0.4)>0){
          smoke[idx(x, y)] = 1.0f;
        }

        float factor = 0.1;
        float radius = std::sqrt((x-width/2)*(x-width/2) + (y-height/2)*(y-height/2));

        mass[index] = 0.5f + value;
        // v[idx(x,y)] = -(x - width/2.0f)*factor*value*radius;
        // u[idx(x,y)] = (y - height/2.0f)*factor*value*radius;
    }
  }
}
