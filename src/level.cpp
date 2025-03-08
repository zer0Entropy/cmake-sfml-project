#include "../include/level.hpp"

void InitLevel(Level& level, unsigned int index) {
    level.index = index;
    for(unsigned int terrainIndex = 0; terrainIndex < (unsigned int)Terrain::Type::NumTerrainTypes; ++terrainIndex) {
        InitTerrain(    level.terrains[terrainIndex],
                        (Terrain::Type)terrainIndex,
                        level.terrainNames[terrainIndex],
                        level.terrainWalkables[terrainIndex],
                        level.terrainTransparents[terrainIndex]);

    }
    InitMap(level.map, Terrain{Terrain::Type::Ground, "ground", true, true});
}

void InitView(Level::View& view, sf::Vector2f position, sf::Vector2u windowSize) {
    view.sizeInTiles.x = 5 * (windowSize.x / Tile::widthInPixels) / 6;
    view.sizeInTiles.y = windowSize.y / Tile::heightInPixels;
    view.widthInPixels = view.sizeInTiles.x * Tile::widthInPixels;
    view.heightInPixels = view.sizeInTiles.y * Tile::heightInPixels;
    view.position = position;
    view.centerLocation = sf::Vector2u{0, 0};
}

void UpdateView(Level& level, sf::Vector2u center) {
    level.mapView.visibleTiles.clear();
    level.mapView.centerLocation = center;
    level.mapView.topLeft = sf::Vector2u{
        center.x - (level.mapView.sizeInTiles.x / 2),
        center.y - (level.mapView.sizeInTiles.y / 2)
    };
    if(level.mapView.topLeft.x >= Map::width) {
        level.mapView.topLeft.x = 0;
    }
    if(level.mapView.topLeft.y >= Map::height) {
        level.mapView.topLeft.y = 0;
    }
    for(unsigned int y = level.mapView.topLeft.y; y < level.mapView.topLeft.y + level.mapView.sizeInTiles.y; ++y) {
        if(y >= level.map.height) {
            break;
        }
        for(unsigned int x = level.mapView.topLeft.x; x < level.mapView.topLeft.x + level.mapView.sizeInTiles.x; ++x) {
            if(x >= level.map.width) {
                break;
            }
            level.mapView.visibleTiles.push_back(&level.map.tiles[y * Map::width + x]);
        }
    }
}