#include "../include/tile.hpp"

void InitTile(Tile& tile, sf::Vector2u location, Terrain terrain) {
    tile.location = location;
    tile.terrain = terrain;
    tile.occupant = nullptr;
}
