#include "../../include/component/creature.hpp"

void InitCreature(CreatureComponent& creature, std::string_view name, sf::Vector2u location, Texture* texture) {
    creature.name = name;
    creature.location = location;
    creature.texture = texture;
}