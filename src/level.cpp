#include "../include/level.hpp"
#include "../include/component/creature.hpp"
#include "../include/rng.hpp"

Level::Level():
    bspTree{sf::Vector2u{Map::width, Map::height}} {

}

Level::~Level() {

}

void InitLevel(Level& level, unsigned int index, RandomNumberGenerator& rng) {
    level.index = index;
    for(unsigned int terrainIndex = 0; terrainIndex < (unsigned int)Terrain::Type::NumTerrainTypes; ++terrainIndex) {
        InitTerrain(    level.terrains[terrainIndex],
                        (Terrain::Type)terrainIndex,
                        level.terrainNames[terrainIndex],
                        level.terrainWalkables[terrainIndex],
                        level.terrainTransparents[terrainIndex]);

    }
    InitMap(level.map, Terrain{Terrain::Type::Ground, "wall", true, true});

    unsigned int numIterations{4};
    BSPTree::Node* rootNode{level.bspTree.CreateNode(sf::IntRect{{0, 0}, {Map::width, Map::height}})};
    for(unsigned int n = 0; n < numIterations; ++n) {
        level.bspTree.SplitNode(*rootNode, rng);
    }

    const auto& leafList{level.bspTree.GetLeafList()};
    for(const auto& leaf : leafList) {
        sf::Vector2i topLeft, bottomRight;
        do {
            int topLeftMinX{0};
            int topLeftMaxX{0};
            int topLeftMinY{0};
            int topLeftMaxY{0};
            do {
                topLeftMinX = leaf->rect.position.x;
                topLeftMaxX = leaf->rect.position.x + (leaf->rect.size.x / 2);
                topLeftMinY = leaf->rect.position.y;
                topLeftMaxY = leaf->rect.position.y + (leaf->rect.size.y / 2);
            } while(topLeftMaxX < topLeftMinX || topLeftMaxY < topLeftMinY);

            topLeft = {
                (int)rng.GetRandom(topLeftMinX, topLeftMaxX),
                (int)rng.GetRandom(topLeftMinY, topLeftMaxY) 
            };

            int bottomRightMinX{0};
            int bottomRightMaxX{0};
            int bottomRightMinY{0};
            int bottomRightMaxY{0};
            do {
                bottomRightMinX = topLeft.x + BSPTree::Node::minWidth;
                bottomRightMaxX = leaf->rect.position.x + leaf->rect.size.x;
                bottomRightMinY = topLeft.y + BSPTree::Node::minHeight;
                bottomRightMaxY = leaf->rect.position.y + leaf->rect.size.y;
            } while(bottomRightMaxX < bottomRightMinX || bottomRightMaxY < bottomRightMinY);

            bottomRight = {
                (int)rng.GetRandom(bottomRightMinX, bottomRightMaxX),
                (int)rng.GetRandom(bottomRightMinY, bottomRightMaxY)
            };
        } while(bottomRight.x <= topLeft.x || bottomRight.y <= topLeft.y);

        level.roomList.push_back(sf::IntRect{
            topLeft,
            sf::Vector2i{bottomRight.x - topLeft.x, bottomRight.y - topLeft.y}
        });
    }
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