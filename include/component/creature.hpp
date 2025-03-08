#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

#include "component.hpp"
#include "../resource/texture.hpp"

struct CreatureComponent: public Component {
    std::string         name;
    sf::Vector2u        location;
};

void InitCreature(CreatureComponent& creature, std::string_view name, sf::Vector2u location);