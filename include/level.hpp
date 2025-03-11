#pragma once

#include <vector>

#include "map.hpp"
#include "resource/texture.hpp"
#include "component/component.hpp"
#include "util/bsp.hpp"

struct Level {

    unsigned int                index;
    Map                         map;

    LogMgr*                     logMgr;
    std::unique_ptr<BSPTree>    bspTree;

    std::vector<sf::IntRect>    roomList;
    std::vector<EntityID>       creatureList;

    std::array<Terrain, (unsigned int)Terrain::Type::NumTerrainTypes>       terrains;
    std::array<Texture*, (unsigned int)Terrain::Type::NumTerrainTypes>      terrainTextures;

    static constexpr std::array<std::string_view, (unsigned int)Terrain::Type::NumTerrainTypes>     terrainNames{
        "ground", "wall", "water"
    };
    static constexpr std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>                 terrainWalkables{
        true, false, false
    };
    static constexpr std::array<bool, (unsigned int)Terrain::Type::NumTerrainTypes>                 terrainTransparents{
        true, false, true
    };

    sf::IntRect                 mapView;

};

void InitLevel(Level& level, unsigned int index, RandomNumberGenerator& rng);

void InitRooms(Level& level);

void InitView(Level& level, sf::Vector2u playerLocation, sf::Vector2u windowSize);
void UpdateView(Level& level, sf::Vector2u playerLocation);

void PlaceCreature(Level& level, CreatureComponent& creature);