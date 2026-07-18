#pragma once

#include "PieceCatalog.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <stdexcept>

class PieceAtlas
{
  public:
    static sf::IntRect textureRect(const sf::Texture &atlas, char pieceCode)
    {
        const auto textureSize = atlas.getSize();
        if (textureSize.x < hive::pieceCatalog.size() || textureSize.y == 0)
        {
            throw std::runtime_error("Piece atlas has invalid dimensions");
        }

        const auto cellWidth =
            static_cast<int>(textureSize.x / hive::pieceCatalog.size());
        const auto &definition = hive::pieceCatalog[hive::pieceIndex(pieceCode)];
        return {static_cast<int>(definition.atlasColumn) * cellWidth, 0, cellWidth,
                static_cast<int>(textureSize.y)};
    }
};
