#pragma once

#include <vector>

#include "map.hpp"
#include "resource/texture.hpp"
#include "component/component.hpp"
#include "util/bsp.hpp"

struct Level {

    unsigned int                index;
    Map                         map;

    BSPTree                     bspTree;

    std::vector<sf::IntRect>    roomList;
    std::vector<EntityID>       creatureList;

    std::array<Terrain, (unsigned int)Terrain::Type::NumTerrainTypes>       terrains;
    std::array<Texture*, (unsigned int)Terrain::Type::NumTerrainTypes>      terrainTextures;
    std::array<std::string, (unsigned int)Terrain::Type::NumTerrainTypes>   terrainNames;
    std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>          terrainWalkables;
    std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>          terrainTransparents;

    sf::IntRect                 mapView;

    Level();
    ~Level();

};

void InitLevel(Level& level, unsigned int index, RandomNumberGenerator& rng);

void InitView(Level& level, sf::Vector2u playerLocation, sf::Vector2u windowSize);
void UpdateView(Level& level, sf::Vector2u playerLocation);

void PlaceCreature(Level& level, CreatureComponent& creature);