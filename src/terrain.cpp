#include "../include/terrain.hpp"

void InitTerrain(Terrain& terrain, Terrain::Type type, std::string_view name, bool walkable, bool transparent) {
    terrain.type = type;
    terrain.name = name;
    terrain.isWalkable = walkable;
    terrain.isTransparent = transparent;
}

