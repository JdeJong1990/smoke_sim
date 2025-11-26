#pragma once
#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include "inputHandler.hpp"
#include "rgb.hpp"

/**
* Render a real time image of the Grid data in a window. 
* 
* Usage: 
* Renderer renderer(width, height, cellSize);
* renderer.draw(grid, input);
*/
class Renderer {
public:
    
    Renderer(int width, int height, int cellSize = 4);

    /**
    * @brief Draws a visualization of the state of the Grid to a window. 
    *
    * @param grid   Grid object containing physical data. 
    * @param input  InputHandler object that stores the inputs of the user. 
    */
    void draw(const Grid& grid, InputHandler& input);

    /**
    * @brief Updates the Renderer render_mode based on the input from the user.
    * @param input  InputHandler object that stores the inputs of the user. 
    */ 
    void updateMode(InputHandler& input);

    /// @brief Samples one of the fields of a Grid object, on a given coordinate, based on the render_mode.
    RGB sampleField(int x, int y, const Grid& grid);

    RGB sampleVelocity(int x, int y, const Grid& grid);

    RGB velocityColor(float u, float v);

    /// @brief Check if the window is closed.
    void pollEvents();

    bool isOpen() const { return window.isOpen(); }
    char render_mode;
    sf::Window& getWindow();

private:
    sf::RenderWindow window;

    // Number of pixels per cell, for rendering.
    int cellSize;
};
