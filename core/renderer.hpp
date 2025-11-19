#pragma once
#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include "inputHandler.hpp"

class Renderer {
public:
    Renderer(int width, int height, int cellSize = 4);
    void draw(const Grid& grid, InputHandler& input);
    void updateMode(InputHandler& input);
    float sampleField(int x, int y, const Grid& grid);
    void pollEvents();

    bool isOpen() const { return window.isOpen(); }

    char render_mode;
    sf::Window& getWindow();

private:
    sf::RenderWindow window;
    int cellSize;
};
