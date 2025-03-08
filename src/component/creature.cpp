#include "../../include/component/creature.hpp"

void InitCreature(CreatureComponent& creature, std::string_view name, sf::Vector2u location) {
    creature.name = name;
    creature.location = location;
}