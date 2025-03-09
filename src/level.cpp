#include "../include/level.hpp"
#include "../include/component/creature.hpp"

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

void InitView(Level& level, sf::Vector2u playerLocation, sf::Vector2u windowSize) {
  
    level.mapView.size = {
        5 * (int)(windowSize.x / Tile::widthInPixels) / 6,
        (int)(windowSize.y / Tile::heightInPixels)
    };

    level.mapView.position = {
        (int)playerLocation.x - (level.mapView.size.x / 2),
        (int)playerLocation.y - (level.mapView.size.y / 2)
    };

    // If player is less than one-half the view's width from right edge, don't scroll further
    if(playerLocation.x > Map::width - (level.mapView.size.x / 2)) {
        level.mapView.position.x = Map::width - (level.mapView.size.x / 2);
    }
    // If player is less than one-half the view's width from left edge, don't scroll further
    else if(playerLocation.x < level.mapView.size.x / 2) {
        level.mapView.position.x = 0;
    }
    // If player is less than one-half the view's height from bottom edge, don't scroll further
    if(playerLocation.y > Map::height - (level.mapView.size.y / 2)) {
        level.mapView.position.y = Map::height - (level.mapView.size.y / 2);
    }
    // If player is less than one-half the view's height from top edge, don't scroll further
    else if(playerLocation.y < level.mapView.size.y / 2) {
        level.mapView.position.y = 0;
    }

}

void UpdateView(Level& level, sf::Vector2u playerLocation) {
    level.mapView.position = {
        (int)playerLocation.x - (level.mapView.size.x / 2),
        (int)playerLocation.y - (level.mapView.size.y / 2)
    };

    // If player is less than one-half the view's width from right edge, don't scroll further
    if(playerLocation.x > Map::width - (level.mapView.size.x / 2)) {
        level.mapView.position.x = Map::width - (level.mapView.size.x / 2);
    }
    // If player is less than one-half the view's width from left edge, don't scroll further
    else if(playerLocation.x < level.mapView.size.x / 2) {
        level.mapView.position.x = 0;
    }
    // If player is less than one-half the view's height from bottom edge, don't scroll further
    if(playerLocation.y > Map::height - (level.mapView.size.y / 2)) {
        level.mapView.position.y = Map::height - (level.mapView.size.y / 2);
    }
    // If player is less than one-half the view's height from top edge, don't scroll further
    else if(playerLocation.y < level.mapView.size.y / 2) {
        level.mapView.position.y = 0;
    }
}

void PlaceCreature(Level& level, CreatureComponent& creature) {
    level.map.tiles[creature.location.y * Map::width + creature.location.x].occupant = &creature;
    level.creatureList.push_back(creature.ownerID);
}