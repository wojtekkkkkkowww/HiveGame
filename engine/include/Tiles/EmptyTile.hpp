#pragma once

#include "Tile.hpp"

namespace hge
{
    class EmptyTile : public Tile
    {
    public:
        EmptyTile() : Tile(TileType::EMPTY) {}
    };
}
