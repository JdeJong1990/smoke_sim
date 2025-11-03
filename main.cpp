#include <iostream>
#include <SFML/System/Clock.hpp>
#include "core/grid.hpp"
#include "core/solver.hpp"
#include "core/renderer.hpp"

int main() {
    const int width = 64;
    const int height = 64;
    const float dt = 0.016f;

    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;

    Grid grid(width, height);
    Solver solver;
    Renderer renderer(width, height, 8);

    std::cout << "Starting simulation...\n";

    while (renderer.isOpen()) {
        renderer.pollEvents();

        solver.step(grid, dt);
        renderer.draw(grid);

        frameCount++;

        // Print FPS once per second
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            float fps = frameCount / fpsClock.getElapsedTime().asSeconds();
            std::cout << "FPS: " << static_cast<int>(fps + 0.5f) << "      \r";
            frameCount = 0;
            fpsClock.restart();
        }
    }

    std::cout << "Simulation complete.\n";
    return 0;
}
