#pragma once

struct InterpolationWeights {
    int x, y;
    int x0, x1, y0, y1;
    float w00, w01, w10, w11;
};