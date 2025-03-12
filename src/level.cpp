#include <cassert>
#include <cmath>
#include "../include/level.hpp"
#include "../include/component/creature.hpp"
#include "../include/rng.hpp"

void InitLevel(Level& level, unsigned int index, RandomNumberGenerator& rng) {
    level.index = index;
    for(unsigned int terrainIndex = 0; terrainIndex < (unsigned int)Terrain::Type::NumTerrainTypes; ++terrainIndex) {
        InitTerrain(    level.terrains[terrainIndex],
                        (Terrain::Type)terrainIndex,
                        level.terrainNames[terrainIndex],
                        level.terrainWalkables[terrainIndex],
                        level.terrainTransparents[terrainIndex]);

    }
    InitMap(level.map, level.terrains[(int)Terrain::Type::Ground]);

    level.bspTree = std::make_unique<BSPTree>(sf::Vector2u{Map::width, Map::height}, *level.logMgr);

    int minWidth{4};
    int minHeight{4};
    int maxWidth{12};
    int maxHeight{12};

    unsigned int numIterations{3};
    BSPTree::Node* rootNode{level.bspTree->CreateRoot()};
    for(unsigned int n = 0; n < numIterations; ++n) {
        level.bspTree->Split(minWidth, minHeight, maxWidth, maxHeight, rng);
    }

    const auto& leafList{level.bspTree->GetLeafList()};
    for(const auto& leaf : leafList) {
        sf::Vector2i roomSize{
            minWidth + (int)rng.GetRandom(0, leaf->rect.size.x - minWidth),
            minHeight + (int)rng.GetRandom(0, leaf->rect.size.y - minHeight)
        };
        sf::Vector2i topLeft{
            leaf->rect.position.x + (int)rng.GetRandom(0, leaf->rect.size.x - roomSize.x),
            leaf->rect.position.y + (int)rng.GetRandom(0, leaf->rect.size.y - roomSize.y)
        };
        if(topLeft.x + roomSize.x < Map::width && topLeft.y + roomSize.y < Map::height) {
            level.roomList.push_back(sf::IntRect{topLeft, roomSize});
        }
    }
}

void InitRooms(Level& level) {
    for(const auto& room : level.roomList) {
        const sf::Vector2i topLeft{room.position.x, room.position.y};
        const sf::Vector2i bottomRight{room.position.x + room.size.x, room.position.y + room.size.y};

        assert(bottomRight.x < Map::width && "Room dimensions exceed Map::width");
        assert(bottomRight.y < Map::height && "Room dimensions exceed Map::height");

        for(unsigned int n = 0; n < room.size.x; ++n) {
            Tile& topTile{level.map.tiles[topLeft.y * Map::width + topLeft.x + n]};
            Tile& bottomTile{level.map.tiles[(topLeft.y + room.size.y) * Map::width + topLeft.x + n]};
            topTile.terrain = level.terrains[(int)Terrain::Type::Wall];
            bottomTile.terrain = level.terrains[(int)Terrain::Type::Wall];
        }
        for(unsigned int n = 0; n < room.size.y; ++n) {
            Tile& leftTile{level.map.tiles[(topLeft.y  + n) * Map::width + topLeft.x]};
            Tile& rightTile{level.map.tiles[(topLeft.y + n) * Map::width + topLeft.x + room.size.x]};
            leftTile.terrain = level.terrains[(int)Terrain::Type::Wall];
            rightTile.terrain = level.terrains[(int)Terrain::Type::Wall];
        }
    }
}

void InitView(Level& level, sf::Vector2u playerLocation, sf::Vector2u windowSize) {
  
    level.mapView.size = {
        5 * (int)(windowSize.x / Tile::widthInPixels) / 6,
        (int)floor(windowSize.y / Tile::heightInPixels)
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
    assert(level.mapView.position.x + level.mapView.size.x <= Map::width && "MapView bounds exceed Map width");
    assert(level.mapView.position.y + level.mapView.size.y <= Map::height && "MapView bounds exceed Map height");

    level.mapView.position = {
        (int)playerLocation.x - (level.mapView.size.x / 2),
        (int)playerLocation.y - (level.mapView.size.y / 2)
    };

    // If player is less than one-half the view's width from right edge, don't scroll further
    //if(playerLocation.x > Map::width - (level.mapView.size.x / 2)) {
    if(level.mapView.position.x + level.mapView.size.x >= Map::width) {
        level.mapView.position.x = Map::width - level.mapView.size.x;
    }
    // If player is less than one-half the view's width from left edge, don't scroll further
    else if(playerLocation.x < level.mapView.size.x / 2) {
        level.mapView.position.x = 0;
    }
    // If player is less than one-half the view's height from bottom edge, don't scroll further
    //if(playerLocation.y > Map::height - (level.mapView.size.y / 2)) {
    if(level.mapView.position.y + level.mapView.size.y >= Map::height) {
        level.mapView.position.y = Map::height - level.mapView.size.y;
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