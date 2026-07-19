#pragma once

#include "Tile.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace hive
{
struct PieceDefinition
{
    PieceType type;
    std::uint8_t initialCount;
    std::string_view assetName;
    std::uint8_t atlasColumn;
};

inline constexpr std::array pieceCatalog = {
    PieceDefinition{PieceType::Ant, 3, "A.png", 1},
    PieceDefinition{PieceType::Beetle, 2, "B.png", 4},
    PieceDefinition{PieceType::Grasshopper, 3, "G.png", 3},
    PieceDefinition{PieceType::Queen, 1, "Q.png", 0},
    PieceDefinition{PieceType::Spider, 2, "S.png", 2},
};

constexpr std::size_t pieceIndex(PieceType type)
{
    for (std::size_t index = 0; index < pieceCatalog.size(); ++index)
    {
        if (pieceCatalog[index].type == type)
        {
            return index;
        }
    }
    throw std::invalid_argument("Invalid piece type");
}
} // namespace hive
