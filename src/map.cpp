#include "../include/map.hpp"

void InitMap(Map& map, Terrain defaultTerrain) {
    for(unsigned int y = 0; y < Map::height; ++y) {
        for(unsigned int x = 0; x < Map::width; ++x) {
            Tile& tile{map.tiles[y * Map::width + x]};
            tile.location = sf::Vector2u{x, y};
            tile.terrain = defaultTerrain;
            tile.occupant = nullptr;
        }
    }
}