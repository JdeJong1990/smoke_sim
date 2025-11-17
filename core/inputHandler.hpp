#pragma once
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <optional>

class InputHandler {
public:
    InputHandler();

    // Call once per frame
    void update(sf::Window& window);

    // Query whether a given key is currently pressed
    bool isDown(sf::Keyboard::Key key) const;

private:
    std::vector<bool> keys;
};
