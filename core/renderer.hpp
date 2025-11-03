#pragma once
#include <SFML/Graphics.hpp>
#include "grid.hpp"

class Renderer {
public:
    Renderer(int width, int height, int cellSize = 4);
    void draw(const Grid& grid);
    void pollEvents();

    bool isOpen() const { return window.isOpen(); }

private:
    sf::RenderWindow window;
    int cellSize;
};
