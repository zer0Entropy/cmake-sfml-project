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

    struct View {
        unsigned int            widthInPixels;
        unsigned int            heightInPixels;
        sf::Vector2u            sizeInTiles;
        sf::Vector2u            topLeft;
        sf::Vector2f            position;
        sf::Vector2u            centerLocation;
        std::vector<Tile*>      visibleTiles;
    };

    View                        mapView;

};

void InitLevel(Level& level, unsigned int index);

void InitView(Level::View& view, sf::Vector2f position, sf::Vector2u windowSize);
void UpdateView(Level& level, sf::Vector2u center);

void PlaceCreature(Level& level, CreatureComponent& creature);