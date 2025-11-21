#pragma once
#ifndef RGB_HPP
#define RGB_HPP
#include <cstdint>
#include <algorithm>

struct RGB {
    uint8_t r, g, b;

    // Constructor: one value (grayscale)
    RGB(int value) {
        uint8_t v = std::clamp(value, 0, 255);
        r = g = b = v;
    }

    // Constructor: three values (R, G, B)
    RGB(int r_in, int g_in, int b_in) {
        r = std::clamp(r_in, 0, 255);
        g = std::clamp(g_in, 0, 255);
        b = std::clamp(b_in, 0, 255);
    }
};


#endif