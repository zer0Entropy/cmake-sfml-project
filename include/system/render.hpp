#pragma once

#include <array>

#include <SFML/Graphics/RenderWindow.hpp>

struct GameState;

class RenderSystem {
public:
    RenderSystem() = delete;
    RenderSystem(const RenderSystem& copy) = delete;
    RenderSystem(RenderSystem& copy) = delete;
    RenderSystem(RenderSystem&& move) = delete;
    RenderSystem(sf::RenderWindow& window);
    ~RenderSystem();


    void                Update(GameState& gameState);
    
private:
    sf::RenderWindow&   mainWindow;
};