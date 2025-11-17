#pragma once
#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include "inputHandler.hpp"

class Renderer {
public:
    Renderer(int width, int height, int cellSize = 4);
    void draw(const Grid& grid, InputHandler& input);
    void pollEvents();

    bool isOpen() const { return window.isOpen(); }

    sf::Window& getWindow();

private:
    sf::RenderWindow window;
    int cellSize;
};
