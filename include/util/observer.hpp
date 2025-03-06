#pragma once

#include <optional>
#include <SFML/Window/Event.hpp>

struct GameState;

struct Observer {
    virtual void Notify(GameState& gameState, const std::optional<sf::Event> event = std::nullopt) const = 0;
};