#pragma once

#include <string>

struct Terrain {

    enum class Type {
        Ground,
        Wall,
        Water,
        NumTerrainTypes
    };

    Type            type;
    std::string     name;
    bool            isWalkable;
    bool            isTransparent;

};

void InitTerrain(Terrain& terrain, Terrain::Type type, std::string_view name, bool walkable, bool transparent);