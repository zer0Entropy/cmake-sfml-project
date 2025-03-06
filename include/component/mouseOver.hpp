#pragma once

#include "component.hpp"
#include "../util/observer.hpp"
#include "../ui/action.hpp"

struct GameState;

struct MouseOverComponent: public Component, public Observer {

    ActionID    triggeredAction;

    void Notify(GameState& gameState, const std::optional<sf::Event> event = std::nullopt) const;
};