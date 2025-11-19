#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "inputHandler.hpp"
#include "renderer.hpp"
#include <cstdint>
#include <iostream>

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

void Renderer::draw(const Grid& grid, InputHandler& input) {
    updateMode(input);

    window.clear(sf::Color::Black);
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            sf::RectangleShape cellShape(
                sf::Vector2f(static_cast<float>(cellSize),
                             static_cast<float>(cellSize))
            );
            cellShape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));

            float value = sampleField(x,y, grid);
            
            uint8_t colorValue = static_cast<uint8_t>(value * 255);
            cellShape.setFillColor(sf::Color(colorValue, colorValue, colorValue));

            window.draw(cellShape);
        }
    }
    
    window.display();
}

void Renderer::updateMode(InputHandler& input){
    if (input.isDown(sf::Keyboard::Key::U)){
        render_mode = 'U';
    } else if (input.isDown(sf::Keyboard::Key::V)){
        render_mode = 'V';
    } else if (input.isDown(sf::Keyboard::Key::P)){
        render_mode = 'P';
    } else if (input.isDown(sf::Keyboard::Key::S)){
        render_mode = 'S';
    } else if (input.isDown(sf::Keyboard::Key::T)){
        render_mode = 'T';
    } else if (input.isDown(sf::Keyboard::Key::M)){
        render_mode = 'M';
    }
}

float Renderer::sampleField(int x, int y, const Grid& grid){
    float value;
    switch(render_mode){
        case 'V':
        value = grid.v[grid.idx(x, grid.getHeight() -1 - y)];
        break;
        case 'U':
        value = grid.u[grid.idx(x, grid.getHeight() -1 - y)];
        break;
        case 'P':
        value = grid.pressure[grid.idx(x, grid.getHeight() -1 - y)];
        break;
        case 'T':
        value = grid.temperature[grid.idx(x, grid.getHeight() -1 - y)];
        break;
        case 'M':
        value = grid.mass[grid.idx(x, grid.getHeight() -1 - y)];
        break;
        default: 
        value = grid.smoke[grid.idx(x, grid.getHeight() -1 - y)];
    }
    return value;
}

sf::Window& Renderer::getWindow(){
    return window;
}