#pragma once

#include <array>

#include "tile.hpp"

struct Map {
    static constexpr unsigned int       width{64};
    static constexpr unsigned int       height{64};

    std::array<Tile, width * height>    tiles;
};

void InitMap(Map& map, Terrain defaultTerrain);