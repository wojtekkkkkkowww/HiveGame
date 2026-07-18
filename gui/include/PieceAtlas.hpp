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

        const auto fullCellWidth =
            static_cast<int>(textureSize.x / hive::pieceCatalog.size());
        const auto horizontalInset = fullCellWidth / 25;
        const auto topInset = static_cast<int>(textureSize.y / 14);
        const auto bottomInset = static_cast<int>(textureSize.y / 7);
        const auto &definition = hive::pieceCatalog[hive::pieceIndex(pieceCode)];
        return {static_cast<int>(definition.atlasColumn) * fullCellWidth + horizontalInset,
                topInset, fullCellWidth - 2 * horizontalInset,
                static_cast<int>(textureSize.y) - topInset - bottomInset};
    }
};
