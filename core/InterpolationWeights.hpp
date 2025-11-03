#pragma once

class InterpolationWeights {
public:
    Weights();

    int x0;
    int x1;
    int y0;
    int y1;

    float w00;
    float w01;
    float w10;
    float w11;
}