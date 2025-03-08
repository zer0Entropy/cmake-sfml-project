#pragma once

#include <SFML/System/Vector2.hpp>

#include "terrain.hpp"

struct Creature;

struct Tile {

    static constexpr unsigned int   widthInPixels{72};
    static constexpr unsigned int   heightInPixels{72};

    sf::Vector2u        location;
    Terrain             terrain;
    Creature*           occupant;

};

void InitTile(Tile& tile, sf::Vector2u location, Terrain terrain);