#pragma once

#include <array>

#include <SFML/Graphics/RenderWindow.hpp>

struct GameState;

class InputSystem {
public:

    InputSystem() = delete;
    InputSystem(const InputSystem& copy) = delete;
    InputSystem(InputSystem& copy) = delete;
    InputSystem(InputSystem&& move) = delete;
    InputSystem(sf::RenderWindow& window);
    ~InputSystem();

    void                Update(GameState& gameState);

private:
    sf::RenderWindow&   mainWindow;
};