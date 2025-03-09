#pragma once

#include <vector>

#include "map.hpp"
#include "resource/texture.hpp"
#include "component/component.hpp"

struct Level {

    unsigned int                index;
    Map                         map;

    std::vector<EntityID>       creatureList;

    std::array<Terrain, (unsigned int)Terrain::Type::NumTerrainTypes>       terrains;
    std::array<Texture*, (unsigned int)Terrain::Type::NumTerrainTypes>      terrainTextures;
    std::array<std::string, (unsigned int)Terrain::Type::NumTerrainTypes>   terrainNames;
    std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>          terrainWalkables;
    std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>          terrainTransparents;

    sf::IntRect                 mapView;

};

void InitLevel(Level& level, unsigned int index);

void InitView(Level& level, sf::Vector2u playerLocation, sf::Vector2u windowSize);
void UpdateView(Level& level, sf::Vector2u playerLocation);

void PlaceCreature(Level& level, CreatureComponent& creature);