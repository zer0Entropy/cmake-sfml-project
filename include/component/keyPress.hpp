#pragma once

#include "component.hpp"
#include "../util/observer.hpp"
#include "../ui/action.hpp"

struct KeyPressComponent: public Component, public Observer {

    void Notify(GameState& gameState, const std::optional<sf::Event> event = std::nullopt) const;

};