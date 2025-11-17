#include "InputHandler.hpp"
#include <SFML/Window/Event.hpp>

InputHandler::InputHandler() {
    keys.resize(static_cast<size_t>(sf::Keyboard::KeyCount), false);
}

void InputHandler::update(sf::Window& window) {
    // Reset all keys to "not pressed" before re-checking
    std::fill(keys.begin(), keys.end(), false);

    // Poll SFML events
    while (auto eventOpt = window.pollEvent()) {
        if (!eventOpt) break;
        const sf::Event& event = *eventOpt;

        // Close window
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }

        // Key pressed
        if (event.is<sf::Event::KeyPressed>()) {
            const auto* kp = event.getIf<sf::Event::KeyPressed>();
            if (kp)
                keys[static_cast<int>(kp->code)] = true;
        }

        // Key released
        if (event.is<sf::Event::KeyReleased>()) {
            const auto* kr = event.getIf<sf::Event::KeyReleased>();
            if (kr)
                keys[static_cast<int>(kr->code)] = false;
        }
    }

    // Also capture real-time key state (optional, if you want hold-keys reliably)
    for (int i = 0; i < static_cast<int>(sf::Keyboard::KeyCount); ++i) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
            keys[i] = true;
    }
}

bool InputHandler::isDown(sf::Keyboard::Key key) const {
    return keys[static_cast<int>(key)];
}
