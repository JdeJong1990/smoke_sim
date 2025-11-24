#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <algorithm> 
#include <cstdint>
#include <iostream>
#include "inputHandler.hpp"
#include "renderer.hpp"
#include "rgb.hpp"

Renderer::Renderer(int width, int height, int cellSize)
    : window(sf::VideoMode({static_cast<unsigned int>(width * cellSize),
                            static_cast<unsigned int>(height * cellSize)}),
             "Grid Simulation"),
      cellSize(cellSize)
{
    window.setFramerateLimit(60);           // Set frame rate limit
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

            RGB color = sampleField(x,y, grid);
            
            // uint8_t colorValue = static_cast<uint8_t>(value * 255);
            cellShape.setFillColor(sf::Color(color.r, color.g, color.b));

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

RGB Renderer::sampleField(int x, int y, const Grid& grid){
    int index = grid.idx(x, grid.getHeight() - 1 - y);
    RGB color(1);

    switch(render_mode){
        case 'V':{
        color = sampleVelocity(x, y, grid);
        break;
        }
        case 'U':{
        color = RGB(grid.u[index] * 255);
        break;
        }
        case 'P':{
        color = RGB(grid.pressure[index] * 255);
        break;
        }
        case 'T':{
        color = RGB(grid.temperature[index] * 255);
        break;
        }
        case 'M':{
        color = RGB(grid.mass[index] * 255);
        break;
        }
        default:{
        color = RGB(grid.smoke[index] * 255);
        }
    }
    return color;
}

sf::Window& Renderer::getWindow(){
    return window;
}

RGB Renderer::sampleVelocity(int x, int y, const Grid& grid){
    int index = grid.idx(x, grid.getHeight() - 1 - y);
    float u = grid.u[index];
    float v = grid.v[index];

    return velocityColor(u,v);
}

inline RGB Renderer::velocityColor(float u, float v)
{
    float ux = u;
    float uy = -v;

    auto rotate = [](float ux, float uy, float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return std::pair<float,float>(
            c * ux - s * uy,
            s * ux + c * uy
        );
    };

    const float angleR = 0.0f;
    const float angleG = 2.0f * static_cast<float>(M_PI) / 3.0f;
    const float angleB = 4.0f * static_cast<float>(M_PI) / 3.0f;

    auto uvR = rotate(ux, uy, angleR);
    auto uvG = rotate(ux, uy, angleG);
    auto uvB = rotate(ux, uy, angleB);

    float lenR = std::hypot(uvR.first, uvR.second);
    float lenG = std::hypot(uvG.first, uvG.second);
    float lenB = std::hypot(uvB.first, uvB.second);

    float r = uvR.second + lenR;
    float g = uvG.second + lenG;
    float b = uvB.second + lenB;

    const float scaling = 300.0f;

    auto mapToByte = [&](float v)
    {
        float mapped = v * scaling * 0.5f + 0.5f;
        mapped = std::clamp(mapped, 0.0f, 255.0f);
        return static_cast<uint8_t>(mapped);
    };

    return RGB{
        mapToByte(r),
        mapToByte(g),
        mapToByte(b)
    };
}
