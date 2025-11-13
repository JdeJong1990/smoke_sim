#include "renderer.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>

Renderer::Renderer(int width, int height, int cellSize)
    : window(sf::VideoMode({static_cast<unsigned int>(width * cellSize),
                            static_cast<unsigned int>(height * cellSize)}),
             "Grid Simulation"),
      cellSize(cellSize)
{
    window.setFramerateLimit(1);           // Set frame rate limit
}

void Renderer::pollEvents() {
    while (auto eventOpt = window.pollEvent()) {
        const auto& event = *eventOpt;
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Renderer::draw(const Grid& grid) {
    window.clear(sf::Color::Black);

    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            sf::RectangleShape cellShape(
                sf::Vector2f(static_cast<float>(cellSize),
                             static_cast<float>(cellSize))
            );
            cellShape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));

            float value = grid.smoke[grid.idx(x, grid.getHeight() -1 - y)];
            uint8_t colorValue = static_cast<uint8_t>(value * 255);
            cellShape.setFillColor(sf::Color(colorValue, colorValue, colorValue));

            window.draw(cellShape);
        }
    }

    window.display();
}
